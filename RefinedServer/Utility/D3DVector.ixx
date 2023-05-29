module;
#include <DirectXMath.h>
export module Utility.D3D.Vector;

export namespace d3d::vec
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
}
