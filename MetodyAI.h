#ifndef METODYAI_H
#define METODYAI_H

#include <vector>
#include <memory>
#include "Plansza.h"

// element do budowy drzewa przechowuj¹cego ruchy do okreœlonego poziomu
// Konstruktor elementu jest funkcja rekurencyjna, która wywo³uje sie, 
// az do osiagniecia maksymalnej g³ebokosci drzewa lub zakoñczenia gry
class Node {
public:
	Color kolor;
	Plansza plansza;
	int value;
	bool isCPU;
	Move ruch;
	std::vector< Node > childs{};

	Node(int depth, Plansza plansza_, bool isCPU_, Color kolor_, Move ruch_ = Move(0, 0, 0, 0));
};

//Dla metody minmax
int max(int a, int b);
int min(int a, int b);

//algorytm minmax, wybiera najlepszy ruch dla wyliczonych wartoœci
int minimax(Node& node, int depth, bool isMaximizingPlayer, int alpha, int beta);

class MetodyAI
{
private:
	MetodyAI();
public:
	
	~MetodyAI();
	static Move pobierzNajlepszyRuch(int maxdepth, Plansza plansza,  Color kolor);
};

#endif