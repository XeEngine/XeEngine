#pragma once
#include <XeSDK/IObject.h>
#include <XeSDK/XeMathVector2.h>
#include <XeSDK/XeMathVector3.h>
#include <XeSDK/IGameComponent.h>

namespace Xe {
	namespace Game {
		namespace Component {
			class Position2d : public IComponent {
			public:
				static const UID ID = 0x0954c68e97eb4796ULL;
				bool Query(IObject **obj, UID id);

				Position2d();
				~Position2d();

				virtual void OnUpdate(float update);
				virtual void OnDraw();

				const Math::Vector3f &GetTranslation() const;
				const Math::Vector3f &SetTranslation(const Math::Vector3f &translation);
				const Math::Vector3f &GetCenter() const;
				const Math::Vector3f &SetCenter(const Math::Vector3f &center);
				const Math::Vector3f &GetRotation() const;
				const Math::Vector3f &SetRotation(const Math::Vector3f &rotation);
				const Math::Vector3f &GetScaling() const;
				const Math::Vector3f &SetScaling(const Math::Vector3f &scaling);
				const Math::Matrix4 &GetMatrix() const;

				const Math::Vector2f(&GetPosition2d() const)[4];

			private:
				Math::Vector3f m_translation;
				Math::Vector3f m_center;
				Math::Vector3f m_rotation;
				Math::Vector3f m_scaling;

				bool m_isTranslationInvalidated;
				bool m_isScalingInvalidated;
				bool m_isRotationXYInvalidated;
				bool m_isRotatedZ;
				bool m_isMatrixInvaldiated;

				Math::Matrix4 m_matrix;
				Math::Vector2f m_position2[4];

				void UpdateMatrix();
				void UpdatePosition2();
			};
		}
	}
}