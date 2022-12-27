#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	class IndexTree final
	{
	// Public constants.
	public:
		// Maximum points indexed by single quad before split it to quarters.
		static constexpr size_t MAX_POINTS = 4;

		// Maximum level of quad tree depth before the quarters splitting will be stopped.
		static constexpr size_t MAX_LEVELS = 8;

	public:
		void Reset();
		void Build( const Demo::BoundingRect& bounds );


		void Push( const Shape& shape );
		void Pop( const Shape& shape );

		std::vector<const Shape*> Find( const Demo::BoundingRect& bounds ) const;


		inline const bool IsEmpty() const	{ return m_root == nullptr; };
		inline const bool IsBuilt() const	{ return m_root != nullptr; };

	private:
		void ReindexShape( Quad& quad, const Shape& shape );
		void SplitToQuarters( Quad& quad );

	private:
		Shapes					m_shapes;
		QuadProvider			m_quad_provider;
		std::shared_ptr<Quad>	m_root;
	};
}
}
}
