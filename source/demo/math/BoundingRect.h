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


		friend void swap( BoundingRect& left, BoundingRect& right )	{ left.Swap( right ); };


		BoundingRect()							= default;
		BoundingRect( const BoundingRect& )		= default;
		BoundingRect( BoundingRect&& other )	: min{ std::move( other.min ) }, max{ std::move( other.max ) } {};
		~BoundingRect()							= default;

		explicit BoundingRect( const Vector2f& point );
		BoundingRect( const Vector2f& point1, const Vector2f& point2 );
		BoundingRect( const Vector2f& min, const Vector2f& max, decltype( std::ignore ) );


		BoundingRect& operator = ( const BoundingRect& )	= default;
		BoundingRect& operator = ( BoundingRect&& other )	{ BoundingRect{ std::move( other ) }.Swap( *this ); return *this; };


		void Swap( BoundingRect& other );

		BoundingRect& Grow( const Vector2f& point );
		BoundingRect& Grow( const BoundingRect& rect );
		BoundingRect& Resize( const float new_size );
		BoundingRect& Resize( const Vector2f& new_size );

		Vector2f GetCorner( const size_t index ) const;
		Vector2f GetCenter() const;
		Vector2f GetSize() const;

		const size_t GetNearestCornerIndex( const Vector2f& point ) const;

		const bool ConsistsOf( const Vector2f& point ) const;
		const bool ConsistsOf( const BoundingRect& rect ) const;
		const bool IsIntersects( const BoundingRect& rect ) const;
		const bool IsIntersects( const Vector2f& center, const float radius ) const;
	};
}
}
