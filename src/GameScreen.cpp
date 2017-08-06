#include "Player.h"
#include "GameData.h"
#include "GameUI.h"
#include "Helpers.h"
#include "GameScreen.h"
#include "Constants.h"
#include "AudioManager.h"
#include "BloodManager.h"
#include "Thor/Math.hpp"
#include "Thor/Animations.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

namespace {
	const unsigned int BULLET_POOL_SIZE = 100;
	const float SCREEN_FADE_TIME = 1.5f;
}

//https://stackoverflow.com/a/14795484
int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
						  float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y) {
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
	s10_x = p1_x - p0_x;
	s10_y = p1_y - p0_y;
	s32_x = p3_x - p2_x;
	s32_y = p3_y - p2_y;

	denom = s10_x * s32_y - s32_x * s10_y;
	if (denom == 0)
		return 0; // Collinear
	bool denomPositive = denom > 0;

	s02_x = p0_x - p2_x;
	s02_y = p0_y - p2_y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
	if ((s_numer < 0) == denomPositive)
		return 0; // No collision

	t_numer = s32_x * s02_y - s32_y * s02_x;
	if ((t_numer < 0) == denomPositive)
		return 0; // No collision

	if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
		return 0; // No collision
				  // Collision detected
	t = t_numer / denom;
	if (i_x != NULL)
		*i_x = p0_x + (t * s10_x);
	if (i_y != NULL)
		*i_y = p0_y + (t * s10_y);

	return 1;
}

