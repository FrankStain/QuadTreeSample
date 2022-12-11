#pragma once


namespace Demo
{
inline namespace Math
{
	/**
		@brief	Axis-Aligned Bounding Rect in 2D space.

		Bounding rect is represented by minimum and maximum points. All four corners of axis-aligned box may be deduced by combining of this points components.
		It also provide some minimal necessary subset of functions.
		Bounding rect instances are swappable.
	*/
	struct BoundingRect final
	{
		// Count of corners in rect. It helps to get rid of magic numbers in code.
		static constexpr size_t CORNERS_COUNT = 4;


		Vector2f min; // Most minimal point in rect.
		Vector2f max; // Most maximal point in rect.


		// `swap` function support.
		inline friend void swap( BoundingRect& left, BoundingRect& right ) noexcept	{ left.Swap( right ); };


		inline BoundingRect() noexcept							= default;
		inline BoundingRect( const BoundingRect& ) noexcept		= default;
		inline BoundingRect( BoundingRect&& other ) noexcept	: min{ std::move( other.min ) }, max{ std::move( other.max ) } {};
		inline ~BoundingRect() noexcept							= default;

		inline explicit BoundingRect( const Vector2f& point ) noexcept;
		inline BoundingRect( const Vector2f& point1, const Vector2f& point2 ) noexcept;
		inline BoundingRect( const Vector2f& min, const Vector2f& max, decltype( std::ignore ) ) noexcept;


		inline BoundingRect& operator = ( const BoundingRect& ) noexcept	= default;
		inline BoundingRect& operator = ( BoundingRect&& other ) noexcept	{ BoundingRect{ std::move( other ) }.Swap( *this ); return *this; };


		// Performs the value swapping.
		inline void Swap( BoundingRect& other ) noexcept;

		// Grow the rect. `point` will be always inside the rect after the growing.
		inline BoundingRect& Grow( const Vector2f& point );

		// Grow the rect. `rect` will be always inside the rect after the growing.
		inline BoundingRect& Grow( const BoundingRect& rect );

		// Change the size of rect. Center point will remains same as before resizing.
		inline BoundingRect& Resize( const float new_size );

		// Change the size of rect. Center point will remains same as before resizing.
		inline BoundingRect& Resize( const Vector2f& new_size );


		// Get the corner point by index. Corners are indexed like `{ (min, min), (max, min), (max, max), (min, max) }`.
		inline Vector2f GetCorner( const size_t index ) const;

		// Get the center point of rect.
		inline Vector2f GetCenter() const;

		// Get the size of rect.
		inline Vector2f GetSize() const;

		// Get the index of nearest corner for given point.
		inline const size_t GetNearestCornerIndex( const Vector2f& point ) const;


		// Whether the point lies inside of this rect. Or, whether the rect consists of given point.
		inline const bool ConsistsOf( const Vector2f& point ) const;

		// Whether the given rect lies totally inside of this rect. Or, whether the rect consists of given rect.
		inline const bool ConsistsOf( const BoundingRect& rect ) const;

		// Whether the rect intersects with given one.
		inline const bool IsIntersects( const BoundingRect& rect ) const;

		// Whether the rect intersects with circle given by center point and radius.
		inline const bool IsIntersects( const Vector2f& center, const float radius ) const;
	};
}
}
