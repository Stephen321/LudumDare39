#pragma once

#include "Screen.h"
#include "GameLoader.h"
#include "BulletPool.h"
#include "PrisonManager.h"
#include "AudioManager.h"

class GameScreen : public Screen {
public:
	Screen::Type run(sf::RenderWindow &window) override;

private:
	void checkCollisions(std::vector<Bullet>& bullets, std::vector<std::unique_ptr<Prisoner>>& prisoners, AudioManager& audioManager);
	bool restrictToBounds(GameObject& object, float boundsSize, const sf::Vector2u& windowSize);
	bool circleCollision(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance);
};