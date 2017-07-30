#pragma once

#include "SFML\Graphics.hpp"

class GameObject : public sf::Drawable{
public:
	enum class Type {
		Player,
		Prisoner,
		Bullet
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
	
protected:
	sf::Vector2f m_position;
	sf::Vector2f m_direction;
	sf::Sprite m_sprite;
	bool m_active;
};