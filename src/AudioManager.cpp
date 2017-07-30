#include "AudioManager.h"
#include "GameData.h"

AudioManager::AudioManager() {
	GameData& data = GameData::getInstance();
	m_cellDoor.setBuffer(data.cellDoorBuffer);
	m_cellDoor.setVolume(40);
	m_deaths[0] = sf::Sound();
	m_deaths[0].setBuffer(data.death1Buffer);
	m_deaths[0].setVolume(40);
	m_deaths[1] = sf::Sound();
	m_deaths[1].setBuffer(data.death2Buffer);
	m_deaths[1].setVolume(40);

	m_gunshot.setBuffer(data.gunshotBuffer);
	m_gunshot.setVolume(45);
	m_hits[0] = sf::Sound();
	m_hits[0].setBuffer(data.hit1Buffer);
	m_hits[0].setVolume(40);
	m_hits[1] = sf::Sound();
	m_hits[1].setBuffer(data.hit2Buffer);
	m_hits[1].setVolume(40);
	m_hits[2] = sf::Sound();
	m_hits[2].setBuffer(data.hit3Buffer);
	m_hits[2].setVolume(40);

	m_select.setBuffer(data.selectBuffer);
	m_select.setVolume(50);
	m_alarm.setBuffer(data.alarmBuffer);
	m_alarm.setVolume(60);


	m_menuMusic.openFromFile("assets/audio/menu.wav");
	m_menuMusic.setVolume(50);
	m_menuMusic.setLoop(true);
	m_gameMusic.openFromFile("assets/audio/game.wav");
	m_gameMusic.setVolume(40);
	m_gameMusic.setLoop(true);
	m_gameOverMusic.openFromFile("assets/audio/gameOver.wav");
	m_gameOverMusic.setVolume(50);
	m_gameOverMusic.setLoop(true);
}

void AudioManager::play(Type type) {
	switch (type) {
	case Type::CellDoor:
		if (m_cellDoor.getStatus() != sf::Sound::Status::Playing)
			m_cellDoor.play();
		break;
	case Type::Death: {
		int play = rand() % 2;
		if (m_deaths[play].getStatus() == sf::Sound::Status::Playing) {
			play = 1 - play;
			if (m_deaths[play].getStatus() != sf::Sound::Status::Playing) {
				m_deaths[play].play();
			}
		}
		else {
			m_deaths[play].play();
		}
		break;
	}
	case Type::Gunshot:
		m_gunshot.play();
		break;
	case Type::Hit: {
		int play = rand() % 3;
		std::vector<int> possiblePlays;
		possiblePlays.push_back(0);
		possiblePlays.push_back(1);
		possiblePlays.push_back(2);
		for (auto it = possiblePlays.begin(); it != possiblePlays.end(); ) {
			if (m_hits[(*it)].getStatus() == sf::Sound::Status::Playing)
				it = possiblePlays.erase(it);
			else 
				++it;
		}
		if (possiblePlays.size() > 0)
			m_hits[possiblePlays[rand() % possiblePlays.size()]].play();
		break;
	}
	case Type::Select:
		if (m_select.getStatus() != sf::Sound::Status::Playing)
			m_select.play();
		break;
	case Type::Alarm:
		if (m_alarm.getStatus() != sf::Sound::Status::Playing)
			m_alarm.play();
		break;
	case Type::MenuMusic:
		if (m_menuMusic.getStatus() != sf::Music::Status::Playing)
			m_menuMusic.play();
		break;
	case Type::GameMusic:
		if (m_gameMusic.getStatus() != sf::Music::Status::Playing)
			m_gameMusic.play();
		break;
	case Type::GameOverMusic:
		if (m_gameOverMusic.getStatus() != sf::Music::Status::Playing)
			m_gameOverMusic.play();
		break;
	}
}
