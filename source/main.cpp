#include <main.h>
#include <demo/math/math.h>
#include <demo/spatial/spatial.h>


int main()
{
	const size_t shapes_count = 105;

	Demo::QuadTree tree;

	std::vector<std::shared_ptr<Demo::QuadTree::Shape>> shapes;
	shapes.reserve( shapes_count );

	{
		std::minstd_rand						randomizer{ std::random_device{}() };
		std::uniform_real_distribution<double>	random_distribution{ 0.0, 1.0 };

		for( size_t index = 0; index < shapes_count; ++index )
		{
			const Demo::Vector2f center{
				float( 10.0 * ( random_distribution( randomizer ) - 0.5 ) ),
				float( 10.0 * ( random_distribution( randomizer ) - 0.5 ) )
			};
			const Demo::Vector2f size{
				float( 6.0 * random_distribution( randomizer ) ),
				float( 6.0 * random_distribution( randomizer ) )
			};

			shapes.emplace_back( tree.Acquire( Demo::BoundingRect{ center }.Resize( size.Maximized( { 1.0f, 1.0f } ) * 0.5f ) ) );
			shapes.back()->SetTag( index );
		}
	}

	{
		auto matches = tree.Find( { { 2.5f, 2.5f }, { 3.95f, 4.8f }, std::ignore } );
	}

	auto new_end = std::remove_if(
		shapes.begin(),
		shapes.end(),
		[]( const std::shared_ptr<Demo::QuadTree::Shape>& shape )
		{
			return ( shape->GetTag() & 1 ) != 0;
		}
	);

	shapes.erase( new_end, shapes.end() );

	{
		auto matches = tree.Find( tree.GetBounds() );
		for( const auto& shape : matches )
		{
			const bool is_valid = ( shape->GetTag() & 1 ) == 0;
		}
	}

	return 0;
}
