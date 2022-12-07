#include <cstdint>
#include <cmath>
#include <array>
#include <vector>
#include <list>
#include <queue>
#include <iterator>
#include <memory>
#include <optional>
#include <variant>
#include <utility>
#include <algorithm>
#include <random>


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

void Vector2f::Swap( Vector2f& other )
{
	using std::swap;
	swap( x, other.x );
	swap( y, other.y );
}

Vector2f& Vector2f::Minimize( const Vector2f& other )
{
	x = std::min( x, other.x );
	y = std::min( y, other.y );

	return *this;
}

Vector2f Vector2f::Minimized( const Vector2f& other ) const
{
	return Vector2f{ *this }.Minimize( other );
}

Vector2f& Vector2f::Maximize( const Vector2f& other )
{
	x = std::max( x, other.x );
	y = std::max( y, other.y );

	return *this;
}

Vector2f Vector2f::Maximized( const Vector2f& other ) const
{
	return Vector2f{ *this }.Maximize( other );
}

const float Vector2f::GetSquareLength() const
{
	return ( x * x ) + ( y * y );
}

const float Vector2f::GetLength() const
{
	return std::sqrt( GetSquareLength() );
}

Vector2f operator + ( const Vector2f& left, const Vector2f& right )
{
	return { left.x + right.x, left.y + right.y };
}

Vector2f& operator += ( Vector2f& left, const Vector2f& right )
{
	left.x += right.x;
	left.y += right.y;
	return left;
}

Vector2f operator - ( const Vector2f& left, const Vector2f& right )
{
	return { left.x - right.x, left.y - right.y };
}

Vector2f& operator -= ( Vector2f& left, const Vector2f& right )
{
	left.x -= right.x;
	left.y -= right.y;
	return left;
}

Vector2f operator * ( const Vector2f& left, const float value )
{
	return { left.x * value, left.y * value };
}


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


class Shape;
struct Quad;
class QuadTree;

class Point final
{
public:
	Point()		= default;
	~Point()	= default;

	Point( Shape& host, const Vector2f& position )	: m_host{ &host }, m_position{ position } {};

public:
	void SetPosition( const Vector2f& position )	{ m_position = position; };

	const Vector2f& GetPosition() const				{ return m_position; };

	const Shape* GetHost() const					{ return m_host; };

private:
	Shape*		m_host		= nullptr;
	Vector2f	m_position;
};


class Shape final
{
public:
	Shape() = delete;
	~Shape() = default;

	Shape( QuadTree& host, const BoundingRect& bounds );

public:
	void SetBounds( const BoundingRect& bounds );

	void SetTag( const size_t value )						{ m_tag = value; };


	const BoundingRect& GetBounds() const					{ return m_bounds; };
	const Point& GetPoint( const size_t index ) const		{ return m_points[ index ]; };
	const size_t GetTag() const								{ return m_tag; };


	const Point& operator [] ( const size_t index ) const	{ return GetPoint( index ); };

private:
	QuadTree*		m_host		= nullptr;
	BoundingRect	m_bounds;
	Point			m_points[4];

	size_t			m_tag		= 0;
};

Shape::Shape( QuadTree& host, const BoundingRect& bounds )
	: m_host{ &host }
	, m_bounds{ bounds }
{
	for( size_t index = 0; index < std::size( m_points ); ++index )
	{
		m_points[ index ] = { *this, bounds.GetCorner( index ) };
	}
}

void Shape::SetBounds( const BoundingRect& bounds )
{
	m_bounds = bounds;
	for( size_t index = 0; index < std::size( m_points ); ++index )
	{
		m_points[ index ].SetPosition( bounds.GetCorner( index ) );
	}
}


using Points	= std::vector<const Point*>;
using Quads		= std::array<std::shared_ptr<Quad>, 4>;
using QuadState	= std::variant<Points, Quads>;


struct Quad final
{
	BoundingRect	bounds;
	Vector2f		center;
	size_t			level;
	QuadState		state;
};


class ShapeProvider final
{
public:
	enum class Handle : size_t;

public:
	std::pair<Shape*, Handle> Create( QuadTree& host, const BoundingRect& bounds );
	void Destroy( const Handle handle );

