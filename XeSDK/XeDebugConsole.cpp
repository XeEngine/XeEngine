#include "pch.h"
#include <XeSDK/XeDebugConsole.h>
#include <XeSDK/XeString.h>
#ifdef PLATFORM_LIBC
#include <cstdio>
#include <cstdarg>
#endif

namespace Xe {
	namespace Debug {
		class Nullcon : public IConsole {
			Properties m_prop;
		public:
			bool Query(IObject **obj, UID id) {
				switch (id) {
				case IConsole::ID:
				case IObject::ID:
					this->AddRef();
					*obj = this;
					return true;
				}
				*obj = (IObject*)nullptr;
				return false;
			}
			Nullcon() {
			}
			const Properties &GetProperties() const {
				return m_prop;
			}
			void SetProperties(const Properties& properties) {
				m_prop = properties;
			}
			const Color &GetForeColor() const {
				return m_prop.foreColor;
			}
			void SetForeColor(const Color &color) {
				m_prop.foreColor = color;
			}
			void Clear() {

			}
			void PutChar(tchar ch) {
				switch (ch) {
				case '\b':
					if (m_prop.coord.x > 0)
						m_prop.coord.x--;
					break;
				case '\n':
				case '\r':
					m_prop.coord.x = 0;
					m_prop.coord.y += 1;
					break;
				case '\t':
					m_prop.coord.x = (m_prop.coord.x + 4) & ~3;
					break;
				default:
					m_prop.coord.x += 1;
					break;
				}
			}
			void Print(ctstring fmt, ...) {
				tchar buf[0x100];
				void *args;
				GET_ARGS_ADDRESS(args, fmt);
				int n = String::PrintArguments(buf, lengthof(buf), fmt, args);
				for (int i = 0; i < n; i++)
					PutChar(buf[i]);
			}
		};
		bool CreateNullcon(IConsole **console) {
			*console = new Nullcon;
			return true;
		}
	}
}