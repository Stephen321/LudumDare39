#include "GameOverScreen.h"
#include "GameData.h"
#include "Constants.h"
#include "AudioManager.h"

Screen::Type GameOverScreen::run(sf::RenderWindow &window) {
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	int menu = 0;

	sf::View view = window.getView();
	sf::FloatRect bounds(0.f, 0.f, view.getSize().x, view.getSize().y);

	sf::Sprite gameOver(GameData::getInstance().gameOverTexture);
	gameOver.setPosition(0.f, 0.f);
	gameOver.setScale(bounds.width / gameOver.getLocalBounds().width, bounds.height / gameOver.getLocalBounds().height);
	sf::Text text("", GameData::getInstance().levelFont, 1.5 * TILE_SIZE * bounds.width);
	text.setFillColor(sf::Color(255,106,0));
	text.setPosition(0.475f * bounds.width, bounds.height * 0.54f);
	text.setStyle(sf::Text::Regular);
	text.setString(std::to_string(GameData::getInstance().levelReached + 1));

	AudioManager audioManager;

	audioManager.play(AudioManager::Type::GameOverMusic);
	while (Running)	{
		float dt = frameClock.restart().asSeconds();

		while (window.pollEvent(Event))	{
			if (Event.type == sf::Event::Closed ||
				(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)) {
				return Screen::Type::Exit;
			}

			if (Event.type == sf::Event::KeyReleased && Event.key.code == sf::Keyboard::Space) {
				audioManager.play(AudioManager::Type::Select);
				return Screen::Type::Menu;
			}
		}

		window.clear();
		window.draw(gameOver);
		window.draw(text);
		window.display();
	}

	return Screen::Type::Exit;
}