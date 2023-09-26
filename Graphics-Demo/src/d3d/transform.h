#pragma once

//#include <DirectXMath.h>

namespace DirectX
{
	struct XMMATRIX;
}

namespace d3d
{
	struct TransformData
	{
		float posX{ 0.0f };
		float posY{ 0.0f };
		float posZ{ 0.0f };
		
		float rotX{ 0.0f };
		float rotY{ 0.0f };
		float rotZ{ 0.0f };

		float scaleX{ 1.0f };
		float scaleY{ 1.0f };
		float scaleZ{ 1.0f };
	};

	class Transform
	{
	public:

		DirectX::XMMATRIX getTransformMatrix() const;

		void translate(float x, float y, float z);

		void rotate(float x, float y, float z);

		Transform(const TransformData& transformData);

		Transform() = default;

		~Transform() = default;

		//Transform(const Transform& transform) = delete;

		Transform(Transform&& transform) noexcept = default;

	private:

		TransformData m_transformData{};
	};
}
