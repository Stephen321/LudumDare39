#include "PrisonManager.h"
#include "GameData.h"
#include "Helpers.h"
#include "Constants.h"

PrisonManager::PrisonManager(const sf::Vector2u& windowSize) :
	m_windowSize(windowSize)
	, m_timer(0.f)
	, m_spawnTimer(0.f)
	, m_power(MAX_POWER){
	// example:
	// SpawnRate = 0.3f;
	// Amount = 3;
	// MaxLevel = 10
	//Level	      1,		2,		3,		  4,		5,		6,		 7,		  8,		9,		 [10],		  11
	//Rate:	     3.f	   2.7f     2.4f     2.1f     1.8f     1.5f     1.2f     0.9f     0.6f       0.3f        0.3f
	//Amount:    3		    6		 9		  12	   15		18		 21		  24		27		  30		33

	m_maxPwrSpawnRate = MaxPwrSpawnRate * MaxLevel;
	m_maxPwrAmount = MaxPwrAmount;

	m_highPwrSpawnRate = HighPwrSpawnRate * MaxLevel;
	m_highPwrAmount = HighPwrAmount;

	m_mediumPwrSpawnRate = MediumPwrSpawnRate * MaxLevel;
	m_mediumPwrAmount = MediumPwrAmount;

	m_lowPwrSpawnRate = LowPwrSpawnRate * MaxLevel;
	m_lowPwrAmount = LowPwrAmount;

	m_spawnRate = m_maxPwrAmount;

	createSpawnLocs();
}

