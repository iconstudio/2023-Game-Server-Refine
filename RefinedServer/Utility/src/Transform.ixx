module;
#include <DirectXMath.h>

export module Utility.D3D.Transform;
import Utility;
import Utility.D3D.Matrix;
import Utility.D3D.Vector;

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

	class Transform
	{
	public:
		constexpr Transform() noexcept = default;
		constexpr Transform(const Transform& other) noexcept = default;
		constexpr Transform(Transform&& other) noexcept = default;
		constexpr Transform& operator=(const Transform& other) noexcept = default;
		constexpr Transform& operator=(Transfor& m& other) noexcept = default;
		constexpr ~Transform() noexcept = default;

		[[nodiscard]]
		inline friend constexpr Transform& operator+(Transform& lhs, const Transform& rhs) noexcept
		{
			if (util::is_constant_evaluated())
			{
				lhs.myMatrix + rhs.myMatrix;
			}
			else
			{

			}
		}

		mat::XMFLOAT4X4 myMatrix{};
		XyzwWrapper myPosition = { myMatrix._41, myMatrix._42, myMatrix._43, myMatrix._44 };
		XyzwWrapper myRight = { myMatrix._11, myMatrix._12, myMatrix._13, myMatrix._14 };
		XyzwWrapper myUp = { myMatrix._21, myMatrix._22, myMatrix._23, myMatrix._24 };
		XyzwWrapper myLook = { myMatrix._31, myMatrix._32, myMatrix._33, myMatrix._34 };
	};
}
