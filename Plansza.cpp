#include "Plansza.h"

/* Koordynaty ruchu z do ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
Coords::Coords(int t_x1, int t_y1, int t_x2, int t_y2) {
	x1 = t_x1;
	y1 = t_y1;
	x2 = t_x2;
	y2 = t_y2;
}
bool Coords::operator!= (const Coords& cord) {
	if (this->x1 != cord.x1 ||
		this->y1 != cord.y1 ||
		this->x2 != cord.x2 ||
		this->y2 != cord.y2)
		return true;
	return false;
}


/* Poszczególne ruchy z do ~~~~~~~~~~~~~~~~~~~~~~~ */
Move::Move(int x1, int y1, int x2, int y2) {
	coords.emplace_back(x1, y1, x2, y2);
}

Move::Move(Coords c) {
	coords.push_back(c);
}

void Move::add(Coords c) {
	coords.insert(coords.begin(), c);
}

bool Move::operator== (const Move& B) {
	if (this->coords.size() != B.coords.size())
		return false;
	else
		for (int i = 0; i < coords.size(); i++)
			if (this->coords[i] != B.coords[i])
				return false;
	return true;
}


Plansza::Plansza()
{
	ustaw();
}


Plansza::~Plansza()
{
}

void Plansza::ustaw()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i % 2 != j % 2)
			{
				if (i < 3)
				{
					plansza[i][j] = RED_PION;
				}
				else if (i > 4)
				{
					plansza[i][j] = BLACK_PION;
				}
				else
				{
					plansza[i][j] = EMPTY;
				}
			}
			else
			{
				plansza[i][j] = EMPTY;
			}
		}
	}
}

//Sprawdzam czy bicie dla całego koloru
bool Plansza::sprawdzCzyBicie(Color kolor)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (kolor == BLACK)
			{
				if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION)
					if (sprawdzCzyBicie(i, j))
						return true;
			}
			else
			{
				if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION)
					if (sprawdzCzyBicie(i, j))
						return true;
			}
		}
	}
	return false;
}

//Sprawdzam czy bicie
bool Plansza::sprawdzCzyBicie(int x, int y)
{
	Color kolor = (plansza[x][y] == BLACK_PION || plansza[x][y] == BLACK_KING) ? BLACK : RED;

	//sprawdzam dla pionków
	if (plansza[x][y] == RED_PION || plansza[x][y] == BLACK_PION)
	{
		for (int a : { -1, 1 })
		{
			for (int b :{ -1, 1 })
			{
				int i = x + a;
				int j = y + b;
				if (i + a <= 7 && i + a >= 0 && j + b <= 7 && j + b >= 0)
				{
					if (kolor == BLACK) 
					{
						if (plansza[i][j] == RED_PION || plansza[i][j] == RED_KING)
							if (plansza[i + a][j + b] == EMPTY)
								return true;
					}
					else
					{
						if (plansza[i][j] == BLACK_PION || plansza[i][j] == BLACK_KING)
							if (plansza[i + a][j + b] == EMPTY)
								return true;
					}
				}
			}
		}
	}
	
	//sprawdzam dla królowej
	else if (plansza[x][y] == RED_KING || plansza[x][y] == BLACK_KING)
	{
		for (int a : {-1, 1})
		{
			for (int b : {-1, 1})
			{
				int i = x + a;
				int j = y + b;
				while (i + a <= 7 && i + a >= 0 && j + b <= 7 && j + b >= 0)
				{
					if (kolor == BLACK)
					{
						if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION)
							break;

						if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION)
						{
							if (plansza[i + a][j + b] == EMPTY)
								return true;
							else
								break;
						}
					}
					else
					{
						if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION)
							break;

						if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION) {
							if (plansza[i + a][j + b] == EMPTY)
								return true;
							else
								break;
						}
					}
					i += a;
					j += b;
				}
			}
		}
	}

	return false;
}

std::vector< Move > Plansza::pobierzSekwencjeRuchow(Move move, Plansza b) {
	std::vector< Move > moves;
	if (b.ruch(move.coords[0]) == 1) 
	{
		moves.emplace_back(move.coords[0]);
	}
	else {
		for (auto& m : b.pobierzMozliweRuchy(move.coords[0].x2, move.coords[0].y2)) {
			std::vector< Move > tmp = pobierzSekwencjeRuchow(m, b);
			moves.insert(moves.end(), tmp.begin(), tmp.end());
		}
		for (auto& m : moves) {
			m.add(move.coords[0]);
		}
	}
	return moves;
}

