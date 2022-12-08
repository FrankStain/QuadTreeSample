#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
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
}
}
}
