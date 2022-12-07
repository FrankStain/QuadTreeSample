#pragma once


namespace Demo
{
inline namespace Math
{
	BoundingRect::BoundingRect( const Vector2f& point )
		: BoundingRect{ point, point, std::ignore }
	{
	}

	BoundingRect::BoundingRect( const Vector2f& point1, const Vector2f& point2 )
		: BoundingRect{ point1.Minimized( point2 ), point2.Maximized( point1 ), std::ignore }
	{
	}

	BoundingRect::BoundingRect( const Vector2f& min, const Vector2f& max, decltype( std::ignore ) )
		: min{ min }
		, max{ max }
	{
	}

	void BoundingRect::Swap( BoundingRect& other )
	{
		using std::swap;
		swap( min, other.min );
		swap( max, other.max );
	}

	BoundingRect& BoundingRect::Grow( const Vector2f& point )
	{
		min.Minimize( point );
		max.Maximize( point );

		return *this;
	}

	BoundingRect& BoundingRect::Grow( const BoundingRect& rect )
	{
		min.Minimize( rect.min );
		max.Maximize( rect.max );

		return *this;
	}

	BoundingRect& BoundingRect::Resize( const float new_size )
	{
		return Resize( { new_size, new_size } );
	}

	BoundingRect& BoundingRect::Resize( const Vector2f& new_size )
	{
		const Vector2f offset{ new_size - GetSize() };
		min -= offset;
		max += offset;

		return *this;
	}

	Vector2f BoundingRect::GetCorner( const size_t index ) const
	{
		using Field = Vector2f BoundingRect::*;
		constexpr std::pair<Field, Field> corner_fields[ CORNERS_COUNT ] {
			{ &BoundingRect::min, &BoundingRect::min },
			{ &BoundingRect::max, &BoundingRect::min },
			{ &BoundingRect::max, &BoundingRect::max },
			{ &BoundingRect::min, &BoundingRect::max },
		};

		const auto [ x_field, y_field ] = corner_fields[ index ];
		return { (this->*x_field).x, (this->*y_field).y };
	}

	Vector2f BoundingRect::GetCenter() const
	{
		return { ( min.x + max.x ) * 0.5f, ( min.y + max.y ) * 0.5f };
	}

	Vector2f BoundingRect::GetSize() const
	{
		return max - min;
	}

	const size_t BoundingRect::GetNearestCornerIndex( const Vector2f& point ) const
	{
		const Vector2f direction{ point - GetCenter() };

		const size_t translation[] { 0, 1, 3, 2 };
		return translation[ size_t( ( direction.x < 0.0f )? 0 : 1 ) + ( ( direction.y < 0.0f )? 0 : 2 ) ];
	}

	const bool BoundingRect::ConsistsOf( const Vector2f& point ) const
	{
		return ( ( point.x >= min.x ) && ( point.x <= max.x ) ) && ( ( point.y >= min.y ) && ( point.y <= max.y ) );
	}

	const bool BoundingRect::ConsistsOf( const BoundingRect& rect ) const
	{
		return ( ( rect.min.x >= min.x ) && ( rect.min.y >= min.y ) ) && ( ( rect.max.x <= max.x ) && ( rect.max.y <= max.y ) );
	}

	const bool BoundingRect::IsIntersects( const BoundingRect& rect ) const
	{
		return ConsistsOf( { min.Maximized( rect.min ), max.Minimized( rect.max ) } );
	}

	const bool BoundingRect::IsIntersects( const Vector2f& center, const float radius ) const
	{
		const size_t corner_index = GetNearestCornerIndex( center );
		return ( GetCorner( corner_index ) - center ).GetLength() <= radius;
	}
}
}