Screen::Type GameScreen::run(sf::RenderWindow &window) {
	sf::Event Event;
	bool Running = true;
	sf::Clock frameClock;
	bool pause = false;

	GameData& data = GameData::getInstance();
	AudioManager audioManager;
	sf::Vector2u windowSize = window.getSize();
	BulletPool bulletPool(BULLET_POOL_SIZE, audioManager);
	Player player(sf::Vector2f(windowSize.x * TILE_SIZE * 2.f, windowSize.y * 0.5f), bulletPool);
	PrisonManager prisonManager(windowSize);
	BloodManager bloodManager;
	GameUI gameUI(windowSize);

	//thor particles
	pSystem.setTexture(data.particle1Tex); 
	pSystem.addAffector(thor::AnimationAffector(thor::FadeAnimation(0.f, 0.5f)));

	bulletHitEmitter.setEmissionRate(100);
	bulletHitEmitter.setParticleColor(thor::Distribution<sf::Color>(sf::Color(225, 200, 180))); 

	sf::Shader lightMaskShader;
	lightMaskShader.loadFromFile("assets/shaders/lighting.vert", "assets/shaders/lighting.frag")
		;
	sf::Shader shadowShader;
	shadowShader.loadFromFile("assets/shaders/shadow.vert", "assets/shaders/shadow.frag");
	shadowShader.setUniform("texture", data.shadowTex);
	//TODO: draw the light textures on to the light mask (like the playerLight) instead of having multiple textures
	sf::Texture* currentLm = &data.lmMax;

	const int LightHeight = windowSize.y;

	sf::RectangleShape screenHider((sf::Vector2f)windowSize);
	screenHider.setFillColor(sf::Color::Black);
	float fadeTimer = 0;
	bool faded = false;


	std::pair<bool, sf::IntRect> lights[LIGHTS];
	lights[0] = std::make_pair<bool, sf::IntRect>(true, sf::IntRect(windowSize.x * 0.03f, windowSize.y * 0.04f, windowSize.x * 0.34f, LightHeight * 0.91f));
	lights[1] = std::make_pair<bool, sf::IntRect>(true, sf::IntRect(windowSize.x * 0.36f, windowSize.y * 0.04f, windowSize.x * 0.28f, LightHeight * 0.91f));
	lights[2] = std::make_pair<bool, sf::IntRect>(true, sf::IntRect(windowSize.x * 0.63f, windowSize.y * 0.04f, windowSize.x * 0.34f, LightHeight * 0.91f));

	sf::VertexArray shadowPolysLeft(sf::PrimitiveType::Quads);
	sf::VertexArray shadowPolysMiddle(sf::PrimitiveType::Quads);
	sf::VertexArray shadowPolysRight(sf::PrimitiveType::Quads);

	sf::Transform shadowTransform;
	shadowShader.setUniform("modelMat", sf::Glsl::Mat4(shadowTransform.getMatrix()));

	std::vector<std::pair<sf::Vector2f, float>> shadowCentres;

	const float PrisonerRadius = 14; //TODO: dont hardcode this
	std::vector<std::unique_ptr<Prisoner>>& prisonersRef = prisonManager.getPrisoners();
	
	sf::RenderTexture gameTexture; //texture of the game to draw
	gameTexture.create(windowSize.x, windowSize.y);

	sf::RenderTexture lightMaskTarget;
	lightMaskTarget.create(windowSize.x, windowSize.y);


	sf::Sprite mouseSprite(data.mouseTex);
	mouseSprite.setOrigin(mouseSprite.getGlobalBounds().width * 0.5f, mouseSprite.getGlobalBounds().height * 0.5f);

	sf::Sprite playerLight(data.playerLightTex);
	playerLight.setOrigin(playerLight.getGlobalBounds().width * 0.5f, playerLight.getGlobalBounds().height * 0.5f);

	bool showLevelSwitchPads = false;
	int level = 0;
	audioManager.play(AudioManager::Type::Select);
	audioManager.play(AudioManager::Type::GameMusic);

	int powerLevel = MAX_POWER;
	struct LevelSwitchPad {
		sf::Vector2f position;
		sf::Sprite sprite;
		sf::Vector2f triggerPosition;
		float triggerRadius;
	};
	const int LevelSwitchPadSize = 3;
	LevelSwitchPad levelSwitchPads[LevelSwitchPadSize];
	for (int i = 0; i < LevelSwitchPadSize; i++) {
		levelSwitchPads[i].position.x = windowSize.x - (TILE_SIZE * 2.f * windowSize.x);
		levelSwitchPads[i].sprite.setTexture(data.switchPadTexture);
		levelSwitchPads[i].sprite.setScale(data.textureScaler, data.textureScaler);
		levelSwitchPads[i].sprite.setOrigin(levelSwitchPads[i].sprite.getGlobalBounds().width * 0.5f, levelSwitchPads[i].sprite.getGlobalBounds().height * 0.5f);
		levelSwitchPads[i].triggerRadius = TILE_SIZE * 1.5f * windowSize.x;
		levelSwitchPads[i].sprite.setPosition(levelSwitchPads[i].position);
	}
	//so messy 
	levelSwitchPads[0].position.y = TILE_SIZE * 3.5f * windowSize.x;
	levelSwitchPads[0].sprite.setPosition(levelSwitchPads[0].position);
	levelSwitchPads[0].triggerPosition = levelSwitchPads[0].position + sf::Vector2f(TILE_SIZE * 2.2f * windowSize.x, 0.f);
	levelSwitchPads[1].position.y = TILE_SIZE * 11.f * windowSize.x;
	levelSwitchPads[1].sprite.setPosition(levelSwitchPads[1].position);
	levelSwitchPads[1].triggerPosition = levelSwitchPads[1].position + sf::Vector2f(TILE_SIZE * 2.2f * windowSize.x, 0.f);
	levelSwitchPads[2].position.y = TILE_SIZE * 18.5f * windowSize.x;
	levelSwitchPads[2].sprite.setPosition(levelSwitchPads[2].position);
	levelSwitchPads[2].triggerPosition = levelSwitchPads[2].position + sf::Vector2f(TILE_SIZE * 2.2f * windowSize.x, 0.f);

	float m_powerDecreasePerSecond = POWER_DRAIN_PER_SECOND;
	sf::Sprite background(data.backgroundTexture);
	background.setScale(windowSize.x / background.getLocalBounds().width, windowSize.y / background.getLocalBounds().height);
	
	//test
	////------------------------------------------------------------------
	//shadowPolys.clear();
	//shadowCentres.clear();
	//shadowCentres.push_back(std::make_pair<sf::Vector2f, float>(sf::Vector2f(450, 550), 90.f));
	//for (int i = 0; i < prisonersRef.size(); i++) {
	//	shadowCentres.push_back(std::make_pair<sf::Vector2f, float>(prisonersRef[i]->getPosition(), prisonersRef[i]->getRotation() + 90.f)); //TODO: easier way to do this
	//}
	//addShadowPolys(shadowPolys, shadowCentres, PrisonerRadius, lights);
	////------------------------------------------------------------------

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
			if (Event.type == sf::Event::MouseButtonPressed &&
				Event.mouseButton.button == sf::Mouse::Button::Left) {
				player.setFiring(true);
			}
		}		

		//update game screen
		sf::Time dtTime = frameClock.restart();
		float dt = dtTime.asSeconds();
		if (dt > 0.3f || pause)
			dt = 0.f;

		if (fadeTimer < SCREEN_FADE_TIME) {
			fadeTimer += dt;
			screenHider.setFillColor(sf::Color(0, 0, 0, Helpers::lerp(255, 0, fadeTimer / SCREEN_FADE_TIME)));
			if (fadeTimer > SCREEN_FADE_TIME) {
				faded = true;
			}
		}

		sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
		sf::Vector2f direction = mousePosition - player.getPosition();
		player.setDirection(direction);
		player.update(dt);
		bulletPool.update(dt);
		bloodManager.update(dt);
		pSystem.update(dtTime);

		mouseSprite.setPosition(mousePosition);
		playerLight.setPosition(player.getPosition());

		int numOfCollision = prisonManager.update(dt, player.getPosition(), player.getSize());
		if (numOfCollision > 0) {
			if (gameUI.changeHealth(-numOfCollision) == false) {
				audioManager.play(AudioManager::Type::Death);
				data.levelReached = level;
				return Screen::Type::GameOver;
			}
			audioManager.play(AudioManager::Type::Hit);
		}
		gameUI.update(dt);
		int prisonersRemaining = prisonManager.getPrisonersRemaining();
		int maxPrisoners = prisonManager.getMaxPrisoners();
		gameUI.setPrisonerInfo(prisonersRemaining, maxPrisoners);

		float previousPower = gameUI.getPower();
		bool openExit = !gameUI.changePower(-POWER_DRAIN_PER_SECOND * dt);
		if (openExit && !showLevelSwitchPads) {
			prisonManager.decreasePower();
			currentLm = &data.lmNo;
			showLevelSwitchPads = true;
		}
		else {
			bool decrease = false;
			for (int i = 1; i < MAX_POWER + 1; i++) {
				if (gameUI.getPower() < gameUI.getMaxPower() - (i * POWER_POINTS_PER_LEVEL)) {
					if (previousPower > gameUI.getMaxPower() - (i * POWER_POINTS_PER_LEVEL)) {
						decrease = true;
						break;
					}
				}
			}
			if (decrease) {
				prisonManager.decreasePower();
				gameUI.decreasePowerLevel();
				powerLevel--;
				if (powerLevel == 2) {
					currentLm = &data.lmHigh;
					lights[0].first = false;
				}
				else if (powerLevel == 1) {
					currentLm = &data.lmMed;
					lights[1].first = false;
				}
				else if (powerLevel == 0) {
					currentLm = &data.lmLow;
					lights[2].first = false;
				}
				audioManager.play(AudioManager::Type::CellDoor);
			}
		}
		
		//restrict to bounds of the play area
		float boundsSize = TILE_SIZE * windowSize.x;
		restrictToBounds(player, boundsSize, windowSize);
		std::vector<Bullet>& bullets = bulletPool.getBullets();
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i].getActive()) {
				if (restrictToBounds(bullets[i], boundsSize, windowSize)) {
					bullets[i].setActive(false);
					createBulletEmitter(bullets, i);
				}
			}
		}

		checkCollisions(bullets, prisonersRef, audioManager, bloodManager);



		//------------------------------------------------------------------
		shadowPolysLeft.clear();
		shadowPolysMiddle.clear();
		shadowPolysRight.clear();
		shadowCentres.clear();
		shadowCentres.push_back(std::make_pair<sf::Vector2f, float>(player.getPosition(), player.getRotation() + 90.f));
		for (int i = 0; i < prisonersRef.size(); i++) {
			shadowCentres.push_back(std::make_pair<sf::Vector2f, float>(prisonersRef[i]->getPosition(), prisonersRef[i]->getRotation() + 90.f)); //TODO: easier way to do this
		}
		addShadowPolys(shadowPolysLeft, shadowCentres, PrisonerRadius, lights[0]);
		addShadowPolys(shadowPolysMiddle, shadowCentres, PrisonerRadius, lights[1]);
		addShadowPolys(shadowPolysRight, shadowCentres, PrisonerRadius, lights[2]);
		//------------------------------------------------------------------


		//draw game screen
		gameTexture.clear();
		gameTexture.draw(background);
		gameTexture.draw(bloodManager);

		if (showLevelSwitchPads && prisonersRemaining == 0) {
			gameUI.decreasePowerLevel();
			for (int i = 0; i < LevelSwitchPadSize; i++) {
				gameTexture.draw(levelSwitchPads[i].sprite);

				//also check player collisions with them
				if (circleCollision(player.getPosition(), levelSwitchPads[i].triggerPosition, levelSwitchPads[i].triggerRadius + player.getSize().x)) {
					level++;
					prisonManager.newLevel();
					player.reset();
					bulletPool.reset();
					showLevelSwitchPads = false;
					gameUI.reset(level);
					powerLevel = MAX_POWER;
					currentLm = &data.lmMax;
					bloodManager.reset();
					lights[0].first = true;
					lights[1].first = true;
					lights[2].first = true;
					screenHider.setFillColor(sf::Color::Black);
					faded = false;
					fadeTimer = 0;
					m_powerDecreasePerSecond += POWER_CHANGE_PER_LEVEL;
					if (m_powerDecreasePerSecond > POWER_DRAIN_MAX)
						m_powerDecreasePerSecond = POWER_DRAIN_MAX;
				}
			}
		}

		gameTexture.draw(pSystem);
		gameTexture.draw(bulletPool); 
		gameTexture.draw(player);
		gameTexture.draw(prisonManager);

		gameTexture.display();


		window.clear(sf::Color(96, 23, 54));

		lightMaskTarget.clear();
		lightMaskTarget.draw(sf::Sprite(*currentLm));

		shadowShader.setUniform("lightPosition", sf::Glsl::Vec2(getRectCentre(lights[0].second)));
		lightMaskTarget.draw(shadowPolysLeft, &shadowShader);
		shadowShader.setUniform("lightPosition", sf::Glsl::Vec2(getRectCentre(lights[1].second)));
		lightMaskTarget.draw(shadowPolysMiddle, &shadowShader);
		shadowShader.setUniform("lightPosition", sf::Glsl::Vec2(getRectCentre(lights[2].second)));
		lightMaskTarget.draw(shadowPolysRight, &shadowShader);
		lightMaskTarget.draw(playerLight);
		for (int i = 0; i < prisonersRef.size(); i++) {
			playerLight.setPosition(prisonersRef[i]->getPosition());
			playerLight.setColor(sf::Color(255, 255, 255, 50));
			lightMaskTarget.draw(playerLight);
		}
		playerLight.setColor(sf::Color(255, 255, 255, 255));
		lightMaskTarget.display();
		lightMaskShader.setUniform("lightmask", lightMaskTarget.getTexture());

		sf::Sprite gameSprite(gameTexture.getTexture());

		window.draw(gameSprite, &lightMaskShader);
		window.draw(mouseSprite);
		window.draw(gameUI);

		////Debug 

		//for (int i = 0; i < LIGHTS; i++) {
		//	sf::RectangleShape r;
		//	r.setPosition(lights[i].second.left, lights[i].second.top);
		//	r.setSize(sf::Vector2f(lights[i].second.width, lights[i].second.height));
		//	r.setFillColor(sf::Color::Transparent);
		//	r.setOutlineThickness(5);
		//	r.setOutlineColor(sf::Color::White);

		//	sf::CircleShape c(10);
		//	c.setOrigin(10, 10);
		//	c.setPosition(getRectCentre(lights[i].second));
		//	window.draw(c);
		//	window.draw(r);
		//}
		window.draw(screenHider);
		window.display();
	}

	return Screen::Type::GameOver;
}

