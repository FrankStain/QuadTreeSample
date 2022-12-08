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
		static constexpr size_t BUCKET_LENGTH = 8;

	public:
		using Bucket = std::array<std::optional<Shape>, BUCKET_LENGTH>;
		using BucketStorage = std::vector<std::unique_ptr<Bucket>>;

		enum class Handle : size_t;

	public:
		std::pair<Shape*, Handle> Create( QuadTree& host, const BoundingRect& bounds );
		void Destroy( const Handle handle );

		Shape* Query( const Handle handle ) const;

	private:
		BucketStorage m_slots;
	};
}
}
}
