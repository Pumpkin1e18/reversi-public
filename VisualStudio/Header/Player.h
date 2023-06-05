#include "ReversiBoard.h"
#include "header.h"

class PlayerRandom {
public:
	ReversiBoard board;
	int n, role;
	PlayerRandom(int N, int ROLE);  // 初期化
	int get_act(ReversiBoard BOARD);  // 行動取得
};

class PlayerHuman {
public:
	ReversiBoard board;
	int n, role;
	PlayerHuman(int N, int ROLE);  // 初期化
	int get_act(ReversiBoard BOARD);  // 行動取得
};


class PlayerAlphaBeta {
public:
	ReversiBoard board;
	int n, role, max_depth, best, last;
	PlayerAlphaBeta(int N, int ROLE);  // 初期化
	int get_score(int base);
	int alphabeta(int alpha, int beta, int color, int depth, int score);
	int get_act(ReversiBoard BOARD);  // 行動取得
};

