module;
#include <DirectXMath.h>
#include <utility>

export module Utility.D3D.Vector;
import Utility;

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
	constexpr float XM_CALLCONV Length(const XMFLOAT2& vector) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float lf = util::sqr(vector.x) + util::sqr(vector.y);
			return util::sqrt(lf);
		}
		else
		{
			return Length(XMLoadFloat2(&vector));
		}
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Length(XMFLOAT2&& vector) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float lf = util::sqr(static_cast<float&&>(vector.x)) + util::sqr(static_cast<float&&>(vector.y));
			return util::sqrt(lf);
		}
		else
		{
			return Length(XMLoadFloat2(&vector));
		}
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Length(const XMFLOAT3& vector) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float lf = util::sqr(vector.x) + util::sqr(vector.y) + util::sqr(vector.z);
			return util::sqrt(lf);
		}
		else
		{
			return Length(XMLoadFloat3(&vector));
		}
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Length(XMFLOAT3&& vector) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float lf = util::sqr(static_cast<float&&>(vector.x)) + util::sqr(static_cast<float&&>(vector.y)) + util::sqr(static_cast<float&&>(vector.z));
			return util::sqrt(lf);
		}
		else
		{
			return Length(XMLoadFloat3(&vector));
		}
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Length(const XMVECTOR& vector) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float lf = util::sqr(vector.m128_f32[0]) + util::sqr(vector.m128_f32[1]) + util::sqr(vector.m128_f32[2]) + util::sqr(vector.m128_f32[3]);
			return util::sqrt(lf);
		}
		else
		{
			XMFLOAT3 result{};
			XMStoreFloat3(&result, XMVector3Length(vector));
			return(result.x);
		}
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Length(XMVECTOR&& vector) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float lf = util::sqr(static_cast<float&&>(vector.m128_f32[0]))
				+ util::sqr(static_cast<float&&>(vector.m128_f32[1]))
				+ util::sqr(static_cast<float&&>(vector.m128_f32[2]))
				+ util::sqr(static_cast<float&&>(vector.m128_f32[3]));
			return util::sqrt(lf);
		}
		else
		{
			XMFLOAT3 result{};
			XMStoreFloat3(&result, XMVector3Length(vector));
			return(result.x);
		}
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Normalize(const XMFLOAT2& vector) noexcept
	{
		XMFLOAT2 result{};
		if (util::is_constant_evaluated())
		{
			const float ln = Length(vector);

			result.x = vector.x / ln;
			result.y = vector.y / ln;
		}
		else
		{
			DirectX::XMStoreFloat2(&result, DirectX::XMVector2Normalize(XMLoadFloat2(&vector)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Normalize(XMFLOAT2&& vector) noexcept
	{
		XMFLOAT2 result{};
		if (util::is_constant_evaluated())
		{
			const float ln = Length(vector);

			result.x = static_cast<float&&>(vector.x) / ln;
			result.y = static_cast<float&&>(vector.y) / ln;
		}
		else
		{
			DirectX::XMStoreFloat2(&result, DirectX::XMVector2Normalize(XMLoadFloat2(&vector)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Normalize(const XMFLOAT3& vector) noexcept
	{
		XMFLOAT3 result{};
		if (util::is_constant_evaluated())
		{
			const float ln = Length(vector);

			result.x = vector.x / ln;
			result.y = vector.y / ln;
			result.z = vector.z / ln;
		}
		else
		{
			DirectX::XMStoreFloat3(&result, DirectX::XMVector3Normalize(XMLoadFloat3(&vector)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Normalize(XMFLOAT3&& vector) noexcept
	{
		XMFLOAT3 result{};
		if (util::is_constant_evaluated())
		{
			const float ln = Length(vector);

			result.x = static_cast<float&&>(vector.x) / ln;
			result.y = static_cast<float&&>(vector.y) / ln;
			result.z = static_cast<float&&>(vector.z) / ln;
		}
		else
		{
			DirectX::XMStoreFloat3(&result, DirectX::XMVector3Normalize(XMLoadFloat3(&vector)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMVECTOR XM_CALLCONV Zero() noexcept
	{
		if (util::is_constant_evaluated())
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
		if (util::is_constant_evaluated())
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
		if (util::is_constant_evaluated())
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
		if (util::is_constant_evaluated())
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
		if (util::is_constant_evaluated())
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
		if (util::is_constant_evaluated())
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
		if (util::is_constant_evaluated())
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