void GameScreen::checkCollisions(std::vector<Bullet>& bullets, std::vector<std::unique_ptr<Prisoner>>& prisoners, AudioManager& audioManager, BloodManager& bloodManager) {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].getActive()) {
			for (int j = 0; j < prisoners.size(); j++) {
				float minDistance = prisoners[j]->getSize().x * 0.5f;
				if (circleCollision(bullets[i].getPosition(), prisoners[j]->getPosition(), minDistance)) {
					bullets[i].setActive(false);
					int healthRemaining = prisoners[j]->decreaseHealth();
					if (healthRemaining == 0) {
						audioManager.play(AudioManager::Type::Death);
						bloodManager.spawnBlood(prisoners[j]->getPosition(), prisoners[j]->getRotation());
					}
					else
						audioManager.play(AudioManager::Type::Hit);

					createBulletEmitter(bullets, i);
				}
			}
		}
	}

}

bool GameScreen::restrictToBounds(GameObject & object, float boundsSize, const sf::Vector2u& windowSize) {
	bool restricted = false;
	sf::Vector2f objectHalfSize = object.getSize() * 0.5f;
	if (object.getPosition().x - objectHalfSize.x < boundsSize) {
		object.setPositionX(boundsSize + objectHalfSize.x);
		restricted = true;
	}
	else if (object.getPosition().x + objectHalfSize.x > windowSize.x - boundsSize) {
		object.setPositionX(windowSize.x - boundsSize - objectHalfSize.x);
		restricted = true;
	}
	if (object.getPosition().y - objectHalfSize.y < boundsSize) {
		object.setPositionY(boundsSize + objectHalfSize.y);
		restricted = true;
	}
	else if (object.getPosition().y + objectHalfSize.y > windowSize.y - boundsSize) {
		object.setPositionY(windowSize.y - boundsSize - objectHalfSize.y);
		restricted = true;
	}
	return restricted;
}

