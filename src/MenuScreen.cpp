#include "MenuScreen.h"
#include "GameData.h"
#include "AudioManager.h"

Screen::Type MenuScreen::run(sf::RenderWindow &window) {
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;


	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	sf::Sprite start(GameData::getInstance().menuTexture);
	start.setPosition(0.f, 0.f);
	start.setScale(bounds.width / start.getLocalBounds().width, bounds.height / start.getLocalBounds().height);

	AudioManager audioManager;
	audioManager.play(AudioManager::Type::MenuMusic);

	if (m_fromGameOver) { 
		audioManager.play(AudioManager::Type::Select);
	}
	m_fromGameOver = true;

	while (Running)	{
		float dt = frameClock.restart().asSeconds();

		while (window.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed ||
				(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)) {
				return Screen::Type::Exit;
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Space) {
				return Screen::Type::Game;
			}
		}

		window.clear();
		window.draw(start);
		window.display();
	}

	return Screen::Type::Exit;
}