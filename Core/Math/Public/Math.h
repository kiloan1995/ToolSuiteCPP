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

	/*Linear interpolate between a and b, alpha is the ratio.
	@Return is unclamped*/
	inline static double Lerp(double A, double B, double Alpha)
	{
		return (B - A) * Alpha + A;
	}

	/*Inverse of lerp function
	@Return is unclamped. Returns the ratio of value between a and b*/
	inline static double InverseLerp(double A, double B, double Value)
	{
		if (A == B) return 0.0;
		return (Value - A) / (B - A);
	}

	inline static double Random(double Min, double Max)
	{
		// https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(Min, Max);
		return dist(mt);
	}

	inline static int32_t Random(int32_t Min, int32_t Max)
	{
		// https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(static_cast<double>(Min), static_cast<double>(Max));
		return static_cast<int32_t>(dist(mt));
	}

	/*converts a from radians to degrees*/
	inline static double RadInDeg(double A)
	{
		return A * 180.0 / PI_DOUBLE;
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