	Shape* Query( const Handle handle ) const;

private:
	static constexpr size_t BUCKET_LENGTH = 8;

private:
	using Bucket = std::array<std::optional<Shape>, BUCKET_LENGTH>;

private:
	static const bool HasSlot( const Bucket& bucket );

private:
	std::optional<Shape>& GetSlot( const Handle handle ) const;

private:
	std::vector<std::unique_ptr<Bucket>> m_slots;
};

std::pair<Shape*, ShapeProvider::Handle> ShapeProvider::Create( QuadTree& host, const BoundingRect& bounds )
{
	auto suitable_bucket = std::find_if( m_slots.begin(), m_slots.end(), []( const std::unique_ptr<Bucket>& bucket ) { return HasSlot( *bucket ); } );
	if( suitable_bucket == m_slots.end() )
	{
		m_slots.emplace_back( std::make_unique<Bucket>() );
		suitable_bucket = std::prev( m_slots.end() );
	}

	Bucket& bucket = *suitable_bucket->get();
	auto suitable_slot = std::find_if( bucket.begin(), bucket.end(), []( const std::optional<Shape>& slot ) { return !slot.has_value(); } );

	Shape& shape = suitable_slot->emplace( host, bounds );
	return { &shape, Handle{ std::distance( m_slots.begin(), suitable_bucket ) * BUCKET_LENGTH + std::distance( bucket.begin(), suitable_slot ) } };
}

void ShapeProvider::Destroy( const Handle handle )
{
	GetSlot( handle ).reset();
}

Shape* ShapeProvider::Query( const Handle handle ) const
{
	auto& slot = GetSlot( handle );
	return ( slot.has_value() )? &slot.value() : nullptr;
}

const bool ShapeProvider::HasSlot( const Bucket& bucket )
{
	return std::any_of( bucket.begin(), bucket.end(), []( const std::optional<Shape>& slot ) { return !slot.has_value(); } );
}

std::optional<Shape>& ShapeProvider::GetSlot( const Handle handle ) const
{
	const size_t bucket_index	= size_t( handle ) / BUCKET_LENGTH;
	const size_t slot_index		= size_t( handle ) % BUCKET_LENGTH;

	return m_slots.at( bucket_index )->at( slot_index );
}


class QuadProvider final
{
public:
	std::shared_ptr<Quad> Create( const BoundingRect& bounds, const size_t level );

private:
	void Destroy( std::list<Quad>::iterator slot );

private:
	std::list<Quad>	m_slots;
};

std::shared_ptr<Quad> QuadProvider::Create( const BoundingRect& bounds, const size_t level )
{
	auto& quad = m_slots.emplace_back();

	quad.bounds = bounds;
	quad.center = bounds.GetCenter();
	quad.level	= level;

	return { &quad, [this, slot = std::prev( m_slots.end() )]( Quad* ) { Destroy( slot ); } };
}

void QuadProvider::Destroy( std::list<Quad>::iterator slot )
{
	slot->state.emplace<Points>();
	m_slots.erase( slot );
}


class QuadTree final
{
public:
	friend class Shape;

public:
	std::shared_ptr<Shape> RetainShape( const BoundingRect& bounds );

	std::vector<const Shape*> Find( const BoundingRect& bounds ) const;
	std::vector<const Shape*> Find( const Vector2f& center, const float radius ) const;

	const BoundingRect& GetBounds() const { return m_bounds; };

private:
	static void AddOnce( std::vector<const Shape*>& destination, const Shape& shape );
	void SplitToChildren( Quad& quad ) const;
	static const size_t GetQuarterIndex( const Quad& quad, const Point& point );
	static BoundingRect GetQuarterBounds( const Quad& quad, const size_t quarter_index );

	static const bool IsLeaf( const Quad& quad );

private:
	void ReleaseShape( const ShapeProvider::Handle handle, Shape* shape );

	void BuildTree() const;
	void AddPoint( Quad& quad, const Point& point ) const;
	void RemovePoint( Quad& quad, const Point& point ) const;

private:
	static constexpr size_t MAX_POINTS = 4;
	static constexpr size_t MAX_LEVELS = 8;

private:
	ShapeProvider	m_shape_provider;
	BoundingRect	m_bounds{ { 0.0f, 0.0f }, { 0.0f, 0.0f }, std::ignore };

private:
	mutable Points					m_points;
	mutable QuadProvider			m_quad_provider;
	mutable std::shared_ptr<Quad>	m_root;
};

