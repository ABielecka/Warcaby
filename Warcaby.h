#ifndef WARCABY_H
#define WARCABY_H

#include <SFML/Graphics.hpp>
#include "Plansza.h"

const int boardSize = 880;

class Warcaby
{
private:
	bool gracz = true;
	sf::Font font;
	sf::Text text;

	sf::RenderWindow window;
	sf::Texture textures[6];
	sf::Sprite sprites[6];

	Plansza plansza;

	bool gameStart();
	void ruchGracza(Color kolor);
	void ruchCPU(Color kolor);

	bool handleEvents();
	int mouse();

public:
	Warcaby();
	//~Warcaby() = default;

	void play();
	void draw();
	bool koniecGry(int status);
};

#endif

