#pragma once


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Provider of shape instances.

		Provider represents the memory pool abstraction. Shapes are stored in slots of type `std::optional`.
		Slots are packed in buckets with length of `BUCKET_LENGTH`, obviously. Buckets are created dynamically and owned by `std::vector` via `std::unique_ptr`.
		All of it forms the 2D grid of shape slots and each shape can be indexed through the 2D point in integer space.
		Such 2D point is packed in value of inner type `Handle`. Each created shape supplied with handle since the destruction of shape is allowed only by handle.
	*/
	class ShapeProvider final
	{
	// Public constants.
	public:
		// Length of single bucket.
		static constexpr size_t BUCKET_LENGTH = 8;

	// Public inner types.
	public:
		// Handle of create shape.
		enum class Handle : size_t;


		// Bucket of shape slots.
		using Bucket = std::array<std::optional<Shape>, BUCKET_LENGTH>;

		// Storage of shape buckets.
		using BucketStorage = std::vector<std::unique_ptr<Bucket>>;

	// Public interface.
	public:
		// Create the new shape.
		std::pair<Shape*, Handle> Create( QuadTree& host, const BoundingRect& bounds );

		// Destroy the previously created shape by given handle.
		void Destroy( const Handle handle );

	// Internal state.
	private:
		BucketStorage m_slots; // Storage for shape slots.
	};
}
}
}
