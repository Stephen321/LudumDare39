#include "PrisonManager.h"
#include "GameData.h"
#include "Helpers.h"
#include <iostream>
#include "Constants.h"

PrisonManager::PrisonManager(const sf::Vector2u& windowSize) :
	m_windowSize(windowSize)
	, m_timer(0.f)
	, m_spawnTimer(0.f)
	, m_spawnRate(MaxPwrSpawnRate)
	, m_power(MAX_POWER){
	createSpawnLocs();
}

int PrisonManager::update(float dt, const sf::Vector2f& playerPosition, const sf::Vector2f& playerSize) {
	int collisions = 0;
	m_spawnTimer += dt;
	m_timer += dt;
	if (m_spawnTimer > m_spawnRate) {
		std::vector<Location> possibleLocations;
		for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
			//if its actively spawning prisoners and make sure its not the right gates as they may be open but we dont need to try spawn there
			if (it->second.active && (it->first != Location::R1 || it->first != Location::R2 || it->first != Location::R3)) {
				possibleLocations.push_back(it->first);
			}
		}
		if (possibleLocations.size() > 0) {
			spawnPrisoner(possibleLocations[rand() % possibleLocations.size()]);
			m_spawnTimer = 0.f;
		}
	}

	for (auto it = m_prisoners.begin(); it != m_prisoners.end(); ++it) {
		(*it)->update(dt, playerPosition);
		if ((*it)->getCollided(playerPosition, playerSize)) 
			collisions++;
	}
	for (auto it = m_prisoners.begin(); it != m_prisoners.end();) {
		if ((*it)->getActive() == false) {
			it = m_prisoners.erase(it);
			m_prisonersRemaining--;
		}
		else
			++it;
	}
	return collisions;
}

void PrisonManager::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	//debug
	for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
		sf::CircleShape c(4);
		c.setFillColor(sf::Color::Red);
		c.setPosition((*it).second.position);
		target.draw(c);

		sf::CircleShape c2(4);
		c2.setFillColor(sf::Color::Yellow);
		c2.setPosition((*it).second.wayPointPosition);
		target.draw(c2);
	}

	for (int i = 0; i < m_prisoners.size(); i++) {
		target.draw(*m_prisoners[i]);
	}

	for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
		if (it->second.active == false)
			target.draw(it->second.sprite);
	}
}

