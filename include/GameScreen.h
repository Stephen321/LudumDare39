#pragma once

#include "Screen.h"
#include "GameLoader.h"
#include "BulletPool.h"
#include "PrisonManager.h"
#include "Thor/Particles.hpp"

class AudioManager;
class BloodManager;
class GameScreen : public Screen {
public:
	Screen::Type run(sf::RenderWindow &window) override;

private:
	thor::ParticleSystem pSystem;
	thor::UniversalEmitter bulletHitEmitter;
private:
	void checkCollisions(std::vector<Bullet>& bullets, std::vector<std::unique_ptr<Prisoner>>& prisoners, AudioManager& audioManager, BloodManager& bloodManager);
	bool restrictToBounds(GameObject& object, float boundsSize, const sf::Vector2u& windowSize);
	bool circleCollision(const sf::Vector2f& pos1, const sf::Vector2f& pos2, float minDistance);
	void createBulletEmitter(std::vector<Bullet>& bullets, int i);
	sf::Vector2f getRectCentre(const sf::IntRect& rect);
	void addShadowPolys(sf::VertexArray& vertexArray, const std::vector<std::pair<sf::Vector2f, float>>& centres, float radius, const std::pair<bool,sf::IntRect> light);
	bool circleRectCollision(std::pair<sf::Vector2f, float> circle, sf::IntRect rect);
	sf::Vector2f rotateVector(const sf::Vector2f& v, float degrees);
};