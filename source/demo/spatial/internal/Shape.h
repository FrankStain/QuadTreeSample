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
		inline ~Shape() noexcept = default;

		Shape( QuadTree& host, const BoundingRect& bounds ) noexcept;

	public:
		void SetBounds( const BoundingRect& bounds );

		inline void SetTag( const size_t value )						{ m_tag = value; };


		inline const BoundingRect& GetBounds() const					{ return m_bounds; };
		inline const Point& GetPoint( const size_t index ) const		{ return m_points[ index ]; };
		inline const size_t GetTag() const								{ return m_tag; };


		inline const Point& operator [] ( const size_t index ) const	{ return GetPoint( index ); };

	private:
		QuadTree*		m_host		= nullptr;
		BoundingRect	m_bounds;
		Point			m_points[4];

		size_t			m_tag		= 0;
	};
}
}
}
