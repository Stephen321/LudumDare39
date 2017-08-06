#include "GameLoader.h"
#include "SFML/Graphics.hpp" 
#include "GameData.h"
#include <memory>
#include <fstream>

GameLoader::GameLoader(const std::string& filePath) {
	loadData(filePath);
}

void GameLoader::loadJSONDATA(std::string const & filename) {
	std::ifstream file(filename);
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			m_JSONData.append(line);
		}
	}
}

void GameLoader::loadData(const std::string& filePath) {
	GameData& ptr = GameData::getInstance();
	m_JSONData.clear();
	loadJSONDATA(filePath + "data.json");
	m_document.Parse<0>(m_JSONData.c_str());

	Value::ConstMemberIterator it = m_document.MemberBegin();
	Value::ConstMemberIterator spritesIt = it->value.GetObject().MemberBegin();
	++it;
	std::string spritesPath = filePath + spritesIt->value.GetString();
	++spritesIt;
	ptr.playerTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.bulletTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.backgroundTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.prisonerTexture.loadFromFile(spritesPath + spritesIt->value.GetString());

	++spritesIt;
	ptr.topbotcellTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.right3cellTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.right4cellTex.loadFromFile(spritesPath + spritesIt->value.GetString());

	++spritesIt;
	ptr.switchPadTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.uiPanelTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.menuTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.gameOverTexture.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.powerIconTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.healthIconTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.powerOverlayTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	
	++spritesIt;
	ptr.highPwrOverlayTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.mediumPwrOverlayTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.lowPwrOverlayTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.noPwrOverlayTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.prisonerBloodTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.deathBloodTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.particle1Tex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.mouseTex.loadFromFile(spritesPath + spritesIt->value.GetString());


	++spritesIt;
	ptr.lmMax.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.lmHigh.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.lmMed.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.lmLow.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.lmNo.loadFromFile(spritesPath + spritesIt->value.GetString());

	++spritesIt;
	ptr.playerLightTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	++spritesIt;
	ptr.shadowTex.loadFromFile(spritesPath + spritesIt->value.GetString());
	
	Value::ConstMemberIterator audioIT = it->value.GetObject().MemberBegin();
	++it;
	std::string audioPath = filePath + audioIT->value.GetString();
	++audioIT;
	ptr.cellDoorBuffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.death1Buffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.death2Buffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.gunshotBuffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.hit1Buffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.hit2Buffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.hit3Buffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.selectBuffer.loadFromFile(audioPath + audioIT->value.GetString());
	++audioIT;
	ptr.alarmBuffer.loadFromFile(audioPath + audioIT->value.GetString());
	
	
	ptr.levelFont.loadFromFile("assets/fonts/Prototype.ttf"); 
}