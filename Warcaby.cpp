#include "Warcaby.h"
#include "MetodyAI.h"

Warcaby::Warcaby()
{
	window.create(sf::VideoMode{ boardSize, boardSize + 30 }, "Warcaby");
	textures[0].loadFromFile("BOARD.png");
	textures[1].loadFromFile("CZERWONY_PION.png");
	textures[2].loadFromFile("CZERWONA_KROLOWA.png");
	textures[3].loadFromFile("CZARNY_PION.png");
	textures[4].loadFromFile("CZARNA_KROLOWA.png");
	textures[5].loadFromFile("Messages.png");

	sprites[0].setTexture(textures[0]);
	sprites[1].setTexture(textures[1]);
	sprites[2].setTexture(textures[2]);
	sprites[3].setTexture(textures[3]);
	sprites[4].setTexture(textures[4]);
	sprites[5].setTexture(textures[5]);

	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(20);
	text.setPosition(20, 880);
	font.loadFromFile("LucidaBrightRegular.ttf");
}


void Warcaby::play()
{
	if (gameStart())
	{
		while (window.isOpen())
		{
			draw();
			if (gracz)
			{
				text.setString("ruch gracza");
				window.draw(text);
			}
			else
			{
				text.setString("ruch CPU");
				window.draw(text);
			}
			window.display();

			if (gracz)
			{
				//ruch gracza
				ruchGracza(BLACK);
			}
			else 
			{
				//ruch cpu
				ruchCPU(RED);
			}
			gracz = !gracz;

			if (koniecGry(plansza.sprawdzStanGry(gracz?BLACK:RED)))
				return;
		}
	}
}

bool Warcaby::gameStart()
{
	sf::Event event;
	sprites[5].setPosition(62, 300);
	sprites[5].setTextureRect(sf::IntRect(0, 0, 750, 260));
	
	draw();
	window.draw(sprites[5]);
	window.display();

	while (true)
	{
		if (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				return 0;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					return 0;
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					return 1;
				}
			default:
				break;
			}
		}
	}

	return 1;
}

bool Warcaby::koniecGry(int status)
{
	switch (status) {
	case 0:
		return 0;
	case -1:
		sprites[5].setTextureRect(sf::IntRect(0, 700, 725, 200));
		break;
	case 1:
		sprites[5].setTextureRect(sf::IntRect(0, 300, 725, 200));
		break;
	case 2:
		sprites[5].setTextureRect(sf::IntRect(0, 500, 725, 200));
		break;
	}
	sprites[5].setPosition(62, 300);
	draw();
	window.draw(sprites[5]);
	window.display();
	sf::Event event;
	while (true) {
		if (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				return 1;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Space) {
					plansza.ustaw();
					gracz = true;
					return !gameStart();
				}
			}
		}
	}
}

void Warcaby::ruchGracza(Color kolor)
{
	int moveResult{}, first, second;
	do
	{
		if (moveResult == 0)
		{
			if ((first = mouse()) == -1)
				return;
		}
		do
		{
			if ((second = mouse()) == -1)
				return;

		} while (second == first);

		moveResult = plansza.ruch(first / 10, first % 10, second / 10, second % 10, kolor);
		if (moveResult == 0)
		{
			text.setString("bledny ruch - sprobuj ponownie");
			window.draw(text);
			window.display();
		}
		if (moveResult == 2) 
		{
			first = second;
			draw();
			window.display();
		}

	} while (moveResult != 1);
}

void Warcaby::ruchCPU(Color kolor)
{
	auto move = MetodyAI::pobierzNajlepszyRuch(6, plansza, RED);
	for (auto& coords : move.coords) {
		plansza.ruch(coords);
		draw();
		window.display();
	}
}

bool Warcaby::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			return true;
		}
	}

	return false;
}

//Przeliczam współrzędne myszki na współrzędne planszy
int Warcaby::mouse()
{
	sf::Vector2i pozycja;
	while (true)
	{
		if (handleEvents())
			return -1;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			pozycja = sf::Mouse::getPosition( window );
			if (pozycja.x > 50 && pozycja.x < 830 && pozycja.y > 50 && pozycja.y < 830)
			{
				pozycja.x -= 30;
				pozycja.y -= 30;
				return (pozycja.y - pozycja.y % 100) / 10 + (pozycja.x / 100);
			}
		}
	}
}

void Warcaby::draw()
{
	window.clear(sf::Color{ 41,41,41 });
	window.draw(sprites[0]);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			switch (plansza.plansza[i][j])
			{
			case RED_PION:
				sprites[1].setPosition(50 + j * 100, 50 + i * 100);
				window.draw(sprites[1]);
				break;
			case RED_KING:
				sprites[2].setPosition(50 + j * 100, 50 + i * 100);
				window.draw(sprites[2]);
				break;
			case BLACK_PION:
				sprites[3].setPosition(50 + j * 100, 50 + i * 100);
				window.draw(sprites[3]);
				break;
			case BLACK_KING:
				sprites[4].setPosition(50 + j * 100, 50 + i * 100);
				window.draw(sprites[4]);
				break;
			default:
				break;
			}
		}
	}
}
