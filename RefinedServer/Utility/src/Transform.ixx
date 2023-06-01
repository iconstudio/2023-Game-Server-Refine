module;
#include <DirectXMath.h>

export module Utility.D3D.Transform;
import Utility;
export import Utility.D3D.Matrix;
export import Utility.D3D.Vector;

export namespace d3d
{
	struct XyzwWrapper
	{
		constexpr XyzwWrapper(float& dx, float& dy, float& dz, float& dw) noexcept
			: x(dx), y(dy), z(dz), w(dw)
		{}

		constexpr ~XyzwWrapper() noexcept = default;

		constexpr XyzwWrapper& operator=(const XyzwWrapper& other) noexcept
		{
			return *this;
		}

		constexpr XyzwWrapper& operator=(XyzwWrapper&& other) noexcept
		{
			return *this;
		}

		float& x, & y, & z, & w;
	};

	using ::operator+;
	using ::operator+=;
	using ::operator-;
	using ::operator-=;
	using ::operator*;
	using ::operator*=;

	class Transform
	{
	public:
		constexpr Transform() noexcept = default;
		constexpr ~Transform() noexcept = default;

		constexpr Transform(const Transform& other) noexcept
			: myMatrix(other.myMatrix)
		{}

		constexpr Transform(Transform&& other) noexcept
			: myMatrix(static_cast<XMFLOAT4X4&&>(other.myMatrix))
		{}

		constexpr Transform& operator=(const Transform& other) noexcept
		{
			myMatrix = other.myMatrix;
			return *this;
		}

		constexpr Transform& operator=(Transform&& other) noexcept
		{
			myMatrix = static_cast<XMFLOAT4X4&&>(other.myMatrix);
			return *this;
		}

		explicit constexpr Transform(const XMFLOAT4X4& matrix) noexcept
			: myMatrix(matrix)
		{}

		explicit constexpr Transform(XMFLOAT4X4&& matrix) noexcept
			: myMatrix(static_cast<XMFLOAT4X4&&>(matrix))
		{}

		constexpr void Jump(const float& x, const float& y, const float& z) noexcept
		{
			myPosition.x = x;
			myPosition.y = y;
			myPosition.z = z;
		}

		constexpr void Jump(const XMFLOAT3& xyz) noexcept
		{
			myPosition.x = xyz.x;
			myPosition.y = xyz.y;
			myPosition.z = xyz.z;
		}

		constexpr void Jump(XMFLOAT3&& xyz) noexcept
		{
			myPosition.x = static_cast<float&&>(xyz.x);
			myPosition.y = static_cast<float&&>(xyz.y);
			myPosition.z = static_cast<float&&>(xyz.z);
		}

		constexpr void Move(const float& x, const float& y, const float& z) noexcept
		{
			myPosition.x += x;
			myPosition.y += y;
			myPosition.z += z;
		}

		constexpr void Move(const XMFLOAT3& xyz) noexcept
		{
			myPosition.x += xyz.x;
			myPosition.y += xyz.y;
			myPosition.z += xyz.z;
		}

		constexpr void Move(XMFLOAT3&& xyz) noexcept
		{
			myPosition.x += static_cast<float&&>(xyz.x);
			myPosition.y += static_cast<float&&>(xyz.y);
			myPosition.z += static_cast<float&&>(xyz.z);
		}

		constexpr void Forward(const float& distance) noexcept
		{
			myPosition.z += distance;
		}

		constexpr void Forward(float&& distance) noexcept
		{
			myPosition.z += static_cast<float&&>(distance);
		}

		constexpr void Forward(const XMFLOAT3& xyz) noexcept
		{
			myPosition.z += xyz.z;
		}

		constexpr void Forward(XMFLOAT3&& xyz) noexcept
		{
			myPosition.z += static_cast<float&&>(xyz.z);
		}

		constexpr void Backward(const float& distance) noexcept
		{
			myPosition.z -= distance;
		}

		constexpr void Backward(float&& distance) noexcept
		{
			myPosition.z -= static_cast<float&&>(distance);
		}

		constexpr void Backward(const XMFLOAT3& xyz) noexcept
		{
			myPosition.z -= xyz.z;
		}

		constexpr void Backward(XMFLOAT3&& xyz) noexcept
		{
			myPosition.z -= static_cast<float&&>(xyz.z);
		}

		constexpr void StrafeLeft(const float& distance) noexcept
		{
			myPosition.x -= distance;
		}

		constexpr void StrafeLeft(float&& distance) noexcept
		{
			myPosition.x -= static_cast<float&&>(distance);
		}

