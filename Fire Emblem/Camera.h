
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <SFML\Graphics\Transform.hpp>

class Camera
{
public:
	Camera() {}

	void SetRect(sf::FloatRect rect)
	{
		mRect = rect;
	}

	void Move(sf::Vector2f vec)
	{
		mRect.left += vec.x;
		mRect.top += vec.y;
	}

	void MoveTo(sf::Vector2f vec)
	{
		mRect.left = vec.x;
		mRect.top = vec.y;
	}

	sf::Transform GetViewTransform() const
	{
		sf::Transform view;
		return view.translate(-sf::Vector2f(mRect.left, mRect.top));
	}

	sf::FloatRect GetRect() const
	{
		return mRect;
	}

private:
	sf::FloatRect mRect;
};

#endif // !_CAMERA_H_