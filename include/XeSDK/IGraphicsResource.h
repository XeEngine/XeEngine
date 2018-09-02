#pragma once

namespace Xe {
	namespace Graphics {
		//! \brief what are the read and write permission for the buffer
		enum UsageType
		{
			//! \brief CPU --, GPU RW
			Usage_Standard,
			//! \brief CPU --, GPU R-
			Usage_Static,
			//! \brief CPU -W, GPU R-
			Usage_Dynamic,
			//! \brief CPU RW, GPU RW
			Usage_Full,
		};
		//! \brief type of locking
		enum LockType
		{
			//! \brief offers only the reading of the resource
			Lock_Read = 1,
			//! \brief offers only the writing of the resource deleting the previous content
			Lock_Write = 2,
			//! \brief offers both reading and writing of the resource
			Lock_ReadWrite = 3
		};
		//! \brief description of data to sent or to be received from the GPU
		/** \details locked resources can be read / written from 'data'.
		* Usually data from CPU and GPU isn't aligned at the same way, so
		* pitch value is filled to know how correctly handle the alignment.
		*/
		struct DataDesc {
			//! \brief data pointer
			const void *data;
			//! \brief how bytes there are in every row
			svar pitch;

			DataDesc();
			DataDesc(const void *data, svar pitch);
		};

		interface IContext;

		//! \brief IResource interface
		//! \details Basic management for every resource in VRAM
		class IResource : public IObject
		{
			bool m_isLocked;
		protected:
			IContext* m_pContext;
			UsageType m_Usage;
			svar m_length;

			virtual bool SubLock(DataDesc& map, LockType type) = 0;
			virtual void SubUnlock() = 0;
		public:
			//! \brief set basic resource attributes
			//! \param[in] video resource that creates this resource
			IResource(IContext* pContext, UsageType usage);

			//! \brief release all resources
			virtual ~IResource();

			//! \brief get the context which created this resource
			void GetContext(IContext **ppContext) const;

			//! \brief get the length of current buffer
			//! \return length in bytes
			svar GetLength() const;

			//! \brief Get how the resource can be managed
			UsageType GetUsageType() const;

			//! \brief check if the resource is currently locked
			//! \sa Lock, Unlock
			bool IsLocked() const;

			//! \brief lock current resource, if possible
			//! \param[out] map of locked data
			//! \param[in] type of lock
			//! \return true if resource was locked with success
			/** \details locking is useful when reading or writing is
			* needed. if the buffer is static, it cannot be lockable
			* and this method fails. Does the right checks about
			* IsLocked, then call SubLock.
			*/
			//! \sa IsLocked, Unlock
			bool Lock(DataDesc& map, LockType type);
			//! \brief unlock current resource if it was previously locked
			/** \details unlock the buffer from previous locking. Does the
			* right checks about IsLocked, then call SubUnlock.
			*/
			//! \sa IsLocked, Lock
			void Unlock();
		};
	}
}