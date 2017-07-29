#include "GameScreen.h"
#include "Player.h"
#include "GameData.h"
#include "BulletPool.h"
#include "PrisonerManager.h"
#include "GameUI.h"

namespace {
	const unsigned int BULLET_POOL_SIZE = 100;
	const float PLAYER_BOUNDS_SIZE = 0.025;// 32 / 1280; //as a percentage of the target window width
}
Screen::Type GameScreen::run(sf::RenderWindow &window) {
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	bool pause = false;

	sf::Vector2u windowSize = window.getSize();
	BulletPool bulletPool(BULLET_POOL_SIZE);
	Player player(sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 0.5f), bulletPool);
	PrisonerManager prisonerManager(windowSize);
	GameUI gameUI(windowSize);

	prisonerManager.decreasePower();
	prisonerManager.decreasePower();
	prisonerManager.decreasePower();

	sf::Sprite background(GameData::getInstance().backgroundTexture);
	
	while (Running)	{
		while (window.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed || 
				(Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)) {
				return Screen::Type::Exit;
			}
			if (Event.type == sf::Event::LostFocus) {
				pause = true;
			}
			if (Event.type == sf::Event::GainedFocus) {
				pause = false;
			}
			//left mouse button pressed  then start firing
			if (Event.type == sf::Event::MouseButtonPressed &&
				Event.mouseButton.button == sf::Mouse::Button::Left) {
				player.setFiring(true);
			}
			//already firing and left button  is released
			else if (player.getFiring() && 
					 Event.type == sf::Event::MouseButtonReleased &&
					 Event.mouseButton.button == sf::Mouse::Button::Left) {
				player.setFiring(false);
			}
		}		

		//update game screen
		float dt = frameClock.restart().asSeconds();
		if (dt > 0.3f || pause)
			dt = 0.f;


		//limit player to within the playerable area
		float boundsSize = PLAYER_BOUNDS_SIZE * windowSize.x;
		sf::Vector2f playerHalfSize = player.getSize() * 0.5f;
		if (player.getPosition().x - playerHalfSize.x < boundsSize)
			player.setPositionX(boundsSize + playerHalfSize.x);
		else if (player.getPosition().x + playerHalfSize.x > windowSize.x - boundsSize)
			player.setPositionX(windowSize.x - boundsSize - playerHalfSize.x);
		if (player.getPosition().y - playerHalfSize.y < boundsSize)
			player.setPositionY(boundsSize + playerHalfSize.y);
		else if (player.getPosition().y + playerHalfSize.y > windowSize.y - boundsSize)
			player.setPositionY(windowSize.y - boundsSize - playerHalfSize.y);

		sf::Vector2f direction = (sf::Vector2f)sf::Mouse::getPosition(window) - player.getPosition();
		player.setDirection(direction);
		player.update(dt);

	
		bulletPool.update(dt);

		int numOfCollision = prisonerManager.update(dt, player.getPosition(), player.getSize());


		//draw game screen
		window.clear(sf::Color(96, 23, 54));

		window.draw(background);
		window.draw(player);
		window.draw(bulletPool);
		window.draw(prisonerManager);
		window.draw(gameUI);

		window.display();
	}

	return Screen::Type::GameOver;
}