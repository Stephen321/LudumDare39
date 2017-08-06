#pragma once

#include "GameObject.h"

class BloodManager : public sf::Drawable{
public:
	BloodManager();
	void spawnBlood(const sf::Vector2f& position, float rotation);
	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void reset();
private:
	std::vector<GameObject> m_bloods;
};
