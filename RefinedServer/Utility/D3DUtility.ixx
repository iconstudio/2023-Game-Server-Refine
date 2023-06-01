module;
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <type_traits>

export module Utility.D3D.Tools;
export import Utility.D3D.Vector;
export import Utility.D3D.Matrix;

using namespace ::DirectX;
using namespace ::DirectX::PackedVector;

export namespace d3d
{
	using DirectX::XMStoreFloat2;
	using DirectX::XMStoreFloat3;
	using DirectX::XMStoreFloat4;
	using DirectX::XMLoadFloat2;
	using DirectX::XMLoadFloat3;
	using DirectX::XMLoadFloat4;

	extern "C++" namespace vec
	{
		inline XMFLOAT3 XMVectorToFloat3(const XMVECTOR& xmvVector) noexcept
		{
			XMFLOAT3 result{};

			XMStoreFloat3(&result, xmvVector);

			return result;
		}

		inline XMFLOAT3 XMVectorToFloat3(XMVECTOR&& xmvVector) noexcept
		{
			XMFLOAT3 result{};

			XMStoreFloat3(&result, std::move(xmvVector));

			return result;
		}

		inline float Angle(XMVECTOR&& lhs, XMVECTOR&& rhs) noexcept
		{
			return XMConvertToDegrees(XMVectorGetX(XMVector3AngleBetweenNormals(std::move(lhs), std::move(rhs))));
		}

		inline float Angle(const XMFLOAT3& lhs, const XMFLOAT3& rhs) noexcept
		{
			return Angle(XMLoadFloat3(&lhs), XMLoadFloat3(&rhs));
		}

		inline XMFLOAT3 TransformNormal(const XMFLOAT3& vector, const XMMATRIX& transform) noexcept
		{
			XMFLOAT3 result{};

			XMStoreFloat3(&result, XMVector3TransformNormal(XMLoadFloat3(&vector), transform));

			return result;
		}

		inline XMFLOAT3 TransformCoord(const XMFLOAT3& vector, const XMMATRIX& transform) noexcept
		{
			XMFLOAT3 result{};

			XMStoreFloat3(&result, XMVector3TransformCoord(XMLoadFloat3(&vector), transform));

			return result;
		}

		inline XMFLOAT3 TransformCoord(const XMFLOAT3& vector, const XMFLOAT4X4& transform) noexcept
		{
			XMFLOAT3 result{};

			XMStoreFloat3(&result
				, XMVector3TransformCoord(XMLoadFloat3(&vector), XMLoadFloat4x4(&transform)));

			return result;
		}
	}

	extern "C++" namespace mat
	{
		inline XMFLOAT4X4 Multiply(XMFLOAT4X4&& lhs, XMFLOAT4X4&& rhs) noexcept
		{
			XMFLOAT4X4 result{};

			const XMMATRIX mat1 = XMLoadFloat4x4(&lhs);
			const XMMATRIX mat2 = XMLoadFloat4x4(&rhs);

			XMStoreFloat4x4(&result, mat1 * mat2);

			return result;
		}

		inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& lhs, const XMFLOAT4X4& rhs) noexcept
		{
			XMFLOAT4X4 result{};

			XMStoreFloat4x4(&result, XMLoadFloat4x4(&lhs) * XMLoadFloat4x4(&rhs));

			return result;
		}

		inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& lhs, const XMMATRIX& rhs) noexcept
		{
			XMFLOAT4X4 result{};

			XMStoreFloat4x4(&result, XMLoadFloat4x4(&lhs) * rhs);

			return result;
		}

		inline XMFLOAT4X4 Multiply(XMFLOAT4X4&& lhs, const XMMATRIX& rhs) noexcept
		{
			XMFLOAT4X4 result{};

			XMStoreFloat4x4(&result, XMLoadFloat4x4(&lhs) * rhs);

			return result;
		}

		inline XMFLOAT4X4 Multiply(const XMMATRIX& lhs, const XMFLOAT4X4& rhs) noexcept
		{
			XMFLOAT4X4 result{};

			XMStoreFloat4x4(&result, lhs * XMLoadFloat4x4(&rhs));

			return result;
		}

		inline XMFLOAT4X4 Multiply(XMMATRIX&& lhs, const XMFLOAT4X4& rhs) noexcept
		{
			XMFLOAT4X4 result{};

			XMStoreFloat4x4(&result, std::move(lhs) * XMLoadFloat4x4(&rhs));

			return result;
		}

		inline XMFLOAT4X4 RotationYawPitchRoll(const float& pitch, const float& yaw, const float& roll) noexcept
		{
			XMFLOAT4X4 result{};

			const float rad_pitch = XMConvertToRadians(pitch);
			const float rad_yaw = XMConvertToRadians(yaw);
			const float rad_roll = XMConvertToRadians(roll);
			const XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYaw(rad_pitch, rad_yaw, rad_roll);
			XMStoreFloat4x4(&result, rot);

			return result;
		}

		inline XMFLOAT4X4 PerspectiveFovLH(const float& fov_y, const float& aspect, const float& znear, const float& zfar) noexcept
		{
			XMFLOAT4X4 result{};

			const XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(fov_y, aspect, znear, zfar);
			XMStoreFloat4x4(&result, mat);

			return result;
		}

		inline XMFLOAT4X4 LookAtLH(const XMFLOAT3& eye_pos, const XMFLOAT3& lookat, const XMFLOAT3& up) noexcept
		{
			XMFLOAT4X4 result{};

			const auto mat = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye_pos), XMLoadFloat3(&lookat), XMLoadFloat3(&up));

			XMStoreFloat4x4(&result, mat);

			return result;
		}

		inline XMFLOAT4X4 LookToLH(const XMFLOAT3& eye_pos, const XMFLOAT3& look_pos, const XMFLOAT3& up_dir) noexcept
		{
			XMFLOAT4X4 result{};

			const XMVECTOR eye = XMLoadFloat3(&eye_pos);
			const XMVECTOR look = XMLoadFloat3(&look_pos);
			const XMVECTOR up = XMLoadFloat3(&up_dir);
			XMStoreFloat4x4(&result, DirectX::XMMatrixLookToLH(eye, look, up));

			return result;
		}
	}

	extern "C++" namespace trig
	{
		inline bool Intersect(const XMFLOAT3& xmf3RayPosition, const XMFLOAT3& xmf3RayDirection, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, float& dist) noexcept
		{
			return TriangleTests::Intersects(XMLoadFloat3(&xmf3RayPosition)
				, XMLoadFloat3(&xmf3RayDirection)
				, XMLoadFloat3(&v0), XMLoadFloat3(&v1), XMLoadFloat3(&v2)
				, dist);
		}
	}

	extern "C++" namespace rect
	{
		inline XMFLOAT4 Normalize(const XMFLOAT4& xmf4Plane) noexcept
		{
			XMFLOAT4 result;
			XMStoreFloat4(&result, XMPlaneNormalize(XMLoadFloat4(&xmf4Plane)));
			return result;
		}
	}
}
