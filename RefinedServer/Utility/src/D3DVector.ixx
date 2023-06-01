module;
#include <DirectXMath.h>
#include <utility>

export module Utility.D3D.Vector;

export namespace d3d
{
	using DirectX::XMVECTOR;
	using DirectX::XMMATRIX;
	using DirectX::XMINT2;
	using DirectX::XMINT3;
	using DirectX::XMINT4;
	using DirectX::XMUINT2;
	using DirectX::XMUINT3;
	using DirectX::XMUINT4;
	using DirectX::XMFLOAT2;
	using DirectX::XMFLOAT3;
	using DirectX::XMFLOAT4;

	using DirectX::operator+;
	using DirectX::operator+=;
	using DirectX::operator-;
	using DirectX::operator-=;
	using DirectX::operator*;
	using DirectX::operator*=;
	using DirectX::operator/;
	using DirectX::operator/=;
}

export namespace d3d::vec
{
	using DirectX::XMVectorSet;
	using DirectX::XMVectorReplicate;

	using DirectX::XMVector3Normalize;
	using DirectX::XMVector3NormalizeEst;

	using DirectX::XMVector3AngleBetweenNormals;
	using DirectX::XMVector3AngleBetweenNormalsEst;
	using DirectX::XMVector3AngleBetweenVectors;
	using DirectX::XMVector3Length;
	using DirectX::XMVector3LinePointDistance;

	using DirectX::XMVector3Cross;
	using DirectX::XMVector3Dot;

	using DirectX::XMVector3Reflect;
	using DirectX::XMVector3Refract;
	using DirectX::XMVector3Rotate;
	using DirectX::XMVector3InverseRotate;

	using DirectX::XMVector3Project;
	using DirectX::XMVector3Orthogonal;

	using DirectX::XMVector3Transform;
	using DirectX::XMVector3TransformNormal;
	using DirectX::XMVector3TransformCoord;

	using DirectX::XMVector3Equal;
	using DirectX::XMVector3Less;
	using DirectX::XMVector3Greater;
	using DirectX::XMVector3LessOrEqual;
	using DirectX::XMVector3GreaterOrEqual;
	using DirectX::XMVector3NotEqual;
	using DirectX::XMVector3NearEqual;

	using DirectX::XMVector3IsInfinite;
	using DirectX::XMVector3IsNaN;
	using DirectX::XMVector3InBounds;

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV Zero() noexcept
	{
		if (std::is_constant_evaluated())
		{
			return { 0.0f, 0.0f, 0.0f, 0.0f };
		}
		else
		{
			return DirectX::XMVectorZero();
		}
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV One() noexcept
	{
		if (std::is_constant_evaluated())
		{
			return { 1.0f, 1.0f, 1.0f, 1.0f };
		}
		else
		{
			return DirectX::XMVectorSplatOne();
		}
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV MergeXY(const XMVECTOR& lhs, const XMVECTOR& rhs) noexcept
	{
		if (std::is_constant_evaluated())
		{
			return
			{
				lhs.m128_f32[0],
				rhs.m128_f32[0],
				lhs.m128_f32[1],
				rhs.m128_f32[1],
			};
		}
		else
		{
			return DirectX::XMVectorMergeXY(lhs, rhs);
		}
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV MergeZW(const XMVECTOR& lhs, const XMVECTOR& rhs) noexcept
	{
		if (std::is_constant_evaluated())
		{
			return
			{
				lhs.m128_f32[2],
				rhs.m128_f32[2],
				lhs.m128_f32[3],
				rhs.m128_f32[3],
			};
		}
		else
		{
			return DirectX::XMVectorMergeZW(lhs, rhs);
		}
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV SplatX(const XMVECTOR& vector) noexcept
	{
		if (std::is_constant_evaluated())
		{
			DirectX::XMVECTORF32 result{};
			result.f[0] = result.f[1] = result.f[2] = result.f[3] = vector.m128_f32[0];
			return result.v;
		}
		else
		{
			return DirectX::XMVectorSplatX(vector);
		}
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV SplatY(const XMVECTOR& vector) noexcept
	{
		if (std::is_constant_evaluated())
		{
			DirectX::XMVECTORF32 result{};
			result.f[0] = result.f[1] = result.f[2] = result.f[3] = vector.m128_f32[1];
			return result.v;
		}
		else
		{
			return DirectX::XMVectorSplatY(vector);
		}
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV SplatZ(const XMVECTOR& vector) noexcept
	{
		if (std::is_constant_evaluated())
		{
			DirectX::XMVECTORF32 result{};
			result.f[0] = result.f[1] = result.f[2] = result.f[3] = vector.m128_f32[2];
			return result.v;
		}
		else
		{
			return DirectX::XMVectorSplatZ(vector);
		}
	}
}
