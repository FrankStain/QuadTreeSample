#pragma once


namespace Demo
{
inline namespace Math
{
	struct BoundingRect final
	{
		static constexpr size_t CORNERS_COUNT = 4;


		Vector2f	min;
		Vector2f	max;


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


		inline void Swap( BoundingRect& other ) noexcept;

		inline BoundingRect& Grow( const Vector2f& point );
		inline BoundingRect& Grow( const BoundingRect& rect );
		inline BoundingRect& Resize( const float new_size );
		inline BoundingRect& Resize( const Vector2f& new_size );

		inline Vector2f GetCorner( const size_t index ) const;
		inline Vector2f GetCenter() const;
		inline Vector2f GetSize() const;

		inline const size_t GetNearestCornerIndex( const Vector2f& point ) const;

		inline const bool ConsistsOf( const Vector2f& point ) const;
		inline const bool ConsistsOf( const BoundingRect& rect ) const;
		inline const bool IsIntersects( const BoundingRect& rect ) const;
		inline const bool IsIntersects( const Vector2f& center, const float radius ) const;
	};
}
}
