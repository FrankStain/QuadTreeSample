#pragma once


namespace Demo
{
inline namespace Spatial
{
	class QuadTree final
	{
	public:
		friend class Internal::Shape;

		using Shape = Internal::Shape;

	public:
		static constexpr size_t MAX_POINTS = 4;
		static constexpr size_t MAX_LEVELS = 8;

	public:
		std::shared_ptr<Shape> Acquire( const BoundingRect& bounds );


		std::vector<const Shape*> Find( const BoundingRect& bounds ) const;
		std::vector<const Shape*> Find( const Vector2f& center, const float radius ) const;


		inline const BoundingRect& GetBounds() const	{ return m_bounds; };

	private:
		void ReleaseShape( const Internal::ShapeProvider::Handle handle, Shape* shape );


		void BuildTree() const;
		void AddPoint( Internal::Quad& quad, const Internal::Point& point ) const;
		void SplitToQuarters( Internal::Quad& quad ) const;

	private:
		Internal::ShapeProvider	m_shape_provider;
		BoundingRect			m_bounds{ { 0.0f, 0.0f } };

	private:
		mutable Internal::Points				m_points;
		mutable Internal::QuadProvider			m_quad_provider;
		mutable std::shared_ptr<Internal::Quad>	m_root;
	};
}
}
