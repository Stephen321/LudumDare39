#pragma once

#include "Bullet.h"

class BulletPool : public sf::Drawable {
public:
	BulletPool(int size);
	bool fire(const sf::Vector2f& start, const sf::Vector2f& direction);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::vector<Bullet>& getBullets();
	void reset();

private:
	int m_size;
	std::vector<Bullet> m_bullets;
};