#pragma once

namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	/**
		@brief	Single point of shape.

		Points are used by quad tree to index the shapes. Only points are indexed by quad tree.
		Each point always hosted by some shape and describes the corner of shape.
	*/
	class Point final
	{
	// Lifetime management.
	public:
		inline Point() noexcept		= default;
		inline ~Point() noexcept	= default;

		inline Point( Shape& host, const Vector2f& position ) noexcept	: m_host{ &host }, m_position{ position } {};

	// Public interface.
	public:
		// Set new position of point.
		void SetPosition( const Vector2f& position );


		// Get current position of point.
		inline const Vector2f& GetPosition() const				{ return m_position; };

		// Get shape that hosts this point.
		inline const Shape* GetHost() const						{ return m_host; };

	// Private state.
	private:
		Shape*		m_host		= nullptr;	// Host shape. Always valid.
		Vector2f	m_position;				// Position of point.
	};
}
}
}
