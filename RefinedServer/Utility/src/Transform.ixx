module;
#include <DirectXMath.h>

export module Utility.D3D.Transform;
import Utility;
export import Utility.D3D.Matrix;
export import Utility.D3D.Vector;

export namespace d3d
{
	struct XyzWrapper
	{
		float& x, & y, & z;
	};

	struct XyzwWrapper
	{
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

		XMFLOAT4X4 myMatrix{};
		XyzwWrapper myPosition = { myMatrix._41, myMatrix._42, myMatrix._43, myMatrix._44 };
		XyzwWrapper myRight = { myMatrix._11, myMatrix._12, myMatrix._13, myMatrix._14 };
		XyzwWrapper myUp = { myMatrix._21, myMatrix._22, myMatrix._23, myMatrix._24 };
		XyzwWrapper myLook = { myMatrix._31, myMatrix._32, myMatrix._33, myMatrix._34 };
	};
}
