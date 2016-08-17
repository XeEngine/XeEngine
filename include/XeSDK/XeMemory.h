#pragma once
#include <XeSDK/XeDef.h>

namespace Xe
{
	class Memory
	{
	public:
		//! \brief allocate a portion of memory
		//! \param[in] length length in bytes of the memory to allocate
		//! \return returns the pointer of the allocated memory, if nullptr the allocation failed
		//! \details the allocated memory will not be initialized
		static void *Alloc(uvar length);

		//! \brief allocate a portion of memory
		//! \param[in] length length in bytes of the memory to allocate
		//! \return returns the pointer of the allocated memory, if nullptr the allocation failed
		//! \details the allocated memory will be zeroed
		static void *AllocZero(uvar length);

		//! \brief resize a portion of memory
		//! \param[in] mem the pointer that currently handles the memory
		//! \param[in] length the new length in bytes of the memory
		//! \return the new resized portion of memory
		//! \details if the memory handle was not previously allocated, it will be allocated with this method
		static void *Resize(void *mem, uvar length);

		//! \brief free a portion of memory
		//! \param[in] mem the memory that will be released
		//! \return nullptr, always
		//! \details the content on the specified portion of memory will be not available anymore
		static void *Free(void *mem);

		//! \brief copy a portion of memory into another
		//! \param[in] dst the destination where the content is to be copied
		//! \param[in] src the source to be copied
		//! \param[in] length number of bytes to copy
		static void Copy(void* dst, const void* src, uvar length);

		//! \brief move a portion of memory into another
		//! \param[in] dst the destination where the content is to be moved
		//! \param[in] src the source to be moved
		//! \param[in] length number of bytes to move
		static void Move(void* dst, const void* src, uvar length);

		//! \brief fill the specified memory with a value
		//! \param[in] dst the destination where the content will be replaced
		//! \param[in] value the number of bytes to write
		//! \param[in] length the byte value that will be used as filler
		static void Fill(void* dst, uvar value, uvar length);

		//! \brief fill the specified memory with a value
		//! \param[in] dst the destination where the content will be replaced
		//! \param[in] value the number of bytes to write
		//! \param[in] position in bytes for dst
		//! \param[in] length the byte value that will be used as filler
		static void Fill(void* dst, uvar value, svar position, uvar length);

		//! \brief compare two portions of memory
		//! \param[in] mem1 first portion of memory that will be compared
		//! \param[in] pos1 position of mem1
		//! \param[in] mem2 second portion of memory that will be compared
		//! \param[in] pos2 position of mem2
		//! \param[in] length maximum bytes to compare
		/** \return < 0 if mem1 is greater than mem2, > 0 if it is the opposite,
		* 0 if both shares the same data in different locations of memory.
		*/
		static svar Compare(const void* mem1, svar pos1, const void* mem2, svar pos2, uvar length);
	};
}