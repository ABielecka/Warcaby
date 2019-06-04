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

//Trzemane kordynaty ruchu (pole pocz¹tkowe x1,y1 i koñcowe x2,y2
struct Coords {
	int x1{};
	int y1{};
	int x2{};
	int y2{};
	Coords(int x1_, int y1_, int x2_, int y2_);
	bool operator!= (const Coords& B);
};

//Trzemane kordynaty wszystkich ruchów
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
	//pionki i królowe na krawêdzi odpowiednio 4, 14 punktów
	//pionki i królowe poza krawedzi¹ odpowiednio 2, 8 punktów
	int funkcjaOceniajacaKrawedziowa(int x, int y);
	//Za ka¿dego pionka 2 punkty
	//Za ka¿d¹ królow¹ 5 punktów
	int funkcjaOceniajacaPionkowa(int x, int y);
	//Mo¿liwoœæ bicia 10 punktów
	int funkcjaOceniajacaCzyBicie(int x, int y);
	int funkcjaOceniajacaPoziomy(int x, int y);
	int funkcjaOceniajacaTrzyObszary(int x, int y);
	int funkcjaOceniajacaPion(int x, int y);
	int funkcjaOceniajacaPlansza(Color gracz);
};

#endif

