#include "GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameData.h"
#include "Helpers.h"

GameObject::GameObject(Type type) :
	m_active(true)
	, m_currentFrame(0)
	, m_animated(false)
	, m_timer(0.f)
	, m_looping(true)
	, m_finished(false) {

	GameData& data = GameData::getInstance();
	switch (type) {
	case Type::Player:
		m_sprite.setTexture(data.playerTexture);
		setUpAnimation(3, 48, 48, 7, 2);			
		break;
	case Type::Prisoner:
		m_sprite.setTexture(data.prisonerTexture);
		setUpAnimation(4, 48, 48, 3, 1);
		break;
	case Type::Bullet:
		m_sprite.setTexture(data.bulletTexture);
		break;
	case Type::Blood:
		m_sprite.setTexture(data.deathBloodTex);
		setLooping(false);
		setUpAnimation(7, 42, 42, 30, 6);
		break;
	default:
		break;
	}
	if (data.textureScaler != 1.f) {
		m_sprite.setScale(data.textureScaler, data.textureScaler);
	}
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width * 0.5f
					 , m_sprite.getGlobalBounds().height * 0.5f);
}

void GameObject::update(float dt) {
	if (m_animated) {
		if (m_playAnimation) {
			m_timer += dt;
			if (m_timer > m_timePerFrame) {
				m_timer = 0.f;
				if (m_finished == false)
					m_currentFrame++;
					if (m_currentFrame >= m_frames) {
						if (m_looping) {
							m_currentFrame = 0;
						}
						else {
							m_finished = true;
							m_currentFrame = m_idleFrame;
							m_animated = false;
						}
					}
				updateTextureRect();
			}
		}
		else if (m_currentFrame != m_idleFrame) {
			m_currentFrame = m_idleFrame;
			updateTextureRect();			
		}
 	}

	m_sprite.setPosition(m_position); 
	if (Helpers::getLength(m_direction) > 0.f)
		m_sprite.setRotation(atan2f(m_direction.y, m_direction.x) * (180.f / M_PI));
}

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	if (m_active)
		target.draw(m_sprite, states);
}

sf::Vector2f GameObject::getPosition() const {
	return m_position;
}

void GameObject::setActive(bool active) {
	m_active = active;
}

bool GameObject::getActive() const {
	return m_active;
}

sf::Vector2f GameObject::getSize() const {
	return sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height);
}

void GameObject::setPositionX(float x) {
	m_position.x = x;
}

void GameObject::setPositionY(float y) {
	m_position.y = y;
}

void GameObject::setRotation(float rotation) {
	m_sprite.setRotation(rotation);
}

float GameObject::getRotation() const {
	return m_sprite.getRotation();
}

void GameObject::updateTextureRect() {
	int row = m_currentFrame / m_framesPerRow;
	int col = m_currentFrame % m_framesPerRow;
	m_sprite.setTextureRect(sf::IntRect(col * m_frameWidth, row * m_frameHeight, m_frameWidth, m_frameHeight));
}

void GameObject::setUpAnimation(int frames, int frameWidth, int frameHeight, int framesPerSecond, int idleFrame) {
	m_animated = true;
	m_playAnimation = true;
	m_currentFrame = 0;
	m_frames = frames;
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;
	m_timePerFrame = (1000.f / framesPerSecond) / 1000.f;
	m_idleFrame = idleFrame;
	m_framesPerRow = m_sprite.getLocalBounds().width / m_frameWidth;
	updateTextureRect();
}

void GameObject::setCurrentFrame(int currentFrame) {
	m_currentFrame = currentFrame;
}

void GameObject::setLooping(bool value) {
	m_looping = value;
}

bool GameObject::getFinished() const {
	return m_finished;
}

void GameObject::setPlayAnimation(bool play) {
	m_playAnimation = play;
}
