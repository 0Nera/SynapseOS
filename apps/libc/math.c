#include <stdint.h>
#include "../include/math.h"

float lerp(float v0, float v1, float t) {
  return (1 - t) * v0 + t * v1;
}

uint32_t pow(uint32_t base, uint32_t power)
{
  uint32_t tmp = base;
  for(int i = power; i > 1; i--)
  {
    tmp *= base;
  }
  return tmp;
}

int coprimes(int a, int b)
{
  int n = 0;
  for(int i = 2; i < MIN(a,b); i++)
  {
    if(a%i == 0 && b%i == 0) return 1;//++n;
  }
  return n;
}

// used for testing if a float is an integer or not
static const uint8_t  gMaskShift[256] = {	0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //16
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //32
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //48
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //64
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //80
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //96
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //112
											0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,    //128
											8, 9,10,11, 12,13,14,15,16,17,18,19,20,21,22,23,    //144
										   24,25,26,27, 28,29,30,31,31,31,31,31,31,31,31,31,    //160
										   31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,    //176
										   31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,    //192
										   31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,    //208
										   31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,    //224
										   31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,    //240
										   31,31,31,31, 31,31,31,31,31,31,31,31,31,31,31,31,    //256
										};

                                        float sqrtf( float x )
{
	return __builtin_sqrtf(x);
}

// Не реализовано
float sqrt( float x )
{
	return (x);
}

// Не реализовано
float exp(float x)
{
  return (x);
}


