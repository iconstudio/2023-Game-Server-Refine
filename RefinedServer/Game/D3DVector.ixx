module;
#include <DirectXMath.h>

export module Game.D3D.Vector;
import <utility>;
import <algorithm>;
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
	inline constexpr XMFLOAT2 Zero2 = {};
	inline constexpr XMFLOAT3 Zero = {};
	inline constexpr XMFLOAT2 NegativeOne2 = { -1.0f, -1.0f };
	inline constexpr XMFLOAT2 One2 = { 1.0f, 1.0f };
	inline constexpr XMFLOAT3 NegativeOne = { -1.0f, -1.0f, -1.0f };
	inline constexpr XMFLOAT3 One = { 1.0f, 1.0f, 1.0f };

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
	constexpr XMFLOAT2 XM_CALLCONV Min(const XMFLOAT2& lhs, const XMFLOAT2& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::min(lhs.x, rhs.x),
			std::min(lhs.y, rhs.y)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Min(XMFLOAT2&& lhs, const XMFLOAT2& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::min(static_cast<float&&>(lhs.x), rhs.x),
			std::min(static_cast<float&&>(lhs.y), rhs.y)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Min(const XMFLOAT2& lhs, XMFLOAT2&& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::min(lhs.x, static_cast<float&&>(rhs.x)),
			std::min(lhs.y, static_cast<float&&>(rhs.y))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Min(XMFLOAT2&& lhs, XMFLOAT2&& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::min(static_cast<float&&>(lhs.x), static_cast<float&&>(rhs.x)),
			std::min(static_cast<float&&>(lhs.y), static_cast<float&&>(rhs.y))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Max(const XMFLOAT2& lhs, const XMFLOAT2& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::max(lhs.x, rhs.x),
			std::max(lhs.y, rhs.y)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Max(XMFLOAT2&& lhs, const XMFLOAT2& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::max(static_cast<float&&>(lhs.x), rhs.x),
			std::max(static_cast<float&&>(lhs.y), rhs.y)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Max(const XMFLOAT2& lhs, XMFLOAT2&& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::max(lhs.x, static_cast<float&&>(rhs.x)),
			std::max(lhs.y, static_cast<float&&>(rhs.y))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT2 XM_CALLCONV Max(XMFLOAT2&& lhs, XMFLOAT2&& rhs) noexcept
	{
		return XMFLOAT2
		{
			std::max(static_cast<float&&>(lhs.x), static_cast<float&&>(rhs.x)),
			std::max(static_cast<float&&>(lhs.y), static_cast<float&&>(rhs.y))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Min(const XMFLOAT3& lhs, const XMFLOAT3& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::min(lhs.x, rhs.x),
			std::min(lhs.y, rhs.y),
			std::min(lhs.z, rhs.z)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Min(XMFLOAT3&& lhs, const XMFLOAT3& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::min(static_cast<float&&>(lhs.x), rhs.x),
			std::min(static_cast<float&&>(lhs.y), rhs.y),
			std::min(static_cast<float&&>(lhs.z), rhs.z)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Min(const XMFLOAT3& lhs, XMFLOAT3&& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::min(lhs.x, static_cast<float&&>(rhs.x)),
			std::min(lhs.y, static_cast<float&&>(rhs.y)),
			std::min(lhs.z, static_cast<float&&>(rhs.z))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Min(XMFLOAT3&& lhs, XMFLOAT3&& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::min(static_cast<float&&>(lhs.x), static_cast<float&&>(rhs.x)),
			std::min(static_cast<float&&>(lhs.y), static_cast<float&&>(rhs.y)),
			std::min(static_cast<float&&>(lhs.z), static_cast<float&&>(rhs.z))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Max(const XMFLOAT3& lhs, const XMFLOAT3& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::max(lhs.x, rhs.x),
			std::max(lhs.y, rhs.y),
			std::max(lhs.z, rhs.z)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Max(XMFLOAT3&& lhs, const XMFLOAT3& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::max(static_cast<float&&>(lhs.x), rhs.x),
			std::max(static_cast<float&&>(lhs.y), rhs.y),
			std::max(static_cast<float&&>(lhs.z), rhs.z)
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Max(const XMFLOAT3& lhs, XMFLOAT3&& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::max(lhs.x, static_cast<float&&>(rhs.x)),
			std::max(lhs.y, static_cast<float&&>(rhs.y)),
			std::max(lhs.z, static_cast<float&&>(rhs.z))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Max(XMFLOAT3&& lhs, XMFLOAT3&& rhs) noexcept
	{
		return XMFLOAT3
		{
			std::max(static_cast<float&&>(lhs.x), static_cast<float&&>(rhs.x)),
			std::max(static_cast<float&&>(lhs.y), static_cast<float&&>(rhs.y)),
			std::max(static_cast<float&&>(lhs.z), static_cast<float&&>(rhs.z))
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Clamp(const XMFLOAT3& vector, const XMFLOAT3& min, const XMFLOAT3& max) noexcept
	{
		if (util::is_constant_evaluated())
		{
			return XMFLOAT3
			{
				std::clamp(vector.x, min.x, max.x),
				std::clamp(vector.y, min.y, max.y),
				std::clamp(vector.z, min.z, max.z)
			};
		}
		else
		{
			XMFLOAT3 calc{};
			XMStoreFloat3(&calc, DirectX::XMVectorClamp(XMLoadFloat3(&vector), XMLoadFloat3(&min), XMLoadFloat3(&max)));

			return calc;
		};
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Clamp(XMFLOAT3&& vector, const XMFLOAT3& min, const XMFLOAT3& max) noexcept
	{
		if (util::is_constant_evaluated())
		{
			return XMFLOAT3
			{
				std::clamp(static_cast<float&&>(vector.x), min.x, max.x),
				std::clamp(static_cast<float&&>(vector.y), min.y, max.y),
				std::clamp(static_cast<float&&>(vector.z), min.z, max.z)
			};
		}
		else
		{
			XMFLOAT3 calc{};
			XMStoreFloat3(&calc, DirectX::XMVectorClamp(XMLoadFloat3(&vector), XMLoadFloat3(&min), XMLoadFloat3(&max)));

			return calc;
		};
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Dot(const XMFLOAT2& lhs, const XMFLOAT2& rhs) noexcept
	{
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Dot(const XMFLOAT3& lhs, const XMFLOAT3& rhs) noexcept
	{
		if (util::is_constant_evaluated())
		{
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}
		else
		{
			XMFLOAT3 calc{};
			XMStoreFloat3(&calc, XMVector3Dot(XMLoadFloat3(&lhs), XMLoadFloat3(&rhs)));

			return calc.x;
		}
	}

	[[nodiscard]]
	constexpr float XM_CALLCONV Dot(XMFLOAT3&& lhs, XMFLOAT3&& rhs) noexcept
	{
		if (util::is_constant_evaluated())
		{
			return static_cast<float&&>(lhs.x) * static_cast<float&&>(rhs.x)
				+ static_cast<float&&>(lhs.y) * static_cast<float&&>(rhs.y)
				+ static_cast<float&&>(lhs.z) * static_cast<float&&>(rhs.z);
		}
		else
		{
			XMFLOAT3 calc{};
			XMStoreFloat3(&calc, XMVector3Dot(XMLoadFloat3(&lhs), XMLoadFloat3(&rhs)));

			return calc.x;
		}
	}

	[[nodiscard]]
	constexpr XMFLOAT3 XM_CALLCONV Cross(const XMFLOAT3& lhs, const XMFLOAT3& rhs, const bool& normalize = true) noexcept
	{
		XMFLOAT3 result{};
		if (util::is_constant_evaluated())
		{
			result.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
			result.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
			result.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);
		}
		else
		{
			XMStoreFloat3(&result, XMVector3Cross(XMLoadFloat3(&lhs), XMLoadFloat3(&rhs)));
		}

		if (normalize)
		{
			return Normalize(result);
		}
		else
		{
			return result;
		}
	}

	[[nodiscard]]
	inline float XM_CALLCONV NormalAngle(const XMFLOAT3& lhs, const XMFLOAT3& rhs) noexcept
	{
		if (util::is_constant_evaluated())
		{
			const float dot = Dot(lhs, rhs);
			const XMFLOAT3 splat = { dot, dot, dot };
			const XMFLOAT3 bound = Clamp(splat, NegativeOne, One);

			XMVECTOR calc{};
			calc.m128_f32[0] = bound.x;
			calc.m128_f32[1] = bound.y;
			calc.m128_f32[2] = bound.z;
			calc.m128_f32[3] = 0.0f;

			// Now not constexpr
			XMVECTOR result = DirectX::XMVectorACos(calc);
			return result.m128_f32[0];
		}
		else
		{
			return DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(DirectX::XMLoadFloat3(&lhs), DirectX::XMLoadFloat3(&rhs)));
		}
	}

	[[nodiscard]]
	inline float XM_CALLCONV Angle(const XMFLOAT3& lhs, const XMFLOAT3& rhs) noexcept
	{
		return NormalAngle(Normalize(lhs), Normalize(rhs));
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
