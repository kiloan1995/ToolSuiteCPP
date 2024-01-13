#pragma once
#include "Math.h"
#include <string>

struct FVector3
{
public:
	double X;
	double Y;
	double Z;

	/*Init*/
	FVector3(double InX, double InY, double InZ)
	{
		X = InX;
		Y = InY;
		Z = InZ;
	}

	/*Init by setting each component to Value*/
	FVector3(double Value)
	{
		X = Value;
		Y = Value;
		Z = Value;
	}

	/*Clamps Vector to Min and Max*/
	inline static FVector3 Clamp(const FVector3& Vector, const FVector3& Min, const FVector3& Max)
	{
		// https://studyflix.de/mathematik/kreuzprodukt-vektorprodukt-2254
		return FVector3(FMath::Clamp(Vector.X, Min.X, Max.X), FMath::Clamp(Vector.Y, Min.Y, Max.Y), FMath::Clamp(Vector.Z, Min.Z, Max.Z));
	}

	inline FVector3 Cross(const FVector3& Other) const
	{
		// https://studyflix.de/mathematik/kreuzprodukt-vektorprodukt-2254
		return FVector3(Y * Other.Z - Z * Other.Y, Z * Other.X - X * Other.Z, X * Other.Y - Y * Other.X);
	}

	inline double Dot(const FVector3& Other) const
	{
		return X * Other.X + Y * Other.Y + Z * Other.Z;
	}

	inline double Distance(const FVector3& Other) const
	{
		return FMath::Sqrt(FMath::Square(X - Other.X) + FMath::Square(Y - Other.Y) + FMath::Square(Z - Other.Z));
	}

	inline double DistanceSquared(const FVector3& Other) const
	{
		return FMath::Square(X - Other.X) + FMath::Square(Y - Other.Y) + FMath::Square(Z - Other.Z);
	}

	inline double Length() const
	{
		return FMath::Sqrt(X * X + Y * Y + Z * Z);
	}

	inline double LengthSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}

	/*Normalizes in place*/
	void Normalize()
	{
		double Len = Length();
		X /= Len;
		Y /= Len;
		Z /= Len;
	}

	inline std::string ToString() const
	{
		return "X: " + std::to_string(X) + ",Y: " + std::to_string(Y) + ",Z: " + std::to_string(Z);
	}

	FVector3 operator+(const FVector3& Other)
	{
		return FVector3(X + Other.X, Y + Other.Y, Z + Other.Z);
	}

	FVector3 operator-(const FVector3& Other)
	{
		return FVector3(X - Other.X, Y - Other.Y, Z - Other.Z);
	}

	FVector3 operator*(const double& Value)
	{
		return FVector3(X * Value, Y * Value, Z * Value);
	}

	FVector3 operator*(const FVector3& Other)
	{
		return FVector3(X * Other.X, Y * Other.Y, Z * Other.Z);
	}

	void operator*=(const FVector3& Other)
	{
		X *= Other.X;
		Y *= Other.Y;
		Z *= Other.Z;
	}

	void operator*=(const double& Value)
	{
		X *= Value;
		Y *= Value;
		Z *= Value;
	}

	FVector3 operator/(const double& Value)
	{
		return FVector3(X / Value, Y / Value, Z / Value);
	}

	FVector3 operator/(const FVector3& Other)
	{
		return FVector3(X / Other.X, Y / Other.Y, Z / Other.Z);
	}

	void operator/=(const FVector3& Other)
	{
		X /= Other.X;
		Y /= Other.Y;
		Z /= Other.Z;
	}

	void operator/=(const double& Value)
	{
		X /= Value;
		Y /= Value;
		Z /= Value;
	}

	void operator+=(const FVector3& Other)
	{
		X += Other.X;
		Y += Other.Y;
		Z += Other.Z;
	}

	void operator-=(const FVector3& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
		Z -= Other.Z;
	}

	bool operator==(const FVector3& Other)
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}

	bool operator!=(const FVector3& Other)
	{
		return X != Other.X || Y != Other.Y || Z != Other.Z;
	}
};