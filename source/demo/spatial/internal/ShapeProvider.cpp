#include <demo/spatial/spatial.h>


namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	std::pair<Shape*, ShapeProvider::Handle> ShapeProvider::Create( QuadTree& host, const BoundingRect& bounds )
	{
		auto suitable_bucket = std::find_if( m_slots.begin(), m_slots.end(), []( const std::unique_ptr<Bucket>& bucket ) { return HasSlot( *bucket ); } );
		if( suitable_bucket == m_slots.end() )
		{
			m_slots.emplace_back( std::make_unique<Bucket>() );
			suitable_bucket = std::prev( m_slots.end() );
		}

		Bucket& bucket = *suitable_bucket->get();
		auto suitable_slot = std::find_if( bucket.begin(), bucket.end(), []( const std::optional<Shape>& slot ) { return !slot.has_value(); } );

		Shape& shape = suitable_slot->emplace( host, bounds );
		return { &shape, Handle{ std::distance( m_slots.begin(), suitable_bucket ) * BUCKET_LENGTH + std::distance( bucket.begin(), suitable_slot ) } };
	}

	void ShapeProvider::Destroy( const Handle handle )
	{
		GetSlot( handle ).reset();
	}

	Shape* ShapeProvider::Query( const Handle handle ) const
	{
		auto& slot = GetSlot( handle );
		return ( slot.has_value() )? &slot.value() : nullptr;
	}

	const bool ShapeProvider::HasSlot( const Bucket& bucket )
	{
		return std::any_of( bucket.begin(), bucket.end(), []( const std::optional<Shape>& slot ) { return !slot.has_value(); } );
	}

	std::optional<Shape>& ShapeProvider::GetSlot( const Handle handle ) const
	{
		const size_t bucket_index	= size_t( handle ) / BUCKET_LENGTH;
		const size_t slot_index		= size_t( handle ) % BUCKET_LENGTH;

		return m_slots.at( bucket_index )->at( slot_index );
	}
}
}
}
