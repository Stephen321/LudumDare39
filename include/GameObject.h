#pragma once

#include "SFML\Graphics.hpp"

class GameObject : public sf::Drawable{
public:
	enum class Type {
		Player,
		Prisoner,
		Bullet,
		Blood
	};
	GameObject(Type type);
	virtual void update(float dt); 
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	sf::Vector2f getPosition() const;
	void setActive(bool active);
	bool getActive() const;
	sf::Vector2f getSize() const;
	void setPositionX(float x);
	void setPositionY(float y);
	void setRotation(float rotation);
	float getRotation() const;

protected:

	void setPlayAnimation(bool play);
	sf::Vector2f m_position;
	sf::Vector2f m_direction;
	sf::Sprite m_sprite;
	bool m_active;


	//TODO: make animated sprite class istead of all this here
	void updateTextureRect();
	void setUpAnimation(int frames, int frameWidth, int frameHeight, int framesPerSecond, int idleFrame);
	void setCurrentFrame(int currentFrame);
	void setLooping(bool value);
	bool getFinished() const;
	int m_frames;
	int m_framesPerRow;
	int m_currentFrame;
	int m_frameWidth;
	int m_frameHeight;
	float m_timePerFrame;
	bool m_playAnimation;
	int m_idleFrame;
	float m_timer;
	bool m_animated;
	bool m_looping;
	bool m_finished;
};