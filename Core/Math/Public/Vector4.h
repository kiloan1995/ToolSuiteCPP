#pragma once
#include "Math.h"
#include <string>

struct FVector4
{
public:
	double X;
	double Y;
	double Z;
	double W;

	/*Init*/
	FVector4(double InX, double InY, double InZ, double InW)
	{
		X = InX;
		Y = InY;
		Z = InZ;
		W = InW;
	}

	/*Init by setting each component to Value*/
	FVector4(double Value)
	{
		X = Value;
		Y = Value;
		Z = Value;
		W = Value;
	}

	/*Clamps Vector to Min and Max*/
	inline static FVector4 Clamp(const FVector4& Vector, const FVector4& Min, const FVector4& Max)
	{
		// https://studyflix.de/mathematik/kreuzprodukt-vektorprodukt-2254
		return FVector4(FMath::Clamp(Vector.X, Min.X, Max.X), FMath::Clamp(Vector.Y, Min.Y, Max.Y), FMath::Clamp(Vector.Z, Min.Z, Max.Z), FMath::Clamp(Vector.W, Min.W, Max.W));
	}

	inline double Length() const
	{
		return FMath::Sqrt(X * X + Y * Y + Z * Z + W * W);
	}

	inline double LengthSquared() const
	{
		return X * X + Y * Y + Z * Z + W * W;
	}

	/*Normalizes in place*/
	void Normalize()
	{
		double Len = Length();
		X /= Len;
		Y /= Len;
		Z /= Len;
		W /= Len;
	}

	inline std::string ToString() const
	{
		return "X: " + std::to_string(X) + ",Y: " + std::to_string(Y) + ",Z: " + std::to_string(Z) + ",W: " + std::to_string(W);
	}

	FVector4 operator+(const FVector4& Other)
	{
		return FVector4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
	}

	FVector4 operator-(const FVector4& Other)
	{
		return FVector4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
	}

	FVector4 operator*(const double& Value)
	{
		return FVector4(X * Value, Y * Value, Z * Value, W * Value);
	}

	FVector4 operator*(const FVector4& Other)
	{
		return FVector4(X * Other.X, Y * Other.Y, Z * Other.Z, W * Other.W);
	}

	void operator*=(const FVector4& Other)
	{
		X *= Other.X;
		Y *= Other.Y;
		Z *= Other.Z;
		W *= Other.W;
	}

	void operator*=(const double& Value)
	{
		X *= Value;
		Y *= Value;
		Z *= Value;
		W *= Value;
	}

	FVector4 operator/(const double& Value)
	{
		return FVector4(X / Value, Y / Value, Z / Value, W / Value);
	}

	FVector4 operator/(const FVector4& Other)
	{
		return FVector4(X / Other.X, Y / Other.Y, Z / Other.Z, W / Other.W);
	}

	void operator/=(const FVector4& Other)
	{
		X /= Other.X;
		Y /= Other.Y;
		Z /= Other.Z;
		W /= Other.W;
	}

	void operator/=(const double& Value)
	{
		X /= Value;
		Y /= Value;
		Z /= Value;
		W /= Value;
	}

	void operator+=(const FVector4& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
		W += Other.W;
	}

	void operator-=(const FVector4& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		Z -= Other.Z;
		W -= Other.W;
	}

	bool operator==(const FVector4& Other)
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
	}

	bool operator!=(const FVector4& Other)
	{
		return X != Other.X || Y != Other.Y || Z != Other.Z || W != Other.W;
	}
};