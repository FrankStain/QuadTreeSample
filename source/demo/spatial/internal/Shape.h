#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
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
}
}
}
