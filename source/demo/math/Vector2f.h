#pragma once


namespace Demo
{
inline namespace Math
{
	struct Vector2f final
	{
		static constexpr float NAN_VALUE = NAN;


		float	x = NAN_VALUE;
		float	y = NAN_VALUE;


		inline friend void swap( Vector2f& left, Vector2f& right ) noexcept	{ left.Swap( right ); };


		inline Vector2f() noexcept						= default;
		inline Vector2f( const Vector2f& ) noexcept		= default;
		inline Vector2f( Vector2f&& other ) noexcept	: x{ std::exchange( other.x, NAN_VALUE ) }, y{ std::exchange( other.y, NAN_VALUE ) } {};
		inline ~Vector2f() noexcept						= default;

		inline Vector2f( const float x, const float y ) noexcept	: x{ x }, y{ y } {};


		inline Vector2f& operator = ( const Vector2f& ) noexcept	= default;
		inline Vector2f& operator = ( Vector2f&& other ) noexcept	{ Vector2f{ std::move( other ) }.Swap( *this ); return *this; };


		inline void Swap( Vector2f& other ) noexcept;


		inline Vector2f& Minimize( const Vector2f& other );
		inline Vector2f Minimized( const Vector2f& other ) const;
		inline Vector2f& Maximize( const Vector2f& other );
		inline Vector2f Maximized( const Vector2f& other ) const;


		inline const float GetSquareLength() const;
		inline const float GetLength() const;
	};
}
}