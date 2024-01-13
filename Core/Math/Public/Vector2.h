#pragma once
#include "Math.h"
#include <string>

struct FVector2
{
public:
	double X;
	double Y;

	/*Init*/
	FVector2(double InX, double InY)
	{
		X = InX;
		Y = InY;
	}

	/*Init by setting each component to Value*/
	FVector2(double Value)
	{
		X = Value;
		Y = Value;
	}

	/*Normalizes in place*/
	void Normalize()
	{
		double Len = Length();
		X /= Len;
		Y /= Len;
	}

	/*Clamps Vector to Min and Max*/
	inline static FVector2 Clamp(const FVector2& Vector, const FVector2& Min, const FVector2& Max)
	{
		// https://studyflix.de/mathematik/kreuzprodukt-vektorprodukt-2254
		return FVector2(FMath::Clamp(Vector.X, Min.X, Max.X), FMath::Clamp(Vector.Y, Min.Y, Max.Y));
	}

	inline double Dot(const FVector2& Other) const
	{
		return X * Other.X + Y * Other.Y;
	}

	inline double Distance(const FVector2& Other) const
	{
		return FMath::Sqrt(FMath::Square(X - Other.X) + FMath::Square(Y - Other.Y));
	}

	inline double DistanceSquared(const FVector2& Other) const
	{
		return FMath::Square(X - Other.X) + FMath::Square(Y - Other.Y);
	}

	inline double Length() const
	{
		return FMath::Sqrt(X * X + Y * Y);
	}

	inline double LengthSquared() const
	{
		return X * X + Y * Y;
	}

	/*Normalizes in place*/
	void Normalize()
	{
		double Len = Length();
		X /= Len;
		Y /= Len;
	}

	inline std::string ToString() const
	{
		return "X: " + std::to_string(X) + ",Y: " + std::to_string(Y);
	}

	FVector2 operator+(const FVector2& Other)
	{
		return FVector2(X + Other.X, Y + Other.Y);
	}

	FVector2 operator-(const FVector2& Other)
	{
		return FVector2(X - Other.X, Y - Other.Y);
	}

	FVector2 operator*(const double& Value)
	{
		return FVector2(X * Value, Y * Value);
	}

	FVector2 operator*(const FVector2& Other)
	{
		return FVector2(X * Other.X, Y * Other.Y);
	}

	void operator*=(const FVector2& Other)
	{
		X *= Other.X;
		Y *= Other.Y;
	}

	void operator*=(const double& Value)
	{
		X *= Value;
		Y *= Value;
	}

	FVector2 operator/(const double& Value)
	{
		return FVector2(X / Value, Y / Value);
	}

	FVector2 operator/(const FVector2& Other)
	{
		return FVector2(X / Other.X, Y / Other.Y);
	}

	void operator/=(const FVector2& Other)
	{
		X /= Other.X;
		Y /= Other.Y;
	}

	void operator/=(const double& Value)
	{
		X /= Value;
		Y /= Value;
	}

	void operator+=(const FVector2& Other)
	{
		X += Other.X;
		Y += Other.Y;
	}

	void operator-=(const FVector2& Other)
	{
		X -= Other.X;
		Y -= Other.Y;
	}

	bool operator==(const FVector2& Other)
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const FVector2& Other)
	{
		return X != Other.X || Y != Other.Y;
	}
};