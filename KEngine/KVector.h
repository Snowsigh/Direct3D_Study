#pragma once
#include <Windows.h>
#include <math.h>
#include "TMath.h"
#define TBASIS_EPSILON		((FLOAT)  0.001f)
#define TBASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))
#define MAKE_ARGB(a, r, g, b)			(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

namespace TBASIS_VECTOR {

	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};

	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	struct float4
	{
		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};
	class KVector3 : public float3
	{
	public:
		KVector3();
		KVector3(const KVector3& v0);
		KVector3(float fX, float fY, float fZ);
		// 연산자 재정의 
		KVector3 operator + (KVector3 const& v0);
		KVector3 operator - (KVector3 const& v0);
		KVector3 operator * (float const& fScala);
		KVector3 operator + (float const& fScala);
		KVector3 operator / (float const& fScala);
		// Dot Product
		float operator | (KVector3 const& v0);
		// Cross Product
		KVector3 operator ^ (KVector3 const& v0);
		bool	operator == (KVector3 const& v0);
		// 제곱
		float LengthSquared();
		// 원점으로 부터의 거리
		float Length();
		KVector3 Normal();
		
		float Angle(KVector3& v0);

		TVector3 ChanigeTK(KVector3 v0);
		KVector3 ChanigeKT(TVector3 v0);

	};
	class KVector4 : public float4
	{
	public:
		KVector4();
		KVector4(const KVector4& v0);
		KVector4(float fX, float fY, float fZ, float fW);
		TVector4 ChanigeTK(KVector4 v0);
		KVector4 ChanigeKT(TVector4 v0);
	};

	class KVector2 : public float2
	{
	public:
		KVector2();
		KVector2(const KVector2& v0);
		KVector2(float fX, float fY);
	};
}
using namespace TBASIS_VECTOR;
