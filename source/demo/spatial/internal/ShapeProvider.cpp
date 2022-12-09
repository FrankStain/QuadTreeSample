#include <demo/spatial/spatial.h>


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
namespace
{
	const ShapeProvider::Handle ToHandle( const size_t bucket_index, const size_t slot_index )
	{
		return ShapeProvider::Handle{ bucket_index * ShapeProvider::BUCKET_LENGTH + slot_index };
	}

	std::pair<size_t, size_t> FromHandle( const ShapeProvider::Handle handle )
	{
		return { size_t( handle ) / ShapeProvider::BUCKET_LENGTH, size_t( handle ) % ShapeProvider::BUCKET_LENGTH };
	}

	const bool HasFreeSlot( const ShapeProvider::Bucket& bucket )
	{
		return std::any_of( bucket.begin(), bucket.end(), []( const std::optional<Shape>& slot ) { return !slot.has_value(); } );
	}

	std::optional<Shape>& GetSlot( const ShapeProvider::BucketStorage& storage, const ShapeProvider::Handle handle )
	{
		auto [ bucket_index, slot_index ] = FromHandle( handle );
		return storage.at( bucket_index )->at( slot_index );
	}
}


	std::pair<Shape*, ShapeProvider::Handle> ShapeProvider::Create( QuadTree& host, const BoundingRect& bounds )
	{
		auto suitable_bucket = std::find_if( m_slots.begin(), m_slots.end(), []( const std::unique_ptr<Bucket>& bucket ) { return HasFreeSlot( *bucket ); } );
		if( suitable_bucket == m_slots.end() )
		{
			m_slots.emplace_back( std::make_unique<Bucket>() );
			suitable_bucket = std::prev( m_slots.end() );
		}

		Bucket& bucket = *suitable_bucket->get();
		auto suitable_slot = std::find_if( bucket.begin(), bucket.end(), []( const std::optional<Shape>& slot ) { return !slot.has_value(); } );

		Shape& shape = suitable_slot->emplace( host, bounds );
		return { &shape, ToHandle( std::distance( m_slots.begin(), suitable_bucket ), std::distance( bucket.begin(), suitable_slot ) ) };
	}

	void ShapeProvider::Destroy( const Handle handle )
	{
		GetSlot( m_slots, handle ).reset();
	}
}
}
}
