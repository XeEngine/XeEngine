#pragma once

namespace Xe {
	namespace Math {
		template <typename T>
		struct Vector3;
		struct Matrix4 {
			static const int Width = 4;
			static const int Height = 4;

			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[Height][Width];
				float n[Width * Height];
			};

			Matrix4();
			Matrix4(float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44);

			Matrix4 &Translate(float x, float y, float z);
			Matrix4 &Translate(const Math::Vector3<float> &translate);
			Matrix4 &Scale(float x, float y, float z);
			Matrix4 &Scale(const Math::Vector3<float> &scale);
			Matrix4 &RotateX(float radiant);
			Matrix4 &RotateY(float radiant);
			Matrix4 &RotateZ(float radiant);
			Matrix4 &Rotation(float pit, float y, float z);
			Matrix4 &Rotation(const Math::Vector3<float> &rotation);
			Matrix4 Multiply(const Matrix4& m) const;
			Matrix4 Inverse() const;

			static Matrix4 Identity();
			static Matrix4 Ortho(float left, float right, float bottom, float top, float near, float far);
		};
	}
}