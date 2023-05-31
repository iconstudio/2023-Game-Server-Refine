module;
#include <DirectXMath.h>
export module Utility.D3D.Matrix;

export namespace d3d::mat
{
	using DirectX::XMMATRIX;
	using DirectX::XMFLOAT3X3;
	using DirectX::XMFLOAT3X4;
	using DirectX::XMFLOAT4X3;
	using DirectX::XMFLOAT4X4;
	using DirectX::XMFLOAT3X4A;
	using DirectX::XMFLOAT4X3A;
	using DirectX::XMFLOAT4X4A;

	using DirectX::operator+;
	using DirectX::operator+=;
	using DirectX::operator-;
	using DirectX::operator-=;
	using DirectX::operator*;
	using DirectX::operator*=;
	using DirectX::operator/;
	using DirectX::operator/=;

	
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
}
