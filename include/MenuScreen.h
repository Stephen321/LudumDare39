#pragma once

#include "Screen.h"

class MenuScreen : public Screen {
public:
	MenuScreen() : m_fromGameOver(false) {}
	Screen::Type run(sf::RenderWindow &window) override;

private:
	bool m_fromGameOver;
};