std::shared_ptr<Shape> QuadTree::RetainShape( const BoundingRect& bounds )
{
	auto [ shape, handle ] = m_shape_provider.Create( *this, bounds );
	if( !m_bounds.ConsistsOf( bounds ) )
	{
		m_bounds.Grow( bounds );
		m_root.reset();
	}

	const size_t indices[] { 0, 1, 2, 3 };
	std::transform(
		std::begin( indices ),
		std::end( indices ),
		std::back_inserter( m_points ),
		[&shape = *shape]( const size_t index ) -> const Point*
		{
			return &shape[ index ];
		}
	);

	if( m_root )
	{
		for( const size_t index : indices )
		{
			AddPoint( *m_root, shape->GetPoint( index ) );
		}
	}

	return { shape, [this, handle = handle]( Shape* shape ){ ReleaseShape( handle, shape ); } };
}

std::vector<const Shape*> QuadTree::Find( const BoundingRect& bounds ) const
{
	std::vector<const Shape*> result;
	if( !m_root )
	{
		BuildTree();
	}

	std::queue<const Quad*> pending_quads;
	pending_quads.push( m_root.get() );
	while( !pending_quads.empty() )
	{
		const Quad& quad = *pending_quads.front();
		pending_quads.pop();

		if( IsLeaf( quad ) )
		{
			for( const auto point : std::get<Points>( quad.state ) )
			{
				if( bounds.ConsistsOf( point->GetPosition() ) )
				{
					AddOnce( result, *point->GetHost() );
				}
			}
		}
		else
		{
			for( const auto& quad_slot : std::get<Quads>( quad.state ) )
			{
				if( quad_slot && bounds.IsIntersects( quad_slot->bounds ) )
				{
					pending_quads.push( quad_slot.get() );
				}
			}
		}
	}

	return result;
}

std::vector<const Shape*> QuadTree::Find( const Vector2f& center, const float radius ) const
{
	std::vector<const Shape*> result;

	BoundingRect bounds{ BoundingRect{ center }.Resize( radius ) };
	result = Find( bounds );

	auto new_result_end = std::remove_if(
		result.begin(),
		result.end(),
		[&center, radius]( const Shape* shape ) -> const bool
		{
			return shape->GetBounds().IsIntersects( center, radius );
		}
	);

	if( new_result_end != result.end() )
	{
		result.erase( new_result_end, result.end() );
	}

	return result;
}

void QuadTree::AddOnce( std::vector<const Shape*>& destination, const Shape& shape )
{
	auto found_slot = std::upper_bound( destination.begin(), destination.end(), &shape );
	if( ( found_slot == destination.begin() ) || ( *std::prev( found_slot ) != &shape ) )
	{
		destination.insert( found_slot, &shape );
	}
}

void QuadTree::SplitToChildren( Quad& quad ) const
{
	const Points points{ std::move( std::get<Points>( quad.state ) ) };

	quad.state.emplace<Quads>();
	for( const auto& point : points )
	{
		AddPoint( quad, *point );
	}
}

const size_t QuadTree::GetQuarterIndex( const Quad& quad, const Point& point )
{
	return quad.bounds.GetNearestCornerIndex( point.GetPosition() );
}

