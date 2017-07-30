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


	ptr.levelFont.loadFromFile("assets/fonts/Prototype.ttf"); 
}