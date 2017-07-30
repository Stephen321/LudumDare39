#pragma once

#include "SFML\Audio.hpp"

class AudioManager {
public:
	enum class Type {
		CellDoor,
		Death,
		Gunshot,
		Hit,
		Select,
		Alarm,
		MenuMusic,
		GameMusic,
		GameOverMusic
	};
	AudioManager();
	void play(Type type);

private:
	sf::Sound m_cellDoor;
	sf::Sound m_deaths[2];
	sf::Sound m_gunshot;
	sf::Sound m_hits[3];
	sf::Sound m_select;
	sf::Sound m_alarm;
	sf::Music m_menuMusic;
	sf::Music m_gameMusic;
	sf::Music m_gameOverMusic;
};