BoundingRect QuadTree::GetQuarterBounds( const Quad& quad, const size_t quarter_index )
{
	constexpr std::pair<size_t, size_t> quarter_min[] { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
	constexpr std::pair<size_t, size_t> quarter_max[] { { 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 } };

	const Vector2f corners[] { quad.bounds.min, quad.center, quad.bounds.max };

	const auto [ min_x, min_y ] = quarter_min[ quarter_index ];
	const auto [ max_x, max_y ] = quarter_max[ quarter_index ];
	return { { corners[ min_x ].x, corners[ min_y ].y }, { corners[ max_x ].x, corners[ max_y ].y }, std::ignore };
}

const bool QuadTree::IsLeaf( const Quad& quad )
{
	return std::holds_alternative<Points>( quad.state );
}

void QuadTree::ReleaseShape( const ShapeProvider::Handle handle, Shape* shape )
{
	if( !shape || ( shape != m_shape_provider.Query( handle ) ) )
	{
		return;
	}

	for( auto& point : m_points )
	{
		if( point && ( point->GetHost() == shape ) )
		{
			point = nullptr;
		}
	}

	if( m_root )
	{
		constexpr size_t indices[] { 0, 1, 2, 3 };
		for( const size_t index : indices )
		{
			RemovePoint( *m_root, shape->GetPoint( index ) );
		}
	}

	m_shape_provider.Destroy( handle );
}

void QuadTree::BuildTree() const
{
	m_root = m_quad_provider.Create( m_bounds, 1 );

	auto valid_end = m_points.end();
	for( auto current = m_points.begin(); current != valid_end; ++current )
	{
		if( !(*current) )
		{
			std::swap( *current, *( --valid_end ) );
		}

		AddPoint( *m_root, *(*current) );
	}

	if( valid_end == m_points.end() )
	{
		return;
	}

	m_points.erase( valid_end, m_points.end() );
}

void QuadTree::AddPoint( Quad& quad, const Point& point ) const
{
	if( IsLeaf( quad ) )
	{
		auto& points = std::get<Points>( quad.state );
		if( ( points.size() < MAX_POINTS ) || ( quad.level == MAX_LEVELS ) )
		{
			points.push_back( &point );
			return;
		}

		SplitToChildren( quad );
	}

	const size_t quarter_index = GetQuarterIndex( quad, point );
	auto& quad_quarters = std::get<Quads>( quad.state );
	auto& quarter = quad_quarters[ quarter_index ];
	if( !quarter )
	{
		quarter = m_quad_provider.Create( GetQuarterBounds( quad, quarter_index ), quad.level + 1 );
	}

	AddPoint( *quarter, point );
}

void QuadTree::RemovePoint( Quad& quad, const Point& point ) const
{
	if( IsLeaf( quad ) )
	{
		auto& points = std::get<Points>( quad.state );
		points.erase( std::find( points.begin(), points.end(), &point ) );
		return;
	}

	const size_t quarter_index = GetQuarterIndex( quad, point );
	auto& quad_quarters = std::get<Quads>( quad.state );
	auto& quarter = quad_quarters[ quarter_index ];
	if( quarter )
	{
		RemovePoint( *quarter, point );
	}

	if( IsLeaf( *quarter ) )
	{
		if( std::get<Points>( quarter->state ).empty() )
		{
			quarter.reset();
		}
	}
	else
	{
		const auto& quarters = std::get<Quads>( quarter->state );
		if( std::all_of( quarters.begin(), quarters.end(), []( const std::shared_ptr<Quad>& slot ) { return !slot; } ) )
		{
			quarter.reset();
		}
	}
}


int main()
{
	const size_t shapes_count = 105;

	QuadTree tree;

	std::vector<std::shared_ptr<Shape>> shapes;
	shapes.reserve( shapes_count );

	{
		std::minstd_rand						randomizer{ std::random_device{}() };
		std::uniform_real_distribution<double>	random_distribution{ 0.0, 1.0 };

		for( size_t index = 0; index < shapes_count; ++index )
		{
			const Vector2f center{
				float( 10.0 * ( random_distribution( randomizer ) - 0.5 ) ),
				float( 10.0 * ( random_distribution( randomizer ) - 0.5 ) )
			};
			const Vector2f size{
				float( 6.0 * random_distribution( randomizer ) ),
				float( 6.0 * random_distribution( randomizer ) )
			};

			shapes.emplace_back( tree.RetainShape( BoundingRect{ center }.Resize( size.Maximized( { 1.0f, 1.0f } ) * 0.5f ) ) );
			shapes.back()->SetTag( index );
		}
	}

	{
		auto matches = tree.Find( { { 2.5f, 2.5f }, { 3.95f, 4.8f }, std::ignore } );
	}

	auto new_end = std::remove_if( shapes.begin(), shapes.end(), []( const std::shared_ptr<Shape>& shape ) { return ( shape->GetTag() & 1 ) != 0; } );
	shapes.erase( new_end, shapes.end() );

	{
		auto matches = tree.Find( tree.GetBounds() );
		for( const auto& shape : matches )
		{
			const bool is_valid = ( shape->GetTag() & 1 ) == 0;
		}
	}

	return 0;
}
