#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeError.h>

namespace Xe {
	namespace Error {
		// An invalid descriptor was specified.
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_NET, 9, SOCKET_BADDESCRIPTOR);
		// The socket is marked nonblocking and the requested operation would block.
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_NET, 11, SOCKET_AGAIN);
		// Connection refused by the server.
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_NET, 107, SOCKET_CONNREFUSED);
		// Connection reset by peer.
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_NET, 108, SOCKET_CONNRESET);
		// The socket is not connected, and no target has been given.
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_NET, 126, SOCKET_NOTCONNECTED);
		// The socket is marked nonblocking and the requested operation would block.
		XE_CREATE_RESULT(1, 0, HM_SDK, LM_NET, 140, SOCKET_WOULDBLOCK);
	}
	namespace Net {
		enum AddressFamily {
			InterNetwork = 2, // AF_INET
			//InterNetworkV6 = 23,
		};
		enum SocketType {
			Stream = 1,
		};
		enum ProtocolType {
			Undefined = 0,
			Tcp = 6,
		};

		union IPv4 {
			u8 data[4];
			u32 addr;
		};
		union IPv6 {
			u8 data[16];
			struct {
				u64 addr_1;
				u64 addr_2;
			};
		};
		struct EndPoint {
			EndPoint(u16 port);
			EndPoint(const IPv4& ip, u16 port);
			//EndPoint(const IPv6& ip, u16 port);

			AddressFamily GetAddressFamily() const;
			const void *GetData() const;
			int GetLength() const;
		private:
			union SockAddr {
				struct my_sockaddr {
					u16 AddressFamily;
					u8 Data[14];
				};
				struct my_sockaddr_in {
					u16 AddressFamily;
					u16 Port;
					IPv4 Address;
					u8 Zero[8];
				};

				u8 Data[16];
				u16 wData[8];
				u32 dData[4];
				u64 qData[2];
				my_sockaddr m_SockAddr;
				my_sockaddr_in m_SockAddrIn;
			};
			SockAddr m_SockAddr;
			int m_Length;
		};

		class Socket : public IObject {
		public:
			static const UID ID = 0xa99960ffd4b14ce5ULL;

			static RESULT Create(Socket** ppSocket, AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType);

			const AddressFamily m_AddressFamily;
			const SocketType m_SocketType;
			const ProtocolType m_ProtocolType;
			
			Socket(const Socket& socket);
			Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType);

			bool IsConnected() const;

			virtual RESULT Connect(const EndPoint& endPoint) = 0;
			virtual RESULT Connect(const void* sockaddr, int namelen) = 0;
			virtual RESULT Bind(const EndPoint& endPoint) = 0;
			virtual RESULT Bind(const void* sockaddr, int namelen) = 0;
			virtual RESULT Listen(int backlog) = 0;
			virtual RESULT Accept(Socket **ppSocket) = 0;
			virtual RESULT Send(const void *data, int offset, int length) = 0;

			//! \brief Receive a message from an opened connection.
			//! \param[out] data where the content will be written.
			//! \param[in] offset of data.
			//! \param[in,out] length of data buffer and how much data was read.
			/** \details After the call, length contains how bytes was received;
			 * if the message is too long to fit in the supplied buffer,
			 * excess bytes may be discarded depending on the type of socket
			 * the message is received from.
			 * If no messages are available at the socket, the receive calls
			 * wait for a message to arrive, unless the socket is nonblocking,
			 * in which case the value -1 is assigned to length and return
			 * value will be assigned to SOCKET_AGAIN or SOCKET_WOULDBLOCK.
			 */
			virtual RESULT Receive(void *data, int offset, int& length) = 0;

		protected:
			bool m_IsConnected;
		};
	}
}