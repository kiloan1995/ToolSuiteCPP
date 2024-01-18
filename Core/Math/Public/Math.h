#pragma once
#include <cstdint>
#include <random>

static const double PI_DOUBLE = 3.1415926535897932384626433832795028841971;

class FMath
{
public:
	inline static double Abs(double Value)
	{
		return std::abs(Value);
	}

	/*Clamps value to min and max*/
	inline static double Clamp(double Value, double Min, double Max)
	{
		if (Value > Max) return Max;
		if (Value < Min) return Min;
		return Value;
	}

	/*Inverse of lerp function
	@Return is unclamped. Returns the ratio of value between a and b*/
	inline static double InverseLerp(double A, double B, double Value)
	{
		if (A == B) return 0.0;
		return (Value - A) / (B - A);
	}

	/*Linear interpolate between a and b, alpha is the ratio.
	@Return is unclamped*/
	inline static double Lerp(double A, double B, double Alpha)
	{
		return (B - A) * Alpha + A;
	}

	// Log with base 2
	inline static double Log2(double Value)
	{
		return log2(Value);
	}

	// Log with base 10
	inline static double Log10(double Value)
	{
		return log10(Value);
	}

	// Log with base E
	inline static double LogE(double Value)
	{
		return log(Value);
	}

	// Log with custom Base
	inline static double LogX(double Value, double Base)
	{
		return log(Value) / log(Base);
	}

	/*Returns lower value*/
	template<typename T>
	inline static T Min(T A, T B)
	{
		return min(A, B);
	}

	/*Returns higher value*/
	template<typename T>
	inline static T Max(T A, T B)
	{
		return max(A, B);
	}

	/*converts a from radians to degrees*/
	inline static double RadInDeg(double A)
	{
		return A * 180.0 / PI_DOUBLE;
	}

	/*Returns random double in Range [Min, Max]*/
	inline static double Random(double Min, double Max)
	{
		// https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(Min, std::nextafter(Max, DBL_MAX));

		return dist(mt);
	}

	/*Returns random int32_t in Range [Min, Max]*/
	inline static int32_t Random(int32_t Min, int32_t Max)
	{
		// https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(static_cast<double>(Min), std::nextafter(static_cast<double>(Max), DBL_MAX));

		return static_cast<int32_t>(dist(mt));
	}

	/*Remap Value from "In" to "Out" Range, unclamped*/
	inline static double Remap(double InMin, double InMax, double OutMin, double OutMax, double Value)
	{
		if (Abs(InMax - InMin) < DBL_EPSILON) return 0.0;
		if (Abs(OutMax - OutMin) < DBL_EPSILON) return 0.0;
		return OutMin + (Value - InMin) / (InMax - InMin) * (OutMax - OutMin);
	}

	/*Remap Value from "In" to "Out" Range, Clamped*/
	inline static double RemapClamped(double InMin, double InMax, double OutMin, double OutMax, double Value)
	{
		return Clamp(Remap(InMin, InMax, OutMin, OutMax, Value), OutMin, OutMax);
	}


	/*converts a from degrees to radians*/
	inline static double DegInRad(double A)
	{
		return A * PI_DOUBLE / 180.0f;
	}

	/*@Return the square of a*/
	inline static double Square(double A)
	{
		return A * A;
	}

	/*@Return the squareroot of a*/
	inline static double Sqrt(double A)
	{
		return std::sqrt(A);
	}
};