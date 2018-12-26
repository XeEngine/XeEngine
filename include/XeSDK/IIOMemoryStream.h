#pragma once
#include <XeSDK/IIOStream.h>

namespace Xe
{
	namespace IO
	{
		//! \brief manage streams that works directly on main memory
		interface IMemoryStream : public IStream
		{
		public:
			//! \brief open a pre-existed and not-writable portion of memory
			//! \param[in] memory pointer where the data to open is contained
			//! \param[in] length size in bytes of portion of memory
			//! \param[in] free if the resource to open is free, then IMemoryStream will not deallocate it
			//! \return false if an error occurred
			/** \details the portion of memory and its content will not
			 * be copied into a new one, but it will be directly used;
			 * the content will not be resizable.
			 * IsReadable() = true
			 * IsWritable() = false
			 * IsOverflowable() = false
			 */
			static IMemoryStream* From(const void* memory, svar length, bool free = true);

			//! \brief open a pre-existed portion of memory
			//! \param[in] memory pointer where the data to open is contained
			//! \param[in] length size in bytes of portion of memory
			//! \param[in] free if the resource to open is free, then IMemoryStream will not deallocate it
			//! \return false if an error occurred
			/** \details the portion of memory and its content will not
			 * be copied into a new one, but it will be directly used;
			 * the content will not be resizable.
			 * IsReadable() = true
			 * IsWritable() = true
			 * IsOverflowable() = false
			 */
			//! \sa Open
			static IMemoryStream* From(void* memory, svar length, bool free = true);

			//! \brief create a portion of memory
			//! \param[in] length size in bytes for the portion of memory to create
			//! \param[in] resizable tell if the memory can be resized
			//! \return false if an error occurred
			/* \details
			 * IsReadable() = true
			 * IsWritable() = true
			 * IsOverflowable() = true
			 */
			//! \sa Create
			static IMemoryStream* New(svar length, bool resizable = false);

			//! \brief get the current portion of memory used
			//! \return portion of memory
			//! \warning use the returned value with consciousness 
			virtual void* GetMemory() = 0;

			template <typename T>
			T* GetData() { return GetMemory(); }

			void* GetData() { return GetData<void>(); }
		};
	}
}