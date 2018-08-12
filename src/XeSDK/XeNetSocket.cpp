#include "pch.h"
#include <XeSDK/INetSocket.h>
#include <XeSDK/XeValue.h>

#include <cerrno>

#ifndef PLATFORM_WINDOWS
#include <sys/socket.h>
#include <unistd.h>
#elif PLATFORM_UWP
#include <Winsock2.h>
#endif

namespace Xe {
    namespace Net {
        EndPoint::EndPoint(u16 port)
        {
            m_SockAddr.m_SockAddrIn.AddressFamily = InterNetwork;
            m_SockAddr.m_SockAddrIn.Port = Int16(port).SwapEndian();
            m_SockAddr.dData[1] = 0;
            m_SockAddr.dData[2] = 0;
            m_SockAddr.qData[1] = 0;
            m_Length = 16;
        }
        EndPoint::EndPoint(const IPv4& ip, u16 port)
        {
            m_SockAddr.m_SockAddrIn.AddressFamily = InterNetwork;
            m_SockAddr.m_SockAddrIn.Port = Int16(port).SwapEndian();
            m_SockAddr.m_SockAddrIn.Address = ip;
            m_SockAddr.qData[1] = 0;
            m_Length = 16;
        }
        /*EndPoint::EndPoint(const IPv6& ip, u16 port)
        {
            m_SockAddr.qData[0] = 0;
            m_SockAddr.qData[1] = 0;
            m_Length = 32;
        }*/
        AddressFamily EndPoint::GetAddressFamily() const {
            return (AddressFamily)m_SockAddr.m_SockAddr.AddressFamily;
        }
        const void* EndPoint::GetData() const {
            return &m_SockAddr;
        }
        int EndPoint::GetLength() const {
            return m_Length;
        }

        int g_SocketRef = 0;
        struct CSocket : public Socket {
#ifndef _WIN32
            typedef int SOCKET;
#endif

            SOCKET m_Socket;

            bool Query(IObject **obj, UID id) {
                switch (id) {
                    case Socket::ID:
                    case IObject::ID:
                        *obj = this;
                        return true;
                }
                *obj = nullptr;
                return false;
            }
            CSocket(SOCKET fd, const CSocket& socket) :
                    m_Socket(fd), Socket(socket)
            {}
            CSocket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) :
                    Socket(addressFamily, socketType, protocolType)
            {
#ifdef _WIN32
                if (g_SocketRef <= 0) {
					WSADATA wsaData;
					int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
					if (r != 0) {
						// FATAL ERROR
					}
				}
#endif
                m_Socket = socket(addressFamily, socketType, protocolType);
            }

            ~CSocket() {
#ifdef _WIN32
                closesocket(m_Socket);
                if (--g_SocketRef == 0) {
					WSACleanup();
				}
#else
                close(m_Socket);
#endif
            }

            RESULT Errno() {
                int err = errno;
                switch (err) {
                    case EBADF: return Error::SOCKET_NOTCONNECTED;
                    //case EAGAIN: return Error::SOCKET_AGAIN;
                    case ECONNREFUSED: return Error::SOCKET_CONNREFUSED;
                    case ECONNRESET: return Error::SOCKET_CONNRESET;
                    case ENOTCONN: return Error::SOCKET_NOTCONNECTED;
                    //case EWOULDBLOCK: return Error::SOCKET_WOULDBLOCK;
                    default: return Error::GENERIC_ERROR;
                }
            }

            RESULT Connect(const EndPoint& endPoint) {
                return Connect(endPoint.GetData(), endPoint.GetLength());
            }
            RESULT Connect(const void* psockaddr, int namelen) {
                // Non deve essere in ascolto.
                // Non deve essere gi� connesso.
                int r = connect(m_Socket, (const sockaddr*)psockaddr, namelen);
                if (r != 0) {
                    m_IsConnected = false;
                    return Errno();
                }
                m_IsConnected = true;
                return Error::OK;
            }
            RESULT Bind(const EndPoint& endPoint) {
                return Bind(endPoint.GetData(), endPoint.GetLength());
            }
            RESULT Bind(const void* psockaddr, int namelen) {
                int r = bind(m_Socket, (const sockaddr*)psockaddr, namelen);
                return r >= 0 ? Error::OK : Errno();
            }
            RESULT Listen(int backlog) {
                int r = listen(m_Socket, backlog);
                return r >= 0 ? Error::OK : Errno();
            }
            RESULT Accept(Socket **ppSocket) {
                SOCKET r = accept(m_Socket, 0, 0);
                if (r < 0) return Errno();
                *ppSocket = new CSocket(r, *this);
                return Error::OK;
            }

            RESULT Send(const void *data, int offset, int length) {
                int r = send(m_Socket, (const char*)data + offset, length, 0);
                return r >= 0 ? Error::OK : Errno();
            }
            RESULT Receive(void *data, int offset, int& length) {
                length = recv(m_Socket, (char*)data + offset, length, 0);
                return length >= 0 ? Error::OK : Errno();
            }
        };

        RESULT Socket::Create(Socket** ppSocket, AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) {
            *ppSocket = new CSocket(addressFamily, socketType, protocolType);
            return Error::OK;
        }
        Socket::Socket(const Socket& socket) :
                m_AddressFamily(socket.m_AddressFamily),
                m_SocketType(socket.m_SocketType),
                m_ProtocolType(socket.m_ProtocolType),
                m_IsConnected(false)
        { }
        Socket::Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) :
                m_AddressFamily(addressFamily),
                m_SocketType(socketType),
                m_ProtocolType(protocolType),
                m_IsConnected(false)
        { }
        bool Socket::IsConnected() const {
            return m_IsConnected;
        }
    }
}