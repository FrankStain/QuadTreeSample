#pragma once

namespace Demo
{
inline namespace Spatial
{
namespace Internal
{
	class Point final
	{
	public:
		inline Point() noexcept		= default;
		inline ~Point() noexcept	= default;

		inline Point( Shape& host, const Vector2f& position ) noexcept	: m_host{ &host }, m_position{ position } {};

	public:
		inline void SetPosition( const Vector2f& position )		{ m_position = position; };

		inline const Vector2f& GetPosition() const				{ return m_position; };

		inline const Shape* GetHost() const						{ return m_host; };

	private:
		Shape*		m_host		= nullptr;
		Vector2f	m_position;
	};
}
}
}
