#include "PrisonerManager.h"
#include "GameData.h"
#include "Helpers.h"
#include <iostream>

PrisonerManager::PrisonerManager(const sf::Vector2u& windowSize) :
	m_windowSize(windowSize)
	, m_timer(0.f)
	, m_spawnTimer(0.f)
	, m_spawnRate(MaxPwrSpawnRate)
	, m_power(MAX_POWER){
	createSpawnLocs();
}

int PrisonerManager::update(float dt, const sf::Vector2f& playerPosition, const sf::Vector2f& playerSize) {
	int collisions = 0;
	m_spawnTimer += dt;
	m_timer += dt;
	if (m_spawnTimer > m_spawnRate) {
		std::vector<Location> possibleLocations;
		for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
			if (it->second.active) {
				possibleLocations.push_back(it->first);
			}
		}
		if (possibleLocations.size() > 0) {
			spawnPrisoner(possibleLocations[rand() % possibleLocations.size()]);
			m_spawnTimer = 0.f;
		}
		else {
			std::cout << "no possible locations to spawn." << std::endl;
		}
	}

	for (auto it = m_prisoners.begin(); it != m_prisoners.end();) {
		it->update(dt, playerPosition, playerSize);
		if (it->getCollided())
			it = m_prisoners.erase(it);
		else
			++it;
	}
	return collisions;
}

void PrisonerManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	//debug
	for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
		sf::CircleShape c(4);
		c.setFillColor(sf::Color::Red);
		c.setPosition((*it).second.position);
		target.draw(c);
	}
	for (int i = 0; i < m_prisoners.size(); i++) {
		target.draw(m_prisoners[i]);
	}
}

void PrisonerManager::decreasePower() {
	m_power--;
	if (m_power < 0) {//TODO: or all prisoners dead?
		//TODO: start next stage/game over
	}
	else if (m_power == MAX_POWER - 1) {
		//TODO: enable other locations
		m_spawnRate = HighPwrSpawnRate;
		m_spawns[Location::T1].active = true;
		m_spawns[Location::B1].active = true;
	}
	else if (m_power == MAX_POWER - 2) {
		m_spawnRate = MediumPwrSpawnRate;
		m_spawns[Location::T2].active = true;
		m_spawns[Location::B2].active = true;
	}
	else if (m_power == MAX_POWER - 3) {
		m_spawnRate = LowPwrSpawnRate;
		m_spawns[Location::T3].active = true;
		m_spawns[Location::B3].active = true;
	}
}


void PrisonerManager::spawnPrisoner(Location location) {
	SpawnLoc& spawnLoc = m_spawns[location];
	if (spawnLoc.remaining > 0) {
		spawnLoc.remaining--;
		if (spawnLoc.remaining == 0) {
			spawnLoc.active = false;
		}
		m_prisoners.push_back(Prisoner(spawnLoc.position));
	}
	else {
		std::cout << "not enough time has elapsed for this spawn location" << std::endl;
	}
}

void PrisonerManager::createSpawnLocs() {

	//TODO: function?
	m_spawns[Location::L1].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L1].position.y = LeftYOffset1 * m_windowSize.y;
	m_spawns[Location::L1].remaining = MaxPwrAmount;
	m_spawns[Location::L1].active = true;
	
	m_spawns[Location::L2].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L2].position.y = m_spawns[Location::L1].position.y + (LeftYOffset2 * m_windowSize.y);
	m_spawns[Location::L2].remaining = MaxPwrAmount;
	m_spawns[Location::L2].active = true;

	m_spawns[Location::L3].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L3].position.y = m_spawns[Location::L2].position.y + (LeftYOffset3 * m_windowSize.y);
	m_spawns[Location::L3].remaining = MaxPwrAmount;
	m_spawns[Location::L3].active = true;

	m_spawns[Location::T1].position.x = TopXOffset1 * m_windowSize.x;
	m_spawns[Location::T1].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T1].remaining = HighPwrAmount;

	m_spawns[Location::T2].position.x = m_spawns[Location::T1].position.x + (TopXOffset2 * m_windowSize.x);
	m_spawns[Location::T2].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T2].remaining = MediumPwrAmount;

	m_spawns[Location::T3].position.x = m_spawns[Location::T2].position.x + (TopXOffset3 * m_windowSize.x);
	m_spawns[Location::T3].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T3].remaining = LowPwrAmount;

	m_spawns[Location::B1].position.x = TopXOffset1 * m_windowSize.x;
	m_spawns[Location::B1].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B1].remaining = HighPwrAmount;

	m_spawns[Location::B2].position.x = m_spawns[Location::B1].position.x + (TopXOffset2 * m_windowSize.x);
	m_spawns[Location::B2].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B2].remaining = MediumPwrAmount;

	m_spawns[Location::B3].position.x = m_spawns[Location::B2].position.x + (TopXOffset3 * m_windowSize.x);
	m_spawns[Location::B3].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B3].remaining = LowPwrAmount;
}

void PrisonerManager::checkCollisions(std::vector<Bullet>& bulletPool) {
}