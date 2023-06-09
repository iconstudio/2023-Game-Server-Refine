module;
#include <DirectXMath.h>

export module Game.D3D.Transform;
import Utility;
export import Game.D3D.Matrix;
export import Game.D3D.Vector;

export namespace d3d
{
	struct XyzwWrapper
	{
		constexpr XyzwWrapper(float& dx, float& dy, float& dz, float& dw) noexcept
			: x(dx), y(dy), z(dz), w(dw)
		{}

		constexpr ~XyzwWrapper() noexcept = default;

		constexpr XyzwWrapper& operator=([[maybe_unused]] const XyzwWrapper&) noexcept
		{
			return *this;
		}

		constexpr XyzwWrapper& operator=([[maybe_unused]] XyzwWrapper&&) noexcept
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

		constexpr void Jump(const float& dx, const float& dy, const float& dz) noexcept
		{
			x = dx;
			y = dy;
			z = dz;
		}

		constexpr void Jump(const XMFLOAT3& xyz) noexcept
		{
			x = xyz.x;
			y = xyz.y;
			z = xyz.z;
		}

		constexpr void Jump(XMFLOAT3&& xyz) noexcept
		{
			x = static_cast<float&&>(xyz.x);
			y = static_cast<float&&>(xyz.y);
			z = static_cast<float&&>(xyz.z);
		}

		constexpr void Move(const float& dx, const float& dy, const float& dz) noexcept
		{
			x += dx;
			y += dy;
			z += dz;
		}

		constexpr void Move(const XMFLOAT3& xyz) noexcept
		{
			x += xyz.x;
			y += xyz.y;
			z += xyz.z;
		}

		constexpr void Move(XMFLOAT3&& xyz) noexcept
		{
			x += static_cast<float&&>(xyz.x);
			y += static_cast<float&&>(xyz.y);
			z += static_cast<float&&>(xyz.z);
		}

		constexpr void Forward(const float& distance) noexcept
		{
			z += distance;
		}

		constexpr void Forward(float&& distance) noexcept
		{
			z += static_cast<float&&>(distance);
		}

		constexpr void Forward(const XMFLOAT3& xyz) noexcept
		{
			z += xyz.z;
		}

		constexpr void Forward(XMFLOAT3&& xyz) noexcept
		{
			z += static_cast<float&&>(xyz.z);
		}

		constexpr void Backward(const float& distance) noexcept
		{
			z -= distance;
		}

		constexpr void Backward(float&& distance) noexcept
		{
			z -= static_cast<float&&>(distance);
		}

		constexpr void Backward(const XMFLOAT3& xyz) noexcept
		{
			z -= xyz.z;
		}

		constexpr void Backward(XMFLOAT3&& xyz) noexcept
		{
			z -= static_cast<float&&>(xyz.z);
		}

		constexpr void StrafeLeft(const float& distance) noexcept
		{
			x -= distance;
		}

		constexpr void StrafeLeft(float&& distance) noexcept
		{
			x -= static_cast<float&&>(distance);
		}

		constexpr void StrafeLeft(const XMFLOAT3& xyz) noexcept
		{
			x -= xyz.x;
		}

		constexpr void StrafeLeft(XMFLOAT3&& xyz) noexcept
		{
			x -= static_cast<float&&>(xyz.x);
		}

		constexpr void StrafeRight(const float& distance) noexcept
		{
			x += distance;
		}

		constexpr void StrafeRight(float&& distance) noexcept
		{
			x += static_cast<float&&>(distance);
		}

		constexpr void StrafeRight(const XMFLOAT3& xyz) noexcept
		{
			x += xyz.x;
		}

		constexpr void StrafeRight(XMFLOAT3&& xyz) noexcept
		{
			x += static_cast<float&&>(xyz.x);
		}

		constexpr void Upward(const float& distance) noexcept
		{
			y += distance;
		}

		constexpr void Upward(float&& distance) noexcept
		{
			y += static_cast<float&&>(distance);
		}

		constexpr void Upward(const XMFLOAT3& xyz) noexcept
		{
			y += xyz.y;
		}

		constexpr void Upward(XMFLOAT3&& xyz) noexcept
		{
			y += static_cast<float&&>(xyz.y);
		}

		constexpr void Downward(const float& distance) noexcept
		{
			y -= distance;
		}

		constexpr void Downward(float&& distance) noexcept
		{
			y -= static_cast<float&&>(distance);
		}

		constexpr void Downward(const XMFLOAT3& xyz) noexcept
		{
			y -= xyz.y;
		}

		constexpr void Downward(XMFLOAT3&& xyz) noexcept
		{
			y -= static_cast<float&&>(xyz.y);
		}

		inline void Rotation(const XMFLOAT3& axis, const float& rot) noexcept
		{
			XMFLOAT3 position = GetPosition();
			myMatrix = mat::RotationAxis(axis, rot);
			Jump(position);
		}

		inline void Rotation(XMFLOAT3&& axis, const float& rot) noexcept
		{
			XMFLOAT3 position = GetPosition();
			myMatrix = mat::RotationAxis(static_cast<XMFLOAT3&&>(axis), rot);
			Jump(position);
		}

		inline void RotateAxis(const XMFLOAT3& axis, const float& rot) noexcept
		{
			myMatrix *= mat::RotationAxis(axis, rot);
		}

		inline void RotateAxis(XMFLOAT3&& axis, const float& rot) noexcept
		{
			myMatrix *= mat::RotationAxis(static_cast<XMFLOAT3&&>(axis), rot);
		}

		[[nodiscard]]
		constexpr XMFLOAT3 GetPosition() const noexcept
		{
			return XMFLOAT3{ x, y, z };
		}

		[[nodiscard]]
		constexpr XMFLOAT4X4 GetRotation() const noexcept
		{
			XMFLOAT4X4 result{ myMatrix };
			result._41 = 0;
			result._42 = 0;
			result._43 = 0;
			//result._44 = 1;

			return result;
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
		float& x = myMatrix._41, & y = myMatrix._42, & z = myMatrix._43;

		XyzwWrapper myRight = { myMatrix._11, myMatrix._12, myMatrix._13, myMatrix._14 };
		XyzwWrapper myUp = { myMatrix._21, myMatrix._22, myMatrix._23, myMatrix._24 };
		XyzwWrapper myLook = { myMatrix._31, myMatrix._32, myMatrix._33, myMatrix._34 };
	};
}
