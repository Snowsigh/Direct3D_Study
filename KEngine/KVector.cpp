#include "KVector.h"

KVector3::KVector3()
{
	x = y = z = 0.0f;
}

KVector3::KVector3(const KVector3& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
}

KVector3::KVector3(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

// 연산자 재정의 
KVector3 KVector3::operator + (KVector3 const& v0)
{
	return KVector3(v0.x + x, v0.y + y, v0.z + z);
}

KVector3 KVector3::operator - (KVector3 const& v0)
{
	return KVector3(x - v0.x, y - v0.y, z - v0.z);
}

KVector3 KVector3::operator + (float const& fScala)
{
	return KVector3(fScala + x, fScala + y, fScala + z);
}
KVector3 KVector3::operator * (float const& fScala)
{
	return KVector3(fScala * x, fScala * y, fScala * z);
}
KVector3 KVector3::operator / (float const& fScala)
{
	float fInvert = 1.0f / fScala;
	return KVector3(x * fInvert, y * fInvert, z * fInvert);
}

// Dot Product
float KVector3::operator | (KVector3 const& v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// Cross Product
KVector3 KVector3::operator ^ (KVector3 const& v0)
{
	return KVector3((y * v0.z - z * v0.y), (z * v0.x - x * v0.z), (x * v0.y - y * v0.x));
}

bool	KVector3::operator == (KVector3 const& v0)
{
	if (fabs(x - v0.x) < TBASIS_EPSILON)
	{
		if (fabs(y - v0.y) < TBASIS_EPSILON)
		{
			if (fabs(z - v0.z) < TBASIS_EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

// 제곱
float KVector3::LengthSquared()
{
	return (x * x + y * y + z * z);
}

// 원점으로 부터의 거리
float KVector3::Length()
{
	return (float)sqrt(LengthSquared());
}

KVector3 KVector3::Normal()
{
	float invertLength = 1.0f / Length();
	return KVector3(x * invertLength, y * invertLength, z * invertLength);
}



float KVector3::Angle(KVector3& v0)
{
	float fLength1 = Length();
	float fLength2 = v0.Length();
	float fCosAngle = ((*this) | v0) / (fLength1 * fLength2);
	return (float)RadianToDegree(acos(fCosAngle));
}


KVector4::KVector4()
{
	x = y = z = w = 0.0f;
}

KVector4::KVector4(const KVector4& v0)
{
	x = v0.x;
	y = v0.y;
	z = v0.z;
	w = v0.w;
}

KVector4::KVector4(float fX, float fY, float fZ, float fW)
{
	x = fX;
	y = fY;
	z = fZ;
	w = fW;
}




KVector2::KVector2()
{
	x = y = 0.0f;
}

KVector2::KVector2(const KVector2& v0)
{
	x = v0.x;
	y = v0.y;
}

KVector2::KVector2(float fX, float fY)
{
	x = fX;
	y = fY;
}

TVector3 TBASIS_VECTOR::KVector3::ChanigeTK(KVector3 v0)
{
	TVector3 q; q.x = v0.x; q.y = v0.y; q.z = v0.z; return q;
}
KVector3 TBASIS_VECTOR::KVector3::ChanigeKT(TVector3 v0)
{ 
	KVector3 q; q.x = v0.x; q.y = v0.y; q.z = v0.z; return q;
}

TVector4 TBASIS_VECTOR::KVector4::ChanigeTK(KVector4 v0)
{
	TVector4 q; q.x = v0.x; q.y = v0.y; q.z = v0.z; q.w = v0.w; return q;
}
KVector4 TBASIS_VECTOR::KVector4::ChanigeKT(TVector4 v0)
{
	KVector4 q; q.x = v0.x; q.y = v0.y; q.z = v0.z; q.w = v0.w; return q;
}