#pragma once
#include <XeSDK/IGraphicsResource.h>

namespace Xe {
	namespace Graphics {
		//! \brief type of the buffer
		enum BufferType
		{
			//! \brief vertex buffer
			BufferType_Vertex,
			//! \brief index buffer
			BufferType_Index,
		};

		//! \brief indirect buffer structure
		class IBuffer : public IResource
		{
		protected:
			BufferType m_type;
		public:
			IBuffer(IContext* pContext, UsageType usage, svar length, BufferType type);
			virtual ~IBuffer();

			BufferType GetType() const;
		};
	}
}