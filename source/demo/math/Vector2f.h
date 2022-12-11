#pragma once


namespace Demo
{
inline namespace Math
{
	/**
		@brief	2D vector of float elements.

		Vector represents the `(x, y)` point or direction in 2D space.
		It also provide some subset of functions that necessary for this project.
		Vector instances are swappable.
	*/
	struct Vector2f final
	{
		// Default value for fields.
		static constexpr float NAN_VALUE = NAN;


		float x = NAN_VALUE;	// X axis.
		float y = NAN_VALUE;	// Y axis.


		// `swap` function support.
		inline friend void swap( Vector2f& left, Vector2f& right ) noexcept	{ left.Swap( right ); };


		inline Vector2f() noexcept						= default;
		inline Vector2f( const Vector2f& ) noexcept		= default;
		inline Vector2f( Vector2f&& other ) noexcept	: x{ std::exchange( other.x, NAN_VALUE ) }, y{ std::exchange( other.y, NAN_VALUE ) } {};
		inline ~Vector2f() noexcept						= default;

		inline Vector2f( const float x, const float y ) noexcept	: x{ x }, y{ y } {};


		inline Vector2f& operator = ( const Vector2f& ) noexcept	= default;
		inline Vector2f& operator = ( Vector2f&& other ) noexcept	{ Vector2f{ std::move( other ) }.Swap( *this ); return *this; };


		// Performs the value swapping.
		inline void Swap( Vector2f& other ) noexcept;


		// Makes this vector to be minimal against other one. For each axis of this vector performs `min(this, other)`.
		inline Vector2f& Minimize( const Vector2f& other );

		// Produces the minimal vector between this one and other. For each axis of this vector performs `min(this, other)`.
		inline Vector2f Minimized( const Vector2f& other ) const;

		// Makes this vector to be maximal against other one. For each axis of this vector performs `max(this, other)`.
		inline Vector2f& Maximize( const Vector2f& other );

		// Produces the maximal vector between this one and other. For each axis of this vector performs `max(this, other)`.
		inline Vector2f Maximized( const Vector2f& other ) const;


		// Calculates the quadratic (^2) length of vector.
		inline const float GetSquareLength() const;

		// Calculates the length of vector.
		inline const float GetLength() const;
	};
}
}