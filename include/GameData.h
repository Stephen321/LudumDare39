#ifndef GAMEDATA_H
#define GAMEDATA_H
#include "SFML\Graphics\Texture.hpp"
#include "SFML\Graphics\Font.hpp"

class GameData {
public:
	static GameData& getInstance();
	GameData(GameData const&) = delete;             
	GameData(GameData&&) = delete;                  
	GameData& operator=(GameData const&) = delete;
	GameData& operator=(GameData &&) = delete;
public:
	sf::Texture playerTexture;
	sf::Texture bulletTexture;
	sf::Texture prisonerTexture;
	sf::Texture backgroundTexture;

	//cell doors
	sf::Texture topbotcellTex;
	sf::Texture right3cellTex;
	sf::Texture right4cellTex;
	sf::Texture switchPadTexture;
	sf::Texture uiPanelTexture;
	sf::Texture menuTexture;
	sf::Texture gameOverTexture;
	
	int levelReached;
	sf::Font levelFont;
	float textureScaler;
	unsigned int WINDOW_WIDTH = 1280u; //TODO: come back to this
	unsigned int WINDOW_HEIGHT = 704u;
private:
	GameData();
};
#endif