void PrisonManager::decreasePower() {
	m_power--;
	if (m_power < 0) {//TODO: or all prisoners dead?
		m_spawns[Location::R1].active = true;
		m_spawns[Location::R2].active = true;
		m_spawns[Location::R3].active = true;
		return;
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

std::vector<std::unique_ptr<Prisoner>>& PrisonManager::getPrisoners() {
	return m_prisoners;
}


void PrisonManager::spawnPrisoner(Location location) {
	SpawnLoc& spawnLoc = m_spawns[location];
	if (spawnLoc.remaining > 0) {
		spawnLoc.remaining--;
		if (spawnLoc.remaining == 0) {
			spawnLoc.active = false;
		}
		m_prisoners.push_back(std::make_unique<Prisoner>(Prisoner(spawnLoc.position, spawnLoc.wayPointPosition)));
	}
	else {
		std::cout << "no remaing prisoners at this location: " << (int)location << std::endl;
	}
}

void PrisonManager::createSpawnLocs() {

	GameData& data = GameData::getInstance();
	m_prisonersRemaining = 0;
	//TODO: function?
	m_prisonersRemaining += MaxPwrAmount + HighPwrAmount + MediumPwrAmount + LowPwrAmount;

	m_spawns[Location::L1].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L1].position.y = LeftYOffset1 * m_windowSize.y;
	m_spawns[Location::L1].remaining = MaxPwrAmount;
	m_spawns[Location::L1].active = true;
	m_spawns[Location::L1].wayPointPosition = sf::Vector2f(1.f, 0.f);
	
	m_spawns[Location::L2].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L2].position.y = m_spawns[Location::L1].position.y + (LeftYOffset2 * m_windowSize.y);
	m_spawns[Location::L2].remaining = MaxPwrAmount;
	m_spawns[Location::L2].active = true;
	m_spawns[Location::L2].wayPointPosition = sf::Vector2f(1.f, 0.f);

	m_spawns[Location::L3].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L3].position.y = m_spawns[Location::L2].position.y + (LeftYOffset3 * m_windowSize.y);
	m_spawns[Location::L3].remaining = MaxPwrAmount;
	m_spawns[Location::L3].active = true;
	m_spawns[Location::L3].wayPointPosition = sf::Vector2f(1.f, 0.f);

	m_spawns[Location::T1].position.x = TopXOffset1 * m_windowSize.x;
	m_spawns[Location::T1].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T1].remaining = HighPwrAmount;
	m_spawns[Location::T1].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::T1].wayPointPosition = sf::Vector2f(0.f, 1.f);

	m_spawns[Location::T2].position.x = m_spawns[Location::T1].position.x + (TopXOffset2 * m_windowSize.x);
	m_spawns[Location::T2].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T2].remaining = MediumPwrAmount;
	m_spawns[Location::T2].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::T2].wayPointPosition = sf::Vector2f(0.f, 1.f);

	m_spawns[Location::T3].position.x = m_spawns[Location::T2].position.x + (TopXOffset3 * m_windowSize.x);
	m_spawns[Location::T3].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T3].remaining = LowPwrAmount;
	m_spawns[Location::T3].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::T3].wayPointPosition = sf::Vector2f(0.f, 1.f);

	m_spawns[Location::B1].position.x = TopXOffset1 * m_windowSize.x;
	m_spawns[Location::B1].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B1].remaining = HighPwrAmount;
	m_spawns[Location::B1].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::B1].wayPointPosition = sf::Vector2f(0.f, -1.f);

	m_spawns[Location::B2].position.x = m_spawns[Location::B1].position.x + (TopXOffset2 * m_windowSize.x);
	m_spawns[Location::B2].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B2].remaining = MediumPwrAmount;
	m_spawns[Location::B2].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::B2].wayPointPosition = sf::Vector2f(0.f, -1.f);

	m_spawns[Location::B3].position.x = m_spawns[Location::B2].position.x + (TopXOffset3 * m_windowSize.x);
	m_spawns[Location::B3].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B3].remaining = LowPwrAmount;
	m_spawns[Location::B3].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::B3].wayPointPosition = sf::Vector2f(0.f, -1.f);


	//putting the right gates in here just to make it easier instead of seperating them
	m_spawns[Location::R1].position.x = m_windowSize.x - (LeftXPos * m_windowSize.x);
	m_spawns[Location::R1].position.y = LeftYOffset1 * m_windowSize.y;
	m_spawns[Location::R1].sprite.setTexture(data.right3cellTex);

	m_spawns[Location::R2].position.x = m_windowSize.x - (LeftXPos * m_windowSize.x);
	m_spawns[Location::R2].position.y = m_spawns[Location::R1].position.y + (LeftYOffset2 * m_windowSize.y);
	m_spawns[Location::R2].sprite.setTexture(data.right4cellTex);

	m_spawns[Location::R3].position.x = m_windowSize.x - (LeftXPos * m_windowSize.x);
	m_spawns[Location::R3].position.y = m_spawns[Location::R2].position.y + (LeftYOffset3 * m_windowSize.y);
	m_spawns[Location::R3].sprite.setTexture(data.right3cellTex);

	for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
		it->second.sprite.setPosition(it->second.position);
		it->second.sprite.setScale(data.textureScaler, data.textureScaler);
		float halfWidth = it->second.sprite.getGlobalBounds().width * 0.5f;
		float halfHeight = it->second.sprite.getGlobalBounds().height * 0.5f;
		it->second.sprite.setOrigin(halfWidth, halfHeight);
		it->second.wayPointPosition = it->second.position + (it->second.wayPointPosition * (TILE_SIZE * m_windowSize.x * 1.2f));
	}
}