std::vector< Move > Plansza::pobierzMozliweRuchy(Color kolor) {
	std::vector< Move > moves;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (kolor == BLACK) {
				if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION) {
					auto tmp = pobierzMozliweRuchy(i, j);
					for (Move& m : tmp) {
						auto tmp2 = pobierzSekwencjeRuchow(m, *this);
						moves.insert(moves.end(), tmp2.begin(), tmp2.end());
					}
				}
			}
			else {
				if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION) {
					auto tmp = pobierzMozliweRuchy(i, j);
					for (Move& m : tmp) {
						auto tmp2 = pobierzSekwencjeRuchow(m, *this);
						moves.insert(moves.end(), tmp2.begin(), tmp2.end());
					}
				}
			}
		}
	}
	return moves;
}

std::vector< Move > Plansza::pobierzMozliweRuchy(int x, int y)
{
	Color kolor;
	std::vector< Move > ruchy;
	bool czyPionMaBicie;

	//Jeżli pole na planszy puste zwracam pusty wektor
	if (plansza[x][y] == EMPTY)
		return ruchy;

	if (plansza[x][y] == BLACK_PION || plansza[x][y] == BLACK_KING) {
		kolor = BLACK;
	}
	else 
	{
		kolor = RED;
	}

	//Jeżli dla ruchu wybranym pionem nie ma bicia sprawdzCzyBicie(x, y) -> false
	//natomiast dla jakiegoœ innego piona jest bicie sprawdzCzyBicie(kolor) -> true
	//to zwracam pusty wektor ponieważ bicie jest obowiązkowe
	czyPionMaBicie = sprawdzCzyBicie(x, y);
	if (sprawdzCzyBicie(kolor) != czyPionMaBicie)
		return ruchy;

	//Sprawdzam dla pionów
	if (plansza[x][y] == RED_PION || plansza[x][y] == BLACK_PION)
	{
		//Bez bicia
		if (!czyPionMaBicie)
		{
			int xdir = (kolor == BLACK) ? -1 : 1;
			if (y < 7)
				if (plansza[x + xdir][y + 1] == EMPTY)
					ruchy.push_back(Move(x, y, x + xdir, y + 1));
			if (y > 0)
				if (plansza[x + xdir][y - 1] == EMPTY)
					ruchy.push_back(Move(x, y, x + xdir, y - 1));
		}
		//Jeżli dostępne bicie
		else
		{
			for (int a : {-1, 1}) {
				for (int b : {-1, 1}) {
					int i = x + a;
					int j = y + b;
					if (i + a <= 7 && i + a >= 0 && j + b <= 7 && j + b >= 0) {
						if (kolor == BLACK) {
							if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION)
								if (plansza[i + a][j + b] == EMPTY)
									ruchy.push_back(Move(x, y, i + a, j + b));
						}
						else {
							if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION)
								if (plansza[i + a][j + b] == EMPTY)
									ruchy.push_back(Move(x, y, i + a, j + b));
						}
					}
				}
			}
		}
	}
	//Sprawdzam dla królowej
	else
	{
		//sprawdzam czy królowa ma bicie
		czyPionMaBicie = sprawdzCzyBicie(x, y);
		//sprawdzam dla każdego kierunku
		for (int a : {-1, 1})
		{
			for (int b : {-1, 1})
			{
				int i = x + a;
				int j = y + b;
				//poruszaj się aż pojawi się coś po drodze
				while (i >= 0 && j >= 0 && i <= 7 && j <= 7)
				{
					if (plansza[i][j] == EMPTY)
					{
						if (!czyPionMaBicie)
							ruchy.push_back(Move(x, y, i, j));
					}
					//gra kolor czarny
					else if (kolor == BLACK)
					{
						//jeżli na drodze pion tego samego koloru
						if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION)
						{
							break;
						}
						//na drodze pojawił się pion przeciwnego koloru	
						else if ((plansza[i][j] == RED_KING || plansza[i][j] == RED_PION) && czyPionMaBicie)
						{
							do {
								i += a;
								j += b;
								if (i >= 0 && j >= 0 && i <= 7 && j <= 7) {
									if (plansza[i][j] == EMPTY)
										ruchy.push_back(Move(x, y, i, j));
									else
										break;
								}
								else
									break;
							} while (true);
							break;
						}
						else
						{
							break;
						}
					}
					//gra kolor czerwony
					else
					{
						//jeżli na drodze pion tego samego koloru
						if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION)
						{
							break;
						}
						//na drodze pojawił siê pion przeciwnego koloru	
						else if ((plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION) && czyPionMaBicie)
						{
							do {
								i += a;
								j += b;
								if (i >= 0 && j >= 0 && i <= 7 && j <= 7) {
									if (plansza[i][j] == EMPTY)
										ruchy.push_back(Move(x, y, i, j));
									else
										break;
								}
								else
									break;
							} while (true);
							break;
						}
						else
						{
							break;
						}
					}
					i += a;
					j += b;
				}				
			}
		}
	}

	return ruchy;
}