int PrisonManager::update(float dt, const sf::Vector2f& playerPosition, const sf::Vector2f& playerSize) {
	int collisions = 0;
	m_spawnTimer += dt;
	m_timer += dt;
	if (m_spawnTimer > m_spawnRate) {
		std::vector<Location> possibleLocations;
		for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
			if (it->second.active && it->second.remaining > 0 && (it->first != Location::R1 && it->first != Location::R2 && it->first != Location::R3)) {
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
	for (int i = 0; i < m_prisoners.size(); i++) {
		target.draw(*m_prisoners[i], states);
	}

	for (auto it = m_spawns.begin(); it != m_spawns.end(); ++it) {
		if (it->second.active == false)
			target.draw(it->second.sprite, states);
	}
}

void PrisonManager::decreasePower() {
	m_power--;
	if (m_power < 0) {
		m_spawns[Location::R1].active = true;
		m_spawns[Location::R2].active = true;
		m_spawns[Location::R3].active = true;
		return;
	}
	else if (m_power == MAX_POWER - 1) {
		m_spawnRate = m_highPwrSpawnRate;
		m_spawns[Location::T1].active = true;
		m_spawns[Location::B1].active = true;
	}
	else if (m_power == MAX_POWER - 2) {
		m_spawnRate = m_mediumPwrSpawnRate;
		m_spawns[Location::T2].active = true;
		m_spawns[Location::B2].active = true;
	}
	else if (m_power == MAX_POWER - 3) {
		m_spawnRate = m_lowPwrSpawnRate;
		m_spawns[Location::T3].active = true;
		m_spawns[Location::B3].active = true;
	}
}

std::vector<std::unique_ptr<Prisoner>>& PrisonManager::getPrisoners() {
	return m_prisoners;
}

void PrisonManager::newLevel() {
	m_timer = 0.f;
	m_spawnTimer = 0.f;
	m_power = MAX_POWER;
	m_prisoners.clear();

	m_maxPwrSpawnRate -= MaxPwrSpawnRate;
	m_maxPwrAmount += MaxPwrAmount;

	m_highPwrSpawnRate -= HighPwrSpawnRate;
	m_highPwrAmount += HighPwrAmount;

	m_mediumPwrSpawnRate -= MediumPwrSpawnRate;
	m_mediumPwrAmount += MediumPwrAmount;

	m_lowPwrSpawnRate -=  LowPwrSpawnRate;
	m_lowPwrAmount += LowPwrAmount;

	m_spawnRate = m_maxPwrAmount;

	createSpawnLocs();
}

int PrisonManager::getPrisonersRemaining() const {
	return m_prisonersRemaining;
}

int PrisonManager::getMaxPrisoners() const {
	return m_maxPrisoners;
}


void PrisonManager::spawnPrisoner(Location location) {
	SpawnLoc& spawnLoc = m_spawns[location];
	if (spawnLoc.remaining > 0) {
		spawnLoc.remaining--;
		m_prisoners.push_back(std::make_unique<Prisoner>(Prisoner(spawnLoc.position, spawnLoc.wayPointPosition)));
	}
}

void PrisonManager::createSpawnLocs() {

	GameData& data = GameData::getInstance();
	m_maxPrisoners = (m_maxPwrAmount * 3) + (m_highPwrAmount * 2) + (m_mediumPwrAmount * 2) + (m_lowPwrAmount * 2);
	m_prisonersRemaining = m_maxPrisoners;

	m_spawns[Location::L1].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L1].position.y = LeftYOffset1 * m_windowSize.y;
	m_spawns[Location::L1].remaining = m_maxPwrAmount;
	m_spawns[Location::L1].wayPointPosition = sf::Vector2f(1.f, 0.f);
	
	m_spawns[Location::L2].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L2].position.y = m_spawns[Location::L1].position.y + (LeftYOffset2 * m_windowSize.y);
	m_spawns[Location::L2].remaining = m_maxPwrAmount;
	m_spawns[Location::L2].wayPointPosition = sf::Vector2f(1.f, 0.f);

	m_spawns[Location::L3].position.x = LeftXPos * m_windowSize.x;
	m_spawns[Location::L3].position.y = m_spawns[Location::L2].position.y + (LeftYOffset3 * m_windowSize.y);
	m_spawns[Location::L3].remaining = m_maxPwrAmount;
	m_spawns[Location::L3].wayPointPosition = sf::Vector2f(1.f, 0.f);

	m_spawns[Location::T1].position.x = TopXOffset1 * m_windowSize.x;
	m_spawns[Location::T1].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T1].remaining = m_highPwrAmount;
	m_spawns[Location::T1].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::T1].wayPointPosition = sf::Vector2f(0.f, 1.f);

	m_spawns[Location::T2].position.x = m_spawns[Location::T1].position.x + (TopXOffset2 * m_windowSize.x);
	m_spawns[Location::T2].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T2].remaining = m_mediumPwrAmount;
	m_spawns[Location::T2].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::T2].wayPointPosition = sf::Vector2f(0.f, 1.f);

	m_spawns[Location::T3].position.x = m_spawns[Location::T2].position.x + (TopXOffset3 * m_windowSize.x);
	m_spawns[Location::T3].position.y = TopYPos * m_windowSize.y;
	m_spawns[Location::T3].remaining = m_lowPwrAmount;
	m_spawns[Location::T3].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::T3].wayPointPosition = sf::Vector2f(0.f, 1.f);

	m_spawns[Location::B1].position.x = TopXOffset1 * m_windowSize.x;
	m_spawns[Location::B1].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B1].remaining = m_highPwrAmount;
	m_spawns[Location::B1].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::B1].wayPointPosition = sf::Vector2f(0.f, -1.f);

	m_spawns[Location::B2].position.x = m_spawns[Location::B1].position.x + (TopXOffset2 * m_windowSize.x);
	m_spawns[Location::B2].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B2].remaining = m_mediumPwrAmount;
	m_spawns[Location::B2].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::B2].wayPointPosition = sf::Vector2f(0.f, -1.f);

	m_spawns[Location::B3].position.x = m_spawns[Location::B2].position.x + (TopXOffset3 * m_windowSize.x);
	m_spawns[Location::B3].position.y = BotYPos * m_windowSize.y;
	m_spawns[Location::B3].remaining = m_lowPwrAmount;
	m_spawns[Location::B3].sprite.setTexture(data.topbotcellTex);
	m_spawns[Location::B3].wayPointPosition = sf::Vector2f(0.f, -1.f);

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
		it->second.active = false;
	}
	m_spawns[Location::L1].active = true;
	m_spawns[Location::L2].active = true;
	m_spawns[Location::L3].active = true;


	//std::cout << "----------------------------------------" << std::endl;
	//std::cout << "\t\t" << "Rate" << "\t" << "Amount" << std::endl;
	//std::cout << "MaxPwr:\t\t " << m_maxPwrSpawnRate << "\t  " << m_maxPwrAmount << std::endl;
	//std::cout << "HighPwr:\t " << m_highPwrSpawnRate << "\t  " << m_highPwrAmount << std::endl;
	//std::cout << "MediumPwr:\t " << m_mediumPwrSpawnRate << "\t  " << m_mediumPwrAmount << std::endl;
	//std::cout << "LowPwr:\t\t " << m_lowPwrSpawnRate << "\t  " << m_lowPwrAmount << std::endl;
	//std::cout << "----------------------------------------" << std::endl;
}