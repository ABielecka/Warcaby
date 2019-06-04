#include "MetodyAI.h"

Node::Node(int depth, Plansza plansza_, bool isCPU_, Color kolor_, Move ruch_) {
	kolor = kolor_;
	plansza = plansza_;
	isCPU = isCPU_;
	ruch = ruch_;
	value = 0;
	if (depth == 0 || plansza.sprawdzStanGry(isCPU ? RED : BLACK) != 0)
		value = plansza.funkcjaOceniajacaPlansza(kolor);
	else {
		//auto m = plansza.pobierzMozliweRuchy(isCPU ? RED : BLACK);
		for (auto& m : plansza.pobierzMozliweRuchy(isCPU ? RED : BLACK)) {
			Plansza tmp(plansza);
			tmp.ruch(m);
			childs.emplace_back(depth - 1, tmp, !isCPU, kolor, m);
		}
	}
}

int max(int a, int b) {
	return a < b ? b : a;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int minimax(Node& node, int depth, bool isMaximizingPlayer, int alpha, int beta) {
	if (node.childs.size() == 0)
		return node.value;

	int value, bestVal;

	if (isMaximizingPlayer) {
		bestVal = -100000;
		for (auto& child : node.childs) {
			value = minimax(child, depth + 1, true, alpha, beta);
			bestVal = max(bestVal, value);
			if (bestVal == value && depth == 0)
				node.ruch = child.ruch;
			alpha = max(alpha, bestVal);
			if (beta <= alpha)
				break;
		}
		return bestVal;

	}
	else {
		bestVal = 100000;
		for (auto& child : node.childs) {
			value = minimax(child, depth + 1, false, alpha, beta);
			bestVal = min(bestVal, value);
			if (bestVal == value && depth == 0)
				node.ruch = child.ruch;
			beta = min(beta, bestVal);
			if (beta <= alpha)
				break;
			return bestVal;
		}
	}
}

Move MetodyAI::pobierzNajlepszyRuch(int maxdepth, Plansza plansza, Color kolor) 
{
	bool isCPU = true;
	auto root = std::make_unique<Node>(maxdepth, plansza, isCPU, kolor);
	minimax(*root, 0, true, -100000, 100000);
	return root->ruch;
}