#pragma once


namespace Demo
{
inline namespace Math
{
	inline Vector2f operator + ( const Vector2f& left, const Vector2f& right )
	{
		return { left.x + right.x, left.y + right.y };
	}

	inline Vector2f& operator += ( Vector2f& left, const Vector2f& right )
	{
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	inline Vector2f operator - ( const Vector2f& left, const Vector2f& right )
	{
		return { left.x - right.x, left.y - right.y };
	}

	inline Vector2f& operator -= ( Vector2f& left, const Vector2f& right )
	{
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}

	inline Vector2f operator * ( const Vector2f& left, const float value )
	{
		return { left.x * value, left.y * value };
	}
}
}
