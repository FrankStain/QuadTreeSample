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
		Point()		= default;
		~Point()	= default;

		Point( Shape& host, const Vector2f& position )	: m_host{ &host }, m_position{ position } {};

	public:
		void SetPosition( const Vector2f& position )	{ m_position = position; };

		const Vector2f& GetPosition() const				{ return m_position; };

		const Shape* GetHost() const					{ return m_host; };

	private:
		Shape*		m_host		= nullptr;
		Vector2f	m_position;
	};
}
}
}
