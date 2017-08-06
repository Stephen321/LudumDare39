#pragma once

#include "SFML\Graphics\Texture.hpp"
#include "SFML\Graphics\Font.hpp"
#include "SFML\Audio.hpp"

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
	sf::Texture topbotcellTex;
	sf::Texture right3cellTex;
	sf::Texture right4cellTex;
	sf::Texture switchPadTexture;
	sf::Texture uiPanelTexture;
	sf::Texture menuTexture;
	sf::Texture gameOverTexture;
	sf::Texture powerIconTex;
	sf::Texture healthIconTex;
	sf::Texture powerOverlayTex;
	sf::Texture highPwrOverlayTex;
	sf::Texture mediumPwrOverlayTex;
	sf::Texture lowPwrOverlayTex;
	sf::Texture noPwrOverlayTex;
	sf::Texture prisonerBloodTex;
	sf::Texture deathBloodTex;
	sf::Texture particle1Tex;
	sf::Texture mouseTex;
	//TODO: use array
	sf::Texture lmMax;
	sf::Texture lmHigh;
	sf::Texture lmMed;
	sf::Texture lmLow;
	sf::Texture lmNo;
	sf::Texture playerLightTex;
	sf::Texture shadowTex;

	sf::SoundBuffer cellDoorBuffer;
	sf::SoundBuffer death1Buffer;
	sf::SoundBuffer death2Buffer;
	sf::SoundBuffer gunshotBuffer;
	sf::SoundBuffer hit1Buffer;
	sf::SoundBuffer hit2Buffer;
	sf::SoundBuffer hit3Buffer;
	sf::SoundBuffer selectBuffer;
	sf::SoundBuffer alarmBuffer;
	
	int levelReached;
	sf::Font levelFont;
	float textureScaler;
	unsigned int WINDOW_WIDTH = 1280u; 
	unsigned int WINDOW_HEIGHT = 704u;

private:
	GameData();
};