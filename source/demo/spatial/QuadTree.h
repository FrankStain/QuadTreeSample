#pragma once


namespace Demo
{
inline namespace Spatial
{
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
}
}
