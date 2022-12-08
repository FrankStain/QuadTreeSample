#pragma once


namespace Demo
{
inline namespace Math
{
	inline void Vector2f::Swap( Vector2f& other ) noexcept
	{
		using std::swap;
		swap( x, other.x );
		swap( y, other.y );
	}

	inline Vector2f& Vector2f::Minimize( const Vector2f& other )
	{
		x = std::min( x, other.x );
		y = std::min( y, other.y );

		return *this;
	}

	inline Vector2f Vector2f::Minimized( const Vector2f& other ) const
	{
		return Vector2f{ *this }.Minimize( other );
	}

	inline Vector2f& Vector2f::Maximize( const Vector2f& other )
	{
		x = std::max( x, other.x );
		y = std::max( y, other.y );

		return *this;
	}

	inline Vector2f Vector2f::Maximized( const Vector2f& other ) const
	{
		return Vector2f{ *this }.Maximize( other );
	}

	inline const float Vector2f::GetSquareLength() const
	{
		return ( x * x ) + ( y * y );
	}

	inline const float Vector2f::GetLength() const
	{
		return std::sqrt( GetSquareLength() );
	}
}
}
