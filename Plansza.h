#ifndef PLANSZA_H
#define PLANSZA_H

#include <vector>

enum Pionki {
	EMPTY,
	RED_PION,
	BLACK_PION,
	RED_KING,
	BLACK_KING
};

enum Color {
	RED,
	BLACK
};

//Trzymane koordynaty ruchu (pole poczatkowe x1,y1 i końcowe x2,y2
struct Coords {
	int x1{};
	int y1{};
	int x2{};
	int y2{};
	Coords(int x1_, int y1_, int x2_, int y2_);
	bool operator!= (const Coords& B);
};

//Trzymane koordynaty wszystkich ruchów
struct Move {
	std::vector< Coords > coords;
	Move() {}
	Move(int x1, int y1, int x2, int y2);
	Move(Coords c);
	void add(Coords c);
	bool operator== (const Move& B);
};

class Plansza
{
public:
	Pionki plansza[8][8];
	//do oceny gry
	int licznikRuchow{};
	int czerwonePiony{}, czarnePiony{}, czerwoneKrolowe{}, czarneKrolowe{};

	Plansza();
	~Plansza();

	void ustaw();
	int ruch(Coords m);
	int ruch(Move m);
	int ruch(int x1, int y1, int x2, int y2, Color kolor);
	std::vector< Move > pobierzMozliweRuchy(int x, int y);
	bool sprawdzCzyBicie(int x, int y);
	bool sprawdzCzyBicie(Color kolor);
	void capture(int x1, int y1, int x2, int y2);
	std::vector< Move > pobierzSekwencjeRuchow(Move move, Plansza b);
	std::vector< Move > pobierzMozliweRuchy(Color kolor);
	int sprawdzStanGry(Color gracz);
	//pionki i królowe na krawędzi odpowiednio 4, 14 punktów
	//pionki i królowe poza krawedzią odpowiednio 2, 8 punktów
	int funkcjaOceniajacaKrawedziowa(int x, int y);
	//Za każdego pionka 2 punkty
	//Za każdą królową 5 punktów
	int funkcjaOceniajacaPionkowa(int x, int y);
	//Możliwość bicia 10 punktów
	int funkcjaOceniajacaCzyBicie(int x, int y);
	int funkcjaOceniajacaPoziomy(int x, int y);
	int funkcjaOceniajacaTrzyObszary(int x, int y);
	int funkcjaOceniajacaPion(int x, int y);
	int funkcjaOceniajacaPlansza(Color gracz);
};

#endif

