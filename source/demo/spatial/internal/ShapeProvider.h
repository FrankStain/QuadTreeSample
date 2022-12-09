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
		enum class Handle : size_t;

		using Bucket = std::array<std::optional<Shape>, BUCKET_LENGTH>;
		using BucketStorage = std::vector<std::unique_ptr<Bucket>>;

	public:
		std::pair<Shape*, Handle> Create( QuadTree& host, const BoundingRect& bounds );
		void Destroy( const Handle handle );

	private:
		BucketStorage m_slots;
	};
}
}
}
