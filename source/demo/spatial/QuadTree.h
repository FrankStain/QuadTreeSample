#pragma once


namespace Demo
{
inline namespace Spatial
{
	class QuadTree final
	{
	public:
		friend class Internal::Shape;

	public:
		std::shared_ptr<Internal::Shape> RetainShape( const BoundingRect& bounds );

		std::vector<const Internal::Shape*> Find( const BoundingRect& bounds ) const;
		std::vector<const Internal::Shape*> Find( const Vector2f& center, const float radius ) const;

		const BoundingRect& GetBounds() const { return m_bounds; };

	private:
		static void AddOnce( std::vector<const Internal::Shape*>& destination, const Internal::Shape& shape );
		void SplitToChildren( Internal::Quad& quad ) const;
		static const size_t GetQuarterIndex( const Internal::Quad& quad, const Internal::Point& point );
		static BoundingRect GetQuarterBounds( const Internal::Quad& quad, const size_t quarter_index );

		static const bool IsLeaf( const Internal::Quad& quad );

	private:
		void ReleaseShape( const Internal::ShapeProvider::Handle handle, Internal::Shape* shape );

		void BuildTree() const;
		void AddPoint( Internal::Quad& quad, const Internal::Point& point ) const;
		void RemovePoint( Internal::Quad& quad, const Internal::Point& point ) const;

	private:
		static constexpr size_t MAX_POINTS = 4;
		static constexpr size_t MAX_LEVELS = 8;

	private:
		Internal::ShapeProvider	m_shape_provider;
		BoundingRect			m_bounds{ { 0.0f, 0.0f }, { 0.0f, 0.0f }, std::ignore };

	private:
		mutable Internal::Points				m_points;
		mutable Internal::QuadProvider			m_quad_provider;
		mutable std::shared_ptr<Internal::Quad>	m_root;
	};
}
}
