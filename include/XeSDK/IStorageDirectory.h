#pragma once
#include <XeSDK/IObject.h>

namespace Xe {
	namespace Storage {
		// \brief directory enumeration
		class IDirectory : public IObject {
		public:
			//! \brief interface id
			static const UID ID = 0xb77760357c0c4b63ULL;

			//! \brief Get the path of current IDirectory
			//! \return A path; this should never return nullptr
			/** \details This is a value that will never change upon the
			 * entire life-cycle of IDirectory.
			 */
			virtual ctstring GetPath() const = 0;

			//! \brief Fill current information with next entry of directory.
			//! \return false when there are no other entries to process
			/** \details when this is called, values obtained from GetName,
			 * IsDirectory and GetLength are modified.
			 */
			virtual bool Next() = 0;

			//! \brief Get the name of current entry
			//! \return content may be invalidated on Next()
			virtual ctstring GetName() const = 0;

			//! \brief Check if current entry is a directory or not
			//! \return true if it's a directory
			virtual bool IsDirectory() const = 0;

			//! \brief Get the length of current file
			//! \return -1 if IsDirectory() is true
			virtual s64 GetLength() const = 0;
		};
	}
}