		constexpr void StrafeLeft(const XMFLOAT3& xyz) noexcept
		{
			myPosition.x -= xyz.x;
		}

		constexpr void StrafeLeft(XMFLOAT3&& xyz) noexcept
		{
			myPosition.x -= static_cast<float&&>(xyz.x);
		}

		constexpr void StrafeRight(const float& distance) noexcept
		{
			myPosition.x += distance;
		}

		constexpr void StrafeRight(float&& distance) noexcept
		{
			myPosition.x += static_cast<float&&>(distance);
		}

		constexpr void StrafeRight(const XMFLOAT3& xyz) noexcept
		{
			myPosition.x += xyz.x;
		}

		constexpr void StrafeRight(XMFLOAT3&& xyz) noexcept
		{
			myPosition.x += static_cast<float&&>(xyz.x);
		}

		constexpr void Upward(const float& distance) noexcept
		{
			myPosition.y += distance;
		}

		constexpr void Upward(float&& distance) noexcept
		{
			myPosition.y += static_cast<float&&>(distance);
		}

		constexpr void Upward(const XMFLOAT3& xyz) noexcept
		{
			myPosition.y += xyz.y;
		}

		constexpr void Upward(XMFLOAT3&& xyz) noexcept
		{
			myPosition.y += static_cast<float&&>(xyz.y);
		}

		constexpr void Downward(const float& distance) noexcept
		{
			myPosition.y -= distance;
		}

		constexpr void Downward(float&& distance) noexcept
		{
			myPosition.y -= static_cast<float&&>(distance);
		}

		constexpr void Downward(const XMFLOAT3& xyz) noexcept
		{
			myPosition.y -= xyz.y;
		}

		constexpr void Downward(XMFLOAT3&& xyz) noexcept
		{
			myPosition.y -= static_cast<float&&>(xyz.y);
		}

		[[nodiscard]]
		constexpr XyzwWrapper& GetPosition() noexcept
		{
			return myPosition;
		}

		[[nodiscard]]
		constexpr const XyzwWrapper& GetPosition() const noexcept
		{
			return myPosition;
		}

		[[nodiscard]]
		constexpr XyzwWrapper& GetRight() noexcept
		{
			return myRight;
		}

		[[nodiscard]]
		constexpr const XyzwWrapper& GetRight() const noexcept
		{
			return myRight;
		}

		[[nodiscard]]
		constexpr XyzwWrapper& GetUp() noexcept
		{
			return myUp;
		}

		[[nodiscard]]
		constexpr const XyzwWrapper& GetUp() const noexcept
		{
			return myUp;
		}

		[[nodiscard]]
		constexpr XyzwWrapper& GetLook() noexcept
		{
			return myLook;
		}

		[[nodiscard]]
		constexpr const XyzwWrapper& GetLook() const noexcept
		{
			return myLook;
		}

		[[nodiscard]]
		inline friend constexpr Transform operator+(const Transform& lhs, const Transform& rhs) noexcept
		{
			return Transform{ lhs.myMatrix + rhs.myMatrix };
		}

		inline friend constexpr Transform& operator+=(Transform& lhs, const Transform& rhs) noexcept
		{
			lhs.myMatrix += rhs.myMatrix;

			return lhs;
		}

		[[nodiscard]]
		inline friend constexpr Transform operator-(const Transform& lhs, const Transform& rhs) noexcept
		{
			return Transform{ lhs.myMatrix - rhs.myMatrix };
		}

		inline friend constexpr Transform& operator-=(Transform& lhs, const Transform& rhs) noexcept
		{
			lhs.myMatrix -= rhs.myMatrix;

			return lhs;
		}

		inline friend constexpr Transform operator*(const Transform& lhs, const Transform& rhs) noexcept
		{
			return Transform{ lhs.myMatrix * rhs.myMatrix };
		}

		[[nodiscard]]
		inline friend constexpr Transform& operator*=(Transform& lhs, const Transform& rhs) noexcept
		{
			lhs.myMatrix *= rhs.myMatrix;

			return lhs;
		}

		/// <summary>
		/// Local Transform
		/// </summary>
		XMFLOAT4X4 myMatrix{};

		XyzwWrapper myPosition = { myMatrix._41, myMatrix._42, myMatrix._43, myMatrix._44 };
		XyzwWrapper myRight = { myMatrix._11, myMatrix._12, myMatrix._13, myMatrix._14 };
		XyzwWrapper myUp = { myMatrix._21, myMatrix._22, myMatrix._23, myMatrix._24 };
		XyzwWrapper myLook = { myMatrix._31, myMatrix._32, myMatrix._33, myMatrix._34 };
	};
}
