#pragma once

#include "Bullet.h"
#include "AudioManager.h"

class BulletPool : public sf::Drawable {
public:
	BulletPool(int size, AudioManager& audioManager);
	bool fire(const sf::Vector2f& start, const sf::Vector2f& direction);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::vector<Bullet>& getBullets();
	void reset();

private:
	int m_size;
	std::vector<Bullet> m_bullets;
	AudioManager& m_audioManager;
};