#include "stdafx.h"
#include "Reversi.h"
#include "ReversiBoard.h"
#include "Player.h"
#include "test.h"
#include "header.h"
#include "bits/stdc++.h"


const int n = 8;
ReversiBoard board(8);
PlayerHuman player1(n, BLACK);
PlayerAlphaBeta player2(n, WHITE);
int turn = BLACK, cnt = 0, role, act;

extern "C" {
	char * communication(char *str) {

		// init1
		if (str[0] == 'i' and str[1] == 'n') {
			board = ReversiBoard(8);
			if (str[4] == '1')role = BLACK;
			else role = WHITE;
			turn = BLACK;
			player1 = PlayerHuman(n, role);
			player2 = PlayerAlphaBeta(n, -role);
			return str;
		}

		// move05**
		if (str[0] == 'm' and str[1] == 'o') {
			int pos = (str[4]-'0')*10+(str[5]-'0');
			// 置けない時は-1
			if (board.can_put(pos / n, pos%n, role) == 0) {
				str[0] = '-'; str[1] = '1';
				return str;
			}
			// put
			board.put(pos / n, pos%n, turn);
			turn = -turn;
			// ボード情報取得
			string state = board.get_state(turn);
			for (int i = 0; i < n*n; i++)str[i] = state[i];
			for (int i = 0; i < n*n; i++) { if (str[i] == '3')str[i] = '0'; }
			str[n*n] = (pos/n)+'0'; str[n*n + 1] = (pos%n)+'0';
			return str;
		}

		// get**
		if (str[0] == 'g' and str[1] == 'e') {
			P cnt = board.can_put_cnt(turn);
			// end
			if (cnt.first == 0 and cnt.second == 0) {
				str[0] = 'e'; str[1] = 'n'; str[2] = 'd';
				return str;
			}
			// pass
			if (cnt.first == 0) {
				turn = -turn;
				str[0] = 'p'; str[1] = 'a'; str[2] = 's'; str[3] = 's';
				return str;
			}
			// wait
			if (turn == role) {
				str[0] = 'w'; str[1] = 'a'; str[2] = 'i'; str[3] = 't';
				return str;
			}
			// put
			act = player2.get_act(board);
			board.put(act / n, act%n, turn);
			turn = -turn;
			// ボード情報取得
			string state = board.get_state(turn);
			for (int i = 0; i < n*n; i++)str[i] = state[i];
			str[n*n] = (act/n)+'0'; str[n*n + 1] = (act%n)+'0';
			return str;
		}

		return str;
	}
}

