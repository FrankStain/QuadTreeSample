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


		friend void swap( Vector2f& left, Vector2f& right )	{ left.Swap( right ); };


		Vector2f()						= default;
		Vector2f( const Vector2f& )		= default;
		Vector2f( Vector2f&& other )	: x{ std::exchange( other.x, NAN_VALUE ) }, y{ std::exchange( other.y, NAN_VALUE ) } {};
		~Vector2f()						= default;

		Vector2f( const float x, const float y )	: x{ x }, y{ y } {};


		Vector2f& operator = ( const Vector2f& )	= default;
		Vector2f& operator = ( Vector2f&& other )	{ Vector2f{ std::move( other ) }.Swap( *this ); return *this; };


		void Swap( Vector2f& other );


		Vector2f& Minimize( const Vector2f& other );
		Vector2f Minimized( const Vector2f& other ) const;
		Vector2f& Maximize( const Vector2f& other );
		Vector2f Maximized( const Vector2f& other ) const;


		const float GetSquareLength() const;
		const float GetLength() const;
	};
}
}