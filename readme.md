## Sample implementation of uniform quadtree.


#### TL;DR;

Here I have no tending to optimal code, fastest algorithm or low memory consumption.
Here one can find a small MVP, a modest prototype where I have checked the single function of algorithm.
In this prototype, the operation of the tree is provided by renting shapes + managing the memory and lifetime of shapes by tree itself.
I can’t say that I find this approach completely correct, but this approach also can be used practically.
So I have implemented it.

My final implementation looks a bit different, it have different characteristics and implemented in private part of my [framework](https://github.com/black-io).


#### QuadTree

One can investigate the quadtrees in details on [Wiki](https://en.wikipedia.org/wiki/Quadtree).

Quadtrees are used to implement various kinds of spatial indexes in order to find objects in a certain neighborhood faster and more efficiently.
Such trees allow fast search and discovery in 2D spaces. They are not suitable for efficient search in 3D spaces.


#### What is supported

This code can:

- Allows to "occupy" the area in space, by means the shape. While the shape is being rented, it is available for issuance in the search functions.
- Implements the indexing strategy in nodes. Quadrants can simultaneously store both lists of shapes and links to subtree elements.
- Shapes are directly indexed. The shape is indexed by the quad, where shape fit most tightly the internal space.
- Performs the searching within different spatial area.
- Support the movable shapes. In this case, the shape is re-indexed.
- Dynamically determines the size of the indexing area.

Each rented shape is described by its own bounds (AABR - Axis-Aligned Bounding Rect). This bounds is used for indexing and searching the shape.
The shape interface allows you to set the position of the new bounds. After changing the position the shape is re-indexed or the entire tree is reset.

The quadtree itself is built lazily, on the first search request, if the tree is reset. The tree builds completely.

After the creation, the quadrant market as leaf - it is only the state that can be determined. In this state, the quadrant only stores the shapes until it reaches
the shapes limit. After splitting into a subtree, the quadrant can no longer become a leaf again.
But any quadrant is always able to store shapes that it describes. If the shape fits completely into the bounds of a quadrants's quarter,
the shape is always indexed by a quarter. And only if the shape does not fit into single quarter, it is indexed by the quadrant itself.

Quads and shapes are controlled by providers. Providers in this prototype don't have the goal of efficient memory usage.
Providers here gives only interface. But the behavior of any provider can be changed to more efficient.
This prototype only shows how the algorithm works and provides opportunities for optimization to achieve specific results.


#### Implementation details

In this code, the main is `Demo::QuadTree` class, that implements the quadtree interface.
Inner public type `Demo::QuadTree::Shape` defines the shape interface, and `Demo::QuadTree::SharedShape` can be used to define instances to own the 
rented shape.

Each shape is described by its own bounds of type `Demo::BoundingRect`. This type implements AABR(Axis-Aligned Bounding Rect) semantics,
oriented along the coordinate axes. The bounds always uniquely determines the position of the shape in space.


One can own the shape by calling `Demo::QuadTree::Acquire`. The function returns `Demo::QuadTree::SharedShape` to a new unique shape, so while the shape
is owned, the tree will continue to index it. Shape owning ends when all `Demo::QuadTree::SharedShape` are no longer referenced to it.
The set of owned shapes always defines the bounds of the index. If the shape lies out of bounds of already built tree, the whole tree is reset 
and the indexing bounds is being rebuilt.

Any owned shape can be moved at any time by calling `Demo::QuadTree::Shape::SetBounds`. After the call, the shape will be set to a new
bounds and shape will be re-indexed. Once the shape goes out of indexing bounds, entire tree is reset and the indexing bounds is recalculated.


The `Demo::Spatial::Internal::IndexTree` class implements an index tree. The functionality of this class is used by a quadtree.
The `IndexTree` itself only controls the consistency of the tree, not the consistency of the data indexed. Reseting, rebuilding, and defining the index bounds
should be done by user - i.e. `QuadTree`.
Inside the `IndexTree` all shapes are stored in an intermediate table. This allows to quickly rebuild the index on demand.

On the search request, the index tree is being built. At this moment all known shapes are indexed.
After the tree is built, for each new shape is owned inside of indexing bounds, the tree remains built and only points of new shape are indexed.
After the tree is built, each time the shape is not owned more, only points of such shape removed from indexing safely, the tree remains built.


#### Build and testing

This code is designed in MS Visual Studio 2019. Also this code was tested using Clang 15 and GCC 12.2. No issues was found for this translators.
This code uses C++17 standard and can be considered as cross-platform.

Solution may be found at `./project` folder. The source code itself may be found at `./source` folder.


#### References

- https://ru.wikipedia.org/wiki/%D0%94%D0%B5%D1%80%D0%B5%D0%B2%D0%BE_%D0%BA%D0%B2%D0%B0%D0%B4%D1%80%D0%B0%D0%BD%D1%82%D0%BE%D0%B2
- https://habr.com/ru/post/473066/
- https://gamedev.ru/art/articles/?id=6999
- https://github.com/pvigier/Quadtree
- https://github.com/psimatis/QuadTree


#### License

This code is licensed under the MIT license. Details described in the license file.

Main repository for this code: https://github.com/FrankStain/QuadTreeSample
