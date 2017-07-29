#pragma once

#include "Prisoner.h"
#include <unordered_map>
#include "Bullet.h"

class PrisonerManager : public sf::Drawable {
public:
	PrisonerManager(const sf::Vector2u& windowSize);
	int update(float dt, const sf::Vector2f& playerPosition, const sf::Vector2f& playerSize); //returns number of collisions
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void decreasePower();
private:
	enum class Location {
		//MaxPwr
		L1 = 0,
		L2 = 1,
		L3 = 2,
		T1 = 3,
		B1 = 4,
		T2 = 5,
		B2 = 6,
		T3 = 7,
		B3 = 8,
		None
	};
	struct SpawnLoc {
		bool active = false;
		int remaining;
		sf::Vector2f position;
	};
	void spawnPrisoner(Location location);
	void createSpawnLocs();
	void checkCollisions(std::vector<Bullet>& bulletPool);
private:
	//Locations:
	//		T1		T2		T3	
	// L1						 exit1	
	//
	// L2						 exit2
	//
	// L3						 exit3
	//		B1		B2		B3

	sf::Vector2u m_windowSize;

	const int MAX_POWER = 3;
	int m_power;

	//TODO: put all this into json file
	const float LeftXPos = 0.012;
	const float LeftYOffset1 = 0.159;
	const float LeftYOffset2 = 0.341;
	const float LeftYOffset3 = 0.341;

	const float TopXOffset1 = 0.225;
	const float TopXOffset2 = 0.275;
	const float TopXOffset3 = 0.275;
	const float TopYPos = 0.022;

	const float BotXOffset1 = 0.225;
	const float BotXOffset2 = 0.275;
	const float BotXOffset3 = 0.275;
	const float BotYPos = 0.978;

	//how many and at what rate to spawn for the different power levels= lower is harder
	const float MaxPwrSpawnRate = 1.f; 
	const int MaxPwrAmount = 10;

	const float HighPwrSpawnRate = 0.8f;
	const int HighPwrAmount = 15;

	const float MediumPwrSpawnRate = 0.6f;
	const int MediumPwrAmount = 20;
	const int MediumPwrSpawnChance = 1;

	const float LowPwrSpawnRate = 0.3f;
	const int LowPwrAmount = 25;

	std::unordered_map<Location, SpawnLoc> m_spawns;
	//TODO: speed up rates or dynamic amounts depending on state of the game

	std::vector<Prisoner> m_prisoners;
	float m_timer;
	float m_spawnTimer; 
	float m_spawnRate;
};
