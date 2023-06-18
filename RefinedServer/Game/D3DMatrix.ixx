module;
#include <DirectXMath.h>

export module Game.D3D.Matrix;
import Utility;
import Game.D3D.Vector;

export
[[nodiscard]]
constexpr DirectX::XMFLOAT3X3 operator+(const DirectX::XMFLOAT3X3& lhs, const DirectX::XMFLOAT3X3& rhs) noexcept
{
	DirectX::XMFLOAT3X3 result{};
	if (util::is_constant_evaluated())
	{
		result._11 = lhs._11 + rhs._11;
		result._21 = lhs._21 + rhs._21;
		result._31 = lhs._31 + rhs._31;
		result._12 = lhs._12 + rhs._12;
		result._22 = lhs._22 + rhs._22;
		result._32 = lhs._32 + rhs._32;
		result._13 = lhs._13 + rhs._13;
		result._23 = lhs._23 + rhs._23;
		result._33 = lhs._33 + rhs._33;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat3x3(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat3x3(&rhs);

		XMStoreFloat3x3(&result, lmt + rmt);
	}

	return result;
}

export constexpr DirectX::XMFLOAT3X3& operator+=(DirectX::XMFLOAT3X3& lhs, const DirectX::XMFLOAT3X3& rhs) noexcept
{
	if (util::is_constant_evaluated())
	{
		lhs = lhs + rhs;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat3x3(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat3x3(&rhs);

		XMStoreFloat3x3(&lhs, lmt + rmt);
	}

	return lhs;
}

export
[[nodiscard]]
constexpr DirectX::XMFLOAT3X3 operator-(const DirectX::XMFLOAT3X3& lhs, const DirectX::XMFLOAT3X3& rhs) noexcept
{
	DirectX::XMFLOAT3X3 result{};
	if (util::is_constant_evaluated())
	{
		result._11 = lhs._11 - rhs._11;
		result._21 = lhs._21 - rhs._21;
		result._31 = lhs._31 - rhs._31;
		result._12 = lhs._12 - rhs._12;
		result._22 = lhs._22 - rhs._22;
		result._32 = lhs._32 - rhs._32;
		result._13 = lhs._13 - rhs._13;
		result._23 = lhs._23 - rhs._23;
		result._33 = lhs._33 - rhs._33;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat3x3(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat3x3(&rhs);

		XMStoreFloat3x3(&result, lmt - rmt);
	}

	return result;
}

export constexpr DirectX::XMFLOAT3X3& operator-=(DirectX::XMFLOAT3X3& lhs, const DirectX::XMFLOAT3X3& rhs) noexcept
{
	if (util::is_constant_evaluated())
	{
		lhs = lhs - rhs;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat3x3(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat3x3(&rhs);

		XMStoreFloat3x3(&lhs, lmt - rmt);
	}

	return lhs;
}

export
[[nodiscard]]
constexpr DirectX::XMFLOAT3X3 operator*(const DirectX::XMFLOAT3X3& lhs, const DirectX::XMFLOAT3X3& rhs) noexcept
{
	DirectX::XMFLOAT3X3 result{};
	if (util::is_constant_evaluated())
	{
		// Cache the invariants in registers
		float x = lhs.m[0][0];
		float y = lhs.m[0][1];
		float z = lhs.m[0][2];

		// Perform the operation on the first row
		result.m[0][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z);
		result.m[0][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z);
		result.m[0][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z);

		// Repeat for all the other rows
		x = lhs.m[1][0];
		y = lhs.m[1][1];
		z = lhs.m[1][2];

		result.m[1][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z);
		result.m[1][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z);
		result.m[1][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z);

		x = lhs.m[2][0];
		y = lhs.m[2][1];
		z = lhs.m[2][2];

		result.m[2][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z);
		result.m[2][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z);
		result.m[2][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z);
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat3x3(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat3x3(&rhs);

		XMStoreFloat3x3(&result, lmt * rmt);
	}

	return result;
}

export constexpr DirectX::XMFLOAT3X3& operator*=(DirectX::XMFLOAT3X3& lhs, const DirectX::XMFLOAT3X3& rhs) noexcept
{
	if (util::is_constant_evaluated())
	{
		lhs = lhs * rhs;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat3x3(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat3x3(&rhs);

		XMStoreFloat3x3(&lhs, lmt * rmt);
	}

	return lhs;
}

export
[[nodiscard]]
constexpr DirectX::XMFLOAT4X4 operator+(const DirectX::XMFLOAT4X4& lhs, const DirectX::XMFLOAT4X4& rhs) noexcept
{
	DirectX::XMFLOAT4X4 result{};
	if (util::is_constant_evaluated())
	{
		result._11 = lhs._11 + rhs._11;
		result._21 = lhs._21 + rhs._21;
		result._31 = lhs._31 + rhs._31;
		result._41 = lhs._41 + rhs._41;

		result._12 = lhs._12 + rhs._12;
		result._22 = lhs._22 + rhs._22;
		result._32 = lhs._32 + rhs._32;
		result._42 = lhs._42 + rhs._42;

		result._13 = lhs._13 + rhs._13;
		result._23 = lhs._23 + rhs._23;
		result._33 = lhs._33 + rhs._33;
		result._43 = lhs._43 + rhs._43;

		result._14 = lhs._14 + rhs._14;
		result._24 = lhs._24 + rhs._24;
		result._34 = lhs._34 + rhs._34;
		result._44 = lhs._44 + rhs._44;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat4x4(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat4x4(&rhs);

		XMStoreFloat4x4(&result, lmt + rmt);
	}

	return result;
}

export constexpr DirectX::XMFLOAT4X4& operator+=(DirectX::XMFLOAT4X4& lhs, const DirectX::XMFLOAT4X4& rhs) noexcept
{
	if (util::is_constant_evaluated())
	{
		lhs = lhs + rhs;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat4x4(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat4x4(&rhs);

		XMStoreFloat4x4(&lhs, lmt + rmt);
	}

	return lhs;
}

export
[[nodiscard]]
constexpr DirectX::XMFLOAT4X4 operator-(const DirectX::XMFLOAT4X4& lhs, const DirectX::XMFLOAT4X4& rhs) noexcept
{
	DirectX::XMFLOAT4X4 result{};
	if (util::is_constant_evaluated())
	{
		result._11 = lhs._11 - rhs._11;
		result._21 = lhs._21 - rhs._21;
		result._31 = lhs._31 - rhs._31;
		result._41 = lhs._41 - rhs._41;

		result._12 = lhs._12 - rhs._12;
		result._22 = lhs._22 - rhs._22;
		result._32 = lhs._32 - rhs._32;
		result._42 = lhs._42 - rhs._42;

		result._13 = lhs._13 - rhs._13;
		result._23 = lhs._23 - rhs._23;
		result._33 = lhs._33 - rhs._33;
		result._43 = lhs._43 - rhs._43;

		result._14 = lhs._14 - rhs._14;
		result._24 = lhs._24 - rhs._24;
		result._34 = lhs._34 - rhs._34;
		result._44 = lhs._44 - rhs._44;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat4x4(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat4x4(&rhs);

		XMStoreFloat4x4(&result, lmt - rmt);
	}

	return result;
}

export constexpr DirectX::XMFLOAT4X4& operator-=(DirectX::XMFLOAT4X4& lhs, const DirectX::XMFLOAT4X4& rhs) noexcept
{
	if (util::is_constant_evaluated())
	{
		lhs = lhs - rhs;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat4x4(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat4x4(&rhs);

		XMStoreFloat4x4(&lhs, lmt - rmt);
	}

	return lhs;
}

export
[[nodiscard]]
constexpr DirectX::XMFLOAT4X4 operator*(const DirectX::XMFLOAT4X4& lhs, const DirectX::XMFLOAT4X4& rhs) noexcept
{
	DirectX::XMFLOAT4X4 result{};
	if (util::is_constant_evaluated())
	{
		// Cache the invariants in registers
		float x = lhs.m[0][0];
		float y = lhs.m[0][1];
		float z = lhs.m[0][2];
		float w = lhs.m[0][3];

		// Perform the operation on the first row
		result.m[0][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
		result.m[0][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
		result.m[0][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
		result.m[0][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);

		// Repeat for all the other rows
		x = lhs.m[1][0];
		y = lhs.m[1][1];
		z = lhs.m[1][2];
		w = lhs.m[1][3];

		result.m[1][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
		result.m[1][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
		result.m[1][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
		result.m[1][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);
		x = lhs.m[2][0];
		y = lhs.m[2][1];
		z = lhs.m[2][2];
		w = lhs.m[2][3];

		result.m[2][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
		result.m[2][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
		result.m[2][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
		result.m[2][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);
		x = lhs.m[3][0];
		y = lhs.m[3][1];
		z = lhs.m[3][2];
		w = lhs.m[3][3];

		result.m[3][0] = (rhs.m[0][0] * x) + (rhs.m[1][0] * y) + (rhs.m[2][0] * z) + (rhs.m[3][0] * w);
		result.m[3][1] = (rhs.m[0][1] * x) + (rhs.m[1][1] * y) + (rhs.m[2][1] * z) + (rhs.m[3][1] * w);
		result.m[3][2] = (rhs.m[0][2] * x) + (rhs.m[1][2] * y) + (rhs.m[2][2] * z) + (rhs.m[3][2] * w);
		result.m[3][3] = (rhs.m[0][3] * x) + (rhs.m[1][3] * y) + (rhs.m[2][3] * z) + (rhs.m[3][3] * w);
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat4x4(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat4x4(&rhs);

		XMStoreFloat4x4(&result, lmt * rmt);
	}

	return result;
}

export constexpr DirectX::XMFLOAT4X4& operator*=(DirectX::XMFLOAT4X4& lhs, const DirectX::XMFLOAT4X4& rhs) noexcept
{
	if (util::is_constant_evaluated())
	{
		lhs = lhs * rhs;
	}
	else
	{
		const DirectX::XMMATRIX lmt = XMLoadFloat4x4(&lhs);
		const DirectX::XMMATRIX rmt = XMLoadFloat4x4(&rhs);

		XMStoreFloat4x4(&lhs, lmt * rmt);
	}

	return lhs;
}

export namespace d3d
{
	using DirectX::XMMATRIX;
	using DirectX::XMFLOAT3X3;
	using DirectX::XMFLOAT3X4;
	using DirectX::XMFLOAT4X3;
	using DirectX::XMFLOAT4X4;
	using DirectX::XMFLOAT3X4A;
	using DirectX::XMFLOAT4X3A;
	using DirectX::XMFLOAT4X4A;

	using DirectX::XMLoadFloat3x3;
	using DirectX::XMStoreFloat3x3;
	using DirectX::XMLoadFloat3x4;
	using DirectX::XMStoreFloat3x4;
	using DirectX::XMLoadFloat3x4A;
	using DirectX::XMStoreFloat3x4A;
	using DirectX::XMLoadFloat4x3;
	using DirectX::XMStoreFloat4x3;
	using DirectX::XMLoadFloat4x3A;
	using DirectX::XMStoreFloat4x3A;
	using DirectX::XMLoadFloat4x4;
	using DirectX::XMStoreFloat4x4;
	using DirectX::XMLoadFloat4x4A;
	using DirectX::XMStoreFloat4x4A;

	using DirectX::operator+;
	using DirectX::operator+=;
	using DirectX::operator-;
	using DirectX::operator-=;
	using DirectX::operator*;
	using DirectX::operator*=;
	using DirectX::operator/;
	using DirectX::operator/=;
}

export namespace d3d::mat
{
	[[nodiscard]]
	constexpr XMMATRIX XM_CALLCONV Identity() noexcept
	{
		XMMATRIX result{};
		result.r[0] = DirectX::g_XMIdentityR0.v;
		result.r[1] = DirectX::g_XMIdentityR1.v;
		result.r[2] = DirectX::g_XMIdentityR2.v;
		result.r[3] = DirectX::g_XMIdentityR3.v;
		return result;
	}

	[[nodiscard]]
	inline XMFLOAT4X4 XM_CALLCONV Inverse(const XMFLOAT4X4& matrix) noexcept
	{
		XMFLOAT4X4 result{};

		const XMMATRIX mat = DirectX::XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrix));
		XMStoreFloat4x4(&result, mat);

		return result;
	}

	[[nodiscard]]
	inline XMFLOAT4X4 XM_CALLCONV Inverse(XMFLOAT4X4&& matrix) noexcept
	{
		XMFLOAT4X4 result{};

		const XMMATRIX mat = DirectX::XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrix));
		XMStoreFloat4x4(&result, mat);

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT3X3 XM_CALLCONV Transpose(const XMFLOAT3X3& matrix) noexcept
	{
		XMFLOAT3X3 result{};
		if (util::is_constant_evaluated())
		{
			result._12 = matrix._21;
			result._21 = matrix._12;
			result._13 = matrix._31;
			result._31 = matrix._13;
			result._23 = matrix._32;
			result._32 = matrix._23;
		}
		else
		{
			XMStoreFloat3x3(&result, DirectX::XMMatrixTranspose(XMLoadFloat3x3(&matrix)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT3X3 XM_CALLCONV Transpose(XMFLOAT3X3&& matrix) noexcept
	{
		XMFLOAT3X3 result{};
		if (util::is_constant_evaluated())
		{
			result._12 = static_cast<float&&>(matrix._21);
			result._21 = static_cast<float&&>(matrix._12);
			result._13 = static_cast<float&&>(matrix._31);
			result._31 = static_cast<float&&>(matrix._13);
			result._23 = static_cast<float&&>(matrix._32);
			result._32 = static_cast<float&&>(matrix._23);
		}
		else
		{
			XMStoreFloat3x3(&result, DirectX::XMMatrixTranspose(XMLoadFloat3x3(&matrix)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT4X4 XM_CALLCONV Transpose(const XMFLOAT4X4& matrix) noexcept
	{
		XMFLOAT4X4 result{};
		if (util::is_constant_evaluated())
		{
			result._12 = matrix._21;
			result._21 = matrix._12;
			result._13 = matrix._31;
			result._31 = matrix._13;
			result._14 = matrix._41;
			result._41 = matrix._14;
			result._23 = matrix._32;
			result._32 = matrix._23;
			result._24 = matrix._42;
			result._42 = matrix._24;
			result._34 = matrix._43;
			result._43 = matrix._34;
		}
		else
		{
			XMStoreFloat4x4(&result, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&matrix)));
		}

		return result;
	}

	[[nodiscard]]
	constexpr XMFLOAT4X4 XM_CALLCONV Transpose(XMFLOAT4X4&& matrix) noexcept
	{
		XMFLOAT4X4 result{};
		if (util::is_constant_evaluated())
		{
			result._12 = static_cast<float&&>(matrix._21);
			result._21 = static_cast<float&&>(matrix._12);
			result._13 = static_cast<float&&>(matrix._31);
			result._31 = static_cast<float&&>(matrix._13);
			result._14 = static_cast<float&&>(matrix._41);
			result._41 = static_cast<float&&>(matrix._14);
			result._23 = static_cast<float&&>(matrix._32);
			result._32 = static_cast<float&&>(matrix._23);
			result._24 = static_cast<float&&>(matrix._42);
			result._42 = static_cast<float&&>(matrix._24);
			result._34 = static_cast<float&&>(matrix._43);
			result._43 = static_cast<float&&>(matrix._34);
		}
		else
		{
			XMStoreFloat4x4(&result, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&matrix)));
		}

		return result;
	}

	[[nodiscard]]
	inline XMFLOAT4X4 XM_CALLCONV RotationAxis(const XMFLOAT3& axis, const float& angle) noexcept
	{
		XMFLOAT4X4 result{};

		const float radian = DirectX::XMConvertToRadians(angle);
		const XMMATRIX rot = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
		XMStoreFloat4x4(&result, rot);
		return result;
	}

	[[nodiscard]]
	inline XMFLOAT4X4 XM_CALLCONV RotationAxis(const XMFLOAT3& axis, float&& angle) noexcept
	{
		XMFLOAT4X4 result{};

		const float radian = DirectX::XMConvertToRadians(static_cast<float&&>(angle));
		const XMMATRIX rot = DirectX::XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
		XMStoreFloat4x4(&result, rot);
		return result;
	}
}
