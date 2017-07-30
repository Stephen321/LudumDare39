#pragma once

#include "Prisoner.h"
#include <unordered_map>
#include "Bullet.h"
#include <memory>

class PrisonManager : public sf::Drawable {
public:
	PrisonManager(const sf::Vector2u& windowSize);
	int update(float dt, const sf::Vector2f& playerPosition, const sf::Vector2f& playerSize); //returns number of collisions
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void decreasePower();
	std::vector<std::unique_ptr<Prisoner>>& getPrisoners();
	void newLevel();
	int getPrisonersRemaining() const;
	int getMaxPrisoners() const;

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
		R1,
		R2,
		R3,
		None
	};
	struct SpawnLoc {
		bool active = false;
		int remaining;
		sf::Vector2f position;
		sf::Vector2f wayPointPosition;
		sf::Sprite sprite;
	};
	void spawnPrisoner(Location location);
	void createSpawnLocs();

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

	int m_power;

	const float LeftXPos = 0.013;
	const float LeftYOffset1 = 0.159;
	const float LeftYOffset2 = 0.341;
	const float LeftYOffset3 = 0.341;

	const float TopXOffset1 = 0.225;
	const float TopXOffset2 = 0.275;
	const float TopXOffset3 = 0.275;
	const float TopYPos = 0.023;

	const float BotXOffset1 = 0.225;
	const float BotXOffset2 = 0.275;
	const float BotXOffset3 = 0.275;
	const float BotYPos = 0.978;

	const int MaxLevel = 10; //after this level there is no increase in spawn rate, just numbers
	//consts are by what value they change per level 
	const float MaxPwrSpawnRate = 0.3f; 
	const int MaxPwrAmount = 1;
	float m_maxPwrSpawnRate;
	float m_maxPwrAmount;
	
	const float HighPwrSpawnRate = 0.25f;
	const int HighPwrAmount = 2;
	float m_highPwrSpawnRate;
	float m_highPwrAmount;

	const float MediumPwrSpawnRate = 0.15f;
	const int MediumPwrAmount = 3;
	float m_mediumPwrSpawnRate;
	float m_mediumPwrAmount;

	const float LowPwrSpawnRate = 0.1f;
	const int LowPwrAmount = 4;
	float m_lowPwrSpawnRate;
	float m_lowPwrAmount;

	std::unordered_map<Location, SpawnLoc> m_spawns;

	int m_maxPrisoners;
	int m_prisonersRemaining;
	std::vector<std::unique_ptr<Prisoner>> m_prisoners;
	float m_timer;
	float m_spawnTimer; 
	float m_spawnRate;
};