float powf( float x, float y )
{
	static const double recip_ln2 = 0x1.71547652b82fep0;

	if( x == 1.0f || y == 1.0f)
		return x;

	//Move the arguments to the integer registers for bitwise inspection
	union{ float f; uint32_t u; } ux, uy;
	ux.f = x;
	uy.f = y;
	uint32_t absux = ux.u & 0x7fffffff;
	uint32_t absuy = uy.u & 0x7fffffff;


	// Handle most edge cases
	//If |x| or |y| is in { +-0, +-Inf, +-NaN }
	if( (ux.u - 1U) >= 0x7f7fffff || (absuy - 1) >= 0x4affffff )
	{
		// any**0 = 1.0f for all values, including NaN
		if( 0 == absuy )
			return 1.0f;

		// handle NaNs
		if( x != x || y != y )
			return x + y;

		//figure out if y is an odd integer
		//Find out if y is an integer or not without raising inexact
		//	Note -- independently tested over entire range. Fails for Inf/NaN. We don't care about that here.
		uint32_t fractMask = 0x3fffffffU >> gMaskShift[ absuy >> 23 ];			//mask bits cover fractional part of value
		uint32_t onesMask = 0x40000000U >> gMaskShift[ absuy >> 23 ];			// we get away with this because leading exponent bit is never set for |y| < 2.0
		uint32_t fractionalBits = absuy & fractMask;
		uint32_t onesBit = absuy & onesMask;

		if( 0 == absux )
		{
			//if y is an odd integer
			if( 0 == fractionalBits && 0 != onesBit )
			{
				if( y < 0.0f )
					return 1.0f / x;

				return x;
			}

			// y is not an odd integer
			if( 0.0f < y )
				return 0.0f;

			return 1.0f / __builtin_fabsf(x);			// return Inf and set div/0

		}

		// deal with infinite y
		if( 0x7f800000 == absuy )
		{
			if( -1.0f == x )
				return 1.0f;

			if( absux > 0x3f800000 )	// |x| > 1.0f
			{	// |x| > 1.0f
				if( 0.0f < y )
					return y;
				else
					return 0.0f;
			}
			else
			{	// |x| < 1.0f
				if( 0.0f < y )
					return 0.0f;
				else
					return __builtin_fabsf(y);
			}
		}

		// we can also deal with x == +inf at this point.
		if( x == __builtin_inff() )
		{
			if( y < 0.0f )
				return 0.0f;
			else
				return x;
		}

		if( x > -__builtin_inff() )
		{
			if( fractionalBits )
				goto nan_sqrt;

			goto ipowf;
		}

		// At this point, we know that x is in { +-0, -Inf } and y is finite non-zero.
		// Deal with y is odd integer cases
		if( 0 == fractionalBits && 0 != onesBit )	// if( |y| >= 1.0f || |y| < 0x1.0p24f )
			return 0.0f < y ? x : -0.0f;

		// x == -inf
		return 0.0f < y ? -x : 0.0f;
	}

	//special case for sqrts
	if( 0x3f000000U == absuy )
		goto nan_sqrt;

	// break |x| into exponent and fractional parts:		|x| = 2**i * m		1.0 <= m < 2.0
	int32_t	i = ((absux >> 23) & 0xff) - 127;
	union
	{
		uint32_t	u;
		float		f;
	}m = { (absux & 0x007fffffU) | 0x3f800000U };

	//normalize denormals
	if( -127 == i )
	{	//denormal
		m.f -= 1.0f;								//	exact
		i = ((m.u >> 23) & 0xff) - (127+126);
		m.u = (m.u & 0x807fffffU) | 0x3f800000U;
	}

	//
	//	We further break down m as :
	//
	//          m = (1+a/256.0)(1+r)              a = high 8 explicit bits of mantissa(m), b = next 7 bits
	//          log2f(m) = log2(1+a/256.0) + log2(1+r)
	//
	//      We use the high 7 bits of the mantissa to look up log2(1+a/256.0) in log2f_table above
	//      We calculate 1+r as:
	//
	//          1+r = m * (1 /(1+a/256.0))
	//
	//      We can lookup (from the same table) the value of 1/(1+a/256.0) based on a too.

	double log2x = i;

	if( m.f != 1.0f )
	{
		int index = (m.u >> (23-7-4)) & 0x7f0;		//top 7 bits of mantissa
		const double *tablep = (void*) powf_log_table + index;
		double r = (double) m.f;

		// reduce
		r *= tablep[0];		// reduce r to  1-2**-7 < r < 1+2**-7
		log2x += tablep[1]; // do this early to force -1.0 + 1.0 to cancel so that we don't end up with (1.0 + tiny) - 1.0 later on.
		r -= 1.0;			// -2**-7 < r < 1+2**-7

		// ln(1+r) = r - rr/2 + rrr/3 - rrrr/4 + rrrrr/5
		//	should provide log(1+r) to at least 35 bits of accuracy for the worst case
		double rr = r*r;
		double small = -0.5 + 0.3333333333333333333333*r;
		double large = -0.25 + 0.2*r;
		double rrrr = rr * rr;
		small *= rr;
		small += r;
		large *= rrrr;
		r = small + large;
		log2x += r * recip_ln2;
	}

	// multiply by Y
	double ylog2x = y * log2x;

// now we need to calculate 2**ylog2x

	//deal with overflow
	if( ylog2x >= 128.0 )
		return (float) (0x1.0p128 * ylog2x);		//set overflow and return inf

	//deal with underflow
	if( ylog2x <= -150.0 )
		return (float) ( ylog2x * 0x1.0p-1022 );		//minimum y * maximum log2(x) is ~-1.0p128 * ~128 = -1.0p135, so we can be sure that we'll drive this to underflow

	//separate ylog2x into integer and fractional parts
	int exp = (int) ylog2x;
	double f = ylog2x - exp;		//may be negative

	// Calculate 2**fract
	// 8th order minimax fit of exp2 on [-1.0,1.0].  |error| < 0.402865722354948566583852e-9:
	static const double c0 =  1.0 + 0.278626872016317130037181614004e-10;
	static const double c1 = .693147176943623740308984004029708;
	static const double c2 = .240226505817268621584559118975830;
	static const double c3 = 0.555041568519883074165425891257052e-1;
	static const double c4overc8 = 0.961813690023115610862381719985771e-2 / 0.134107709538786543922336536865157e-5;
	static const double c5overc8 = 0.133318252930790403741964203236548e-2 / 0.134107709538786543922336536865157e-5;
	static const double c6overc8 = 0.154016177542147239746127455226575e-3 / 0.134107709538786543922336536865157e-5;
	static const double c7overc8 = 0.154832722143258821052933667742417e-4 / 0.134107709538786543922336536865157e-5;
	static const double c8 = 0.134107709538786543922336536865157e-5;

	double z = 1.0;
	if( 0.0 != f )
	{ // don't set inexact if we don't need to
		double ff = f * f;
		double s7 = c7overc8 * f;			double s3 = c3 * f;
		double s5 = c5overc8 * f;			double s1 = c1 * f;
		double ffff = ff * ff;
		s7 += c6overc8;						s3 += c2;
		s5 += c4overc8;						s1 += c0;
		s7 *= ff;							s3 *= ff;
		s5 += ffff;
		double c8ffff = ffff * c8;
		s7 += s5;							s3 += s1;
		s7 *= c8ffff;
		z = s3 + s7;
	}


	//prepare 2**i
	union{ uint64_t u; double d; } two_exp = { ((uint64_t) exp + 1023) << 52 };

	return (float) (z * two_exp.d );


	//one last edge case -- pow(x, y) returns NaN and raises invalid for x < 0 and finite non-integer y
	// and one special case --	call sqrt for |y| == 0.5
nan_sqrt:
	if( x < 0.0f || y > 0.0f )
		return sqrtf(x);

	return (float) sqrt( 1.0 / (double) x );

ipowf:
	// clamp  -0x1.0p31 < y < 0x1.0p31
	y = y > -0x1.fffffep30f ? y : -0x1.fffffep30f;
	y = y <  0x1.fffffep30f ? y :  0x1.fffffep30f;
	i = (int) y;
	double dx = (double) x;
	double r = 1.0;

	if( i < 0 )
	{
		i = -i;
		dx = 1.0 / dx;
	}

	if( i & 1 )
		r = dx;

	do
	{
		i >>= 1;
		if( 0 == i )
			break;
		dx *= dx;
		if( i & 1 )
			r *= dx;
	}while(1);

	return (float) r;
}

float log10(float x)
{
   return (((powf(x, 0.00001f) - 1) / 0.00001f) * LOG10_E);
}

float log(float x)
{
   return ((powf(x, 0.00001f) - 1) / 0.00001f);//(log10(x) / LOG10_E);
}

float logx(float x, float base)
{
   // base may not equal 1 or be negative.
   if(base == 1.f || base < 0.f)
      return 0.f; // Return what you want here, could be Not-A-Number or NAN.

   return (log10(x) / log10(base));
}

double fabs(double val) {
	if(val < (double)0) return -val;
	return val;
}