bool GameScreen::circleCollision(const sf::Vector2f & pos1, const sf::Vector2f & pos2, float minDistance) {
	bool collided = false;
	float distanceBetween = Helpers::getLength(pos2 - pos1);
	if (distanceBetween < minDistance)
		collided = true;
	return collided;
}

void GameScreen::createBulletEmitter(std::vector<Bullet>& bullets, int i) {
	sf::Vector2f center = bullets[i].getPosition();
	float radius = 5;
	float speed = 80.f;
	sf::Vector2f velocity = -bullets[i].getVelocity();
	Helpers::limit(velocity, speed);
	bulletHitEmitter.setParticlePosition(thor::Distributions::circle(center, radius));
	bulletHitEmitter.setParticleVelocity(thor::Distributions::deflect(velocity, 15.f));;
	bulletHitEmitter.setParticleLifetime(thor::Distributions::uniform(sf::milliseconds(40), sf::milliseconds(80)));
	pSystem.addEmitter(bulletHitEmitter, sf::milliseconds(80));
}

sf::Vector2f GameScreen::getRectCentre(const sf::IntRect & rect) {
	return sf::Vector2f(rect.left + (rect.width * 0.5f), rect.top + (rect.height * 0.5f));
}

void GameScreen::addShadowPolys(sf::VertexArray & vertexArray, const std::vector<std::pair<sf::Vector2f, float>>& centres, float radius, const std::pair<bool, sf::IntRect> light) {
	if (light.first) {
		sf::IntRect rect = light.second;
		sf::Vector2f centre = getRectCentre(rect);

		for (int j = 0; j < centres.size(); j++) {
			sf::Vector2f shadowCentre = centres[j].first;

			float distanceToLight = Helpers::getLength(shadowCentre - centre);

			if (distanceToLight > 700 || distanceToLight < 5) {
				continue;
			}

			float degrees = centres[j].second;
			sf::Vector2f v1(radius, 0);
			v1 = shadowCentre + rotateVector(v1, degrees);
			sf::Vector2f v2(-radius, 0);
			v2 = shadowCentre + rotateVector(v2, degrees);

			sf::Vector2f v1Extended = v1 + (3000.f * Helpers::normaliseCopy(v1 - centre));
			sf::Vector2f v2Extended = v2 + (3000.f * Helpers::normaliseCopy(v2 - centre));

			//test
			sf::Vector2f screenCentre(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f);

			//work out bounds
			sf::Vector2f topBoundsP1(0.f, 0.f);
			sf::Vector2f topBoundsP2(WINDOW_WIDTH, 0.f);
			sf::Vector2f bottomBoundsP1(0.f, WINDOW_HEIGHT);
			sf::Vector2f bottomBoundsP2(WINDOW_WIDTH, WINDOW_HEIGHT);
			sf::Vector2f leftBoundsP1(0.f, 0.f);
			sf::Vector2f leftBoundsP2(0.f, WINDOW_HEIGHT);
			sf::Vector2f rightBoundsP1(WINDOW_WIDTH, 0.f);
			sf::Vector2f rightBoundsP2(WINDOW_WIDTH, WINDOW_HEIGHT);

			float intersectionX = 0;
			float intersectionY = 0;
			if (get_line_intersection(topBoundsP1.x, topBoundsP1.y, topBoundsP2.x, topBoundsP2.y, v1.x, v1.y, v1Extended.x, v1Extended.y, &intersectionX, &intersectionY)) {
			}
			else if (get_line_intersection(bottomBoundsP1.x, bottomBoundsP1.y, bottomBoundsP2.x, bottomBoundsP2.y, v1.x, v1.y, v1Extended.x, v1Extended.y, &intersectionX, &intersectionY)) {
			}
			else if (get_line_intersection(leftBoundsP1.x, leftBoundsP1.y, leftBoundsP2.x, leftBoundsP2.y, v1.x, v1.y, v1Extended.x, v1Extended.y, &intersectionX, &intersectionY)) {
			}
			else if (get_line_intersection(rightBoundsP1.x, rightBoundsP1.y, rightBoundsP2.x, rightBoundsP2.y, v1.x, v1.y, v1Extended.x, v1Extended.y, &intersectionX, &intersectionY)) {
			}
			sf::Vector2f v3(intersectionX, intersectionY);
			intersectionX = 0;
			intersectionY = 0;

			if (get_line_intersection(topBoundsP1.x, topBoundsP1.y, topBoundsP2.x, topBoundsP2.y, v2.x, v2.y, v2Extended.x, v2Extended.y, &intersectionX, &intersectionY)) {
			}
			else if (get_line_intersection(bottomBoundsP1.x, bottomBoundsP1.y, bottomBoundsP2.x, bottomBoundsP2.y, v2.x, v2.y, v2Extended.x, v2Extended.y, &intersectionX, &intersectionY)) {
			}
			else if (get_line_intersection(leftBoundsP1.x, leftBoundsP1.y, leftBoundsP2.x, leftBoundsP2.y, v2.x, v2.y, v2Extended.x, v2Extended.y, &intersectionX, &intersectionY)) {
			}
			else if (get_line_intersection(rightBoundsP1.x, rightBoundsP1.y, rightBoundsP2.x, rightBoundsP2.y, v2.x, v2.y, v2Extended.x, v2Extended.y, &intersectionX, &intersectionY)) {
			}
			sf::Vector2f v4(intersectionX, intersectionY);
			//	---------------->
			//	|				|
			//	|				|
			//	v				v
			//	---------------->

			sf::Vector2f center = (v1 + v2 + v3 + v4) / 4.f;

			std::vector<sf::Vector2f> sorted;
			sorted.push_back(v1);
			sorted.push_back(v2);
			sorted.push_back(v3);
			sorted.push_back(v4);

			//sort vertices to be clockwise

			//https://stackoverflow.com/a/6989383
			std::sort(sorted.begin(), sorted.end(), [=](sf::Vector2f a, sf::Vector2f b) -> bool {
				if (a.x - center.x >= 0 && b.x - center.x < 0)
					return true;
				if (a.x - center.x < 0 && b.x - center.x >= 0)
					return false;
				if (a.x - center.x == 0 && b.x - center.x == 0) {
					if (a.y - center.y >= 0 || b.y - center.y >= 0)
						return a.y > b.y;
					return b.y > a.y;
				}

				// compute the cross product of vectors (center -> a) x (center -> b)
				int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
				if (det < 0)
					return true;
				if (det > 0)
					return false;

				// points a and b are on the same line from the center
				// check which point is closer to the center
				int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
				int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
				return d1 > d2;
			});

			float w = GameData::getInstance().shadowTex.getSize().x;
			float h = GameData::getInstance().shadowTex.getSize().y;
			vertexArray.append(sf::Vertex(sorted[0], sf::Vector2f(0.f, 0.f)));
			vertexArray.append(sf::Vertex(sorted[1], sf::Vector2f(w, 0.f)));
			vertexArray.append(sf::Vertex(sorted[2], sf::Vector2f(w, h)));
			vertexArray.append(sf::Vertex(sorted[3], sf::Vector2f(0.f, h)));
		}
	}
}

sf::Vector2f GameScreen::rotateVector(const sf::Vector2f & v, float degrees) {
	sf::Vector2f result;
	float radians = degrees * (M_PI / 180.f);
	result.x = v.x * cos(radians) - v.y * sin(radians);
	result.y = v.x * sin(radians) + v.y * cos(radians);
	return result;
}