void Plansza::capture(int x1, int y1, int x2, int y2) {
	int a = (x1 < x2) ? 1 : -1;
	int b = (y1 < y2) ? 1 : -1;

	for (int i = x1 + a, j = y1 + b; i != x2, j != y2; i += a, j += b) {
		plansza[i][j] = EMPTY;
	}
}

//zwraca 0 jeżli błędny ruch, 1 jeżli koniec ruchów, 2 jeżli jest jeszcze ruch
int Plansza::ruch(Coords m) {
	Color color = (plansza[m.x1][m.y1] == RED_KING || plansza[m.x1][m.y1] == RED_PION) ? RED : BLACK;
	return ruch(m.x1, m.y1, m.x2, m.y2, color);
}

int Plansza::ruch(Move m) {
	bool color = (plansza[m.coords[0].x1][m.coords[0].y1] == RED_KING ||
		plansza[m.coords[0].x1][m.coords[0].y1] == RED_PION) ? 0 : 1;
	int i{};
	while (ruch(m.coords[i]) == 2)
		i++;
	return 1;
}

int Plansza::ruch(int x1, int y1, int x2, int y2, Color kolor)
{
	if ((plansza[x1][y1] == RED_PION && kolor == BLACK) || (plansza[x1][y1] == RED_KING && kolor == BLACK) ||
		(plansza[x1][y1] == BLACK_PION && kolor == RED) || (plansza[x1][y1] == BLACK_KING && kolor == RED))
		return 0;

	bool czyBicie = false;
	//jeżli dany ruch dla tego pionka jest wykonalny
	std::vector< Move > aa = pobierzMozliweRuchy(x1, y1);
	for (auto& move : pobierzMozliweRuchy(x1, y1))
	{
		if (move == Move(x1, y1, x2, y2))
		{
			//Jeśli ruch zawiera bicie
			if (sprawdzCzyBicie(kolor)) {
				capture(x1, y1, x2, y2);
				czyBicie = true;
			}
			plansza[x2][y2] = plansza[x1][y1];
			plansza[x1][y1] = EMPTY;
			// Jeżli nadal jest bicie dla tego piona
			if (czyBicie && sprawdzCzyBicie(x2, y2))
				return 2;
			else if (((kolor == RED) && x2 == 7) || ((kolor == BLACK) && x2 == 0))
				plansza[x2][y2] = (kolor == BLACK) ? BLACK_KING : RED_KING;
			return 1;
		}
	}

	return 0;
}

// zwraca 0 jeżli gra kontynuowana, 1 jeżli czerwone wygrały, 2 jeœli czarne wygrały, -1 jeœli remis 
int Plansza::sprawdzStanGry(Color gracz)
{
	int redPawns{}, blackPawns{}, redKings{}, blackKings{};
	bool redRuchy{}, blackRuchy{};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (plansza[i][j]) {
			case EMPTY:
				break;
			case RED_PION:
				if (!redRuchy)
					if (pobierzMozliweRuchy(i, j).size() != 0)
						redRuchy = true;
				redPawns++;
				break;
			case BLACK_PION:
				if (!blackRuchy)
					if (pobierzMozliweRuchy(i, j).size() != 0)
						blackRuchy = true;
				blackPawns++;
				break;
			case RED_KING:
				if (!redRuchy)
					if (pobierzMozliweRuchy(i, j).size() != 0)
						redRuchy = true;
				redKings++;
				break;
			case BLACK_KING:
				if (!blackRuchy)
					if (pobierzMozliweRuchy(i, j).size() != 0)
						blackRuchy = true;
				blackKings++;
				break;
			}
		}
	}

	if ((blackPawns || blackKings) && (redPawns || redKings) && ((gracz == BLACK && blackRuchy) || (gracz == RED && redRuchy))) 
	{ 
		if (blackKings && redKings &&
			blackPawns == czarnePiony &&
			redPawns == czerwonePiony &&
			blackKings == czarneKrolowe &&
			redKings == czerwoneKrolowe) 
		{
			if (++licznikRuchow == 30)
				return -1;
		}
		else
		{
			licznikRuchow = 0;
		}

		czarnePiony = blackPawns;
		czerwonePiony = redPawns;
		czarneKrolowe = blackKings;
		czerwoneKrolowe = redKings;
		return 0;
	}
	else if ((!blackPawns && !blackKings) || (gracz == BLACK && !blackRuchy))
		return 1;
	else if ((!redPawns && !redKings) || (!gracz == RED && !redRuchy))
		return 2;
}

