
#ifndef _IRENDERABLE_H_
#define _IRENDERABLE_H_

namespace sf
{
	class RenderTarget;
}

class IRenderable
{
public:
	virtual ~IRenderable() {}

	virtual void Render(const sf::RenderTarget& rt) = 0;
};

#endif // !_IRENDERABLE_H_