//Funkcja krawedziowa
int Plansza::funkcjaOceniajacaKrawedziowa(int x, int y)
{
	if (x > 0 && x < 7 && y > 0 && y < 7)
	{
		if (plansza[x][y] == RED_KING || plansza[x][y] == BLACK_KING)
		{
			return 8;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if (plansza[x][y] == RED_KING || plansza[x][y] == BLACK_KING)
		{
			return 14;
		}
		else
		{
			return 4;
		}
	}
}

//Zlicza ilość pionków i królowych pion 2 punkty, królowa 5 punktów
int Plansza::funkcjaOceniajacaPionkowa(int x, int y)
{
	switch (plansza[x][y]) {
	case EMPTY:
		return 0;
	case RED_PION:
		return 2;
	case BLACK_PION:
		return 2;
	case RED_KING:
		return 5;
	case BLACK_KING:
		return 5;
	}
}

//Kazda figura z mozliwoscia wykonania bicia + 10 pkt
int Plansza::funkcjaOceniajacaCzyBicie(int x, int y)
{
	int punkty{};
	sprawdzCzyBicie(x, y) ? punkty += 10 : punkty += 0;

	return punkty;
}

//Punkty za Poziomy II +1 punkt, III +3 punkty, IV +10 punktów
int Plansza::funkcjaOceniajacaPoziomy(int x, int y)
{
	if (plansza[x][y] == EMPTY)
		return 0;

	if (plansza[x][y] == RED_PION)
	{
		if (x >= 2 && x <= 3)        //II poziom (3,4 rząd dla czerwonych)
		{
			return 1;
		}
		if (x >= 4 && x <= 5)        //III poziom (5,6 rząd)
		{
			return 3;
		}
		if (x >= 6 && x <= 7)        //IV poziom (7,8 rząd)
		{
			return 10;
		}
	}
	else
	{
		if (x >= 4 && x <= 5)        //II poziom (3,4 rząd dla czarnych)
		{
			return 1;
		}
		if (x >= 2 && x <= 3)        //III poziom (5,6 rząd)
		{
			return 3;
		}
		if (x >= 0 && x <= 1)        //IV poziom (7,8 rząd)
		{
			return 10;
		}
	}
}

//Obszar I - 3 punkty, II - 2 punkty, III - 1 punkt
int Plansza::funkcjaOceniajacaTrzyObszary(int x, int y)
{
	if (plansza[x][y] == EMPTY)
		return 0;

	if ((x == 0 || x == 7) && (y == 0 || y == 7))
	{
		return 3;
	}

	if ((x == 1 || x == 6) && (y == 1 || y == 6))
	{
		return 2;
	}

	if ((x >= 2 && x <= 5) && (y >= 2 && y <= 5))
	{
		return 1;
	}
}

//zwraca wartość w zależności od stanu gry i sumy wartości pól
int Plansza::funkcjaOceniajacaPlansza(Color gracz)
{
	int black{}, red{};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (plansza[i][j] == RED_KING || plansza[i][j] == RED_PION)
			{
				red += funkcjaOceniajacaKrawedziowa(i, j);
				red += funkcjaOceniajacaPionkowa(i, j);
				red += funkcjaOceniajacaCzyBicie(i, j);
				//red += funkcjaOceniajacaPoziomy(i, j);
				//red += funkcjaOceniajacaTrzyObszary(i, j);
			}			
			else if (plansza[i][j] == BLACK_KING || plansza[i][j] == BLACK_PION)
			{
				black += funkcjaOceniajacaKrawedziowa(i, j);
				black += funkcjaOceniajacaPionkowa(i, j);
				black += funkcjaOceniajacaCzyBicie(i, j);
				//black += funkcjaOceniajacaPoziomy(i, j);
				//black += funkcjaOceniajacaTrzyObszary(i, j);
			}			
		}
	}

	if (gracz == BLACK)
		return black - red;
	else
		return red - black;
}
