#include "stdafx.h"
#include "ReversiBoard.h"
#include "header.h"


// inline
// 負の値に対応したshift
inline Bit r_shift(const Bit &bit, int x) { if (x < 0)return bit >> -x; return bit << x; }
inline Bit l_shift(const Bit &bit, int x) { if (x < 0)return bit << -x; return bit >> x; }

// 最下位bit, 最上位bit取得
inline int get_first(const Bit &bit) { return bit.find_first(); }
inline int get_last(const Bit &bit) {
	int ans = -1, pos = bit.find_first();
	for (; pos != bit.npos; pos = bit.find_next(pos))ans = pos;
	return ans;
}

// 初期化
ReversiBoard::ReversiBoard() { init(8); }
ReversiBoard::ReversiBoard(int N) { init(N); }

// マージン1のBit作成
void ReversiBoard::set_extension() {
	extension.reset();
	for (int pos = empty.find_first(); pos != empty.npos; pos = empty.find_next(pos)) {
		extension.set((pos / n + 1)*(n + 2) + (pos%n + 1));
	}
}

// 初期化の処理
void ReversiBoard::init(int N) {
	n = N; turn_cnt = 0;
	black.resize(N*N); white.resize(N*N); empty.resize(N*N);  // サイズ初期化
	can.resize(N*N); sur.resize((N + 2)*(N + 2)); extension.resize((N + 2)*(N + 2));
	rep(i, 4)vec[i].resize(N + 1);
	rep(i, 4)rep(j, N + 1)vec[i][j].resize(N*N);
	black.set((n / 2)*n + (n / 2 - 1)); black.set((n / 2 - 1)*n + (n / 2));  // 初期盤面(黒)
	white.set((n / 2 - 1)*n + (n / 2 - 1)); white.set((n / 2)*n + (n / 2));  // 初期盤面(白)
	rep(i, n)rep(j, n) { if (i <= j)vec[0][j + 1].set(i); }  // 横
	rep(i, n)rep(j, n) { if (i <= j)vec[1][j + 1].set(i*n); }  // 縦
	rep(i, n)rep(j, n) { if (i <= j)vec[2][j + 1].set(i*n + i); }  // 左上斜め
	rep(i, n)rep(j, n) { if (i <= j)vec[3][j + 1].set(i*n + (n - i - 1)); }  // 右上斜め

	empty = (black | white).flip();
	rep(i, 3)rep(j, 3)sur.set(i*(n + 2) + j);
	set_extension();
	repi(i, n / 2 - 2, n / 2 + 2)repi(j, n / 2 - 2, n / 2 + 2) { // 置ける候補の初期値
		if (black[i*n + j] or white[i*n + j])continue;
		can.set(i*n + j);
	}
}

string ReversiBoard::get_state(int color) {
	string str = "";
	for (int i = 0; i < n;i++) {
		for (int j = 0; j < n;j++) {
			if (black[i*n + j])str.push_back('1');
			else if (white[i*n + j])str.push_back('2');
			else if (can_put(i, j, color))str.push_back('3');
			else str.push_back('0');
		}
	}
	return str;
}

// (y, x)に置いた時の変える位置のBit取得
Bit ReversiBoard::get_mask(int y, int x, int color) {
	Bit fri = black, ene = white;
	if (color == WHITE)swap(fri, ene);
	Bit mask(n*n);

	if (x < n - 2 and ene[y*n + x + 1]) {   //右
		Bit bit = vec[0][n - 2 - x] << (y*n + x + 2) & fri;
		if (bit.any()) {
			int pos = get_first(bit);
			bit = vec[0][pos%n - x - 1] << (y*n + x + 1);
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (2 <= x and ene[y*n + x - 1]) {   //左
		Bit bit = vec[0][x - 1] << (y*n) & fri;
		if (bit.any()) {
			int pos = get_last(bit);
			bit = vec[0][x - pos % n - 1] << (pos + 1);
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (y < n - 2 and ene[(y + 1)*n + x]) {   //下
		Bit bit = vec[1][n - 2 - y] << ((y + 2)*n + x) & fri;
		if (bit.any()) {
			int pos = get_first(bit);
			bit = vec[1][pos / n - y - 1] << ((y + 1)*n + x);
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (2 <= y and ene[(y - 1)*n + x]) {   //上
		Bit bit = vec[1][y - 1] << (x)& fri;
		if (bit.any()) {
			int pos = get_last(bit);
			bit = vec[1][y - pos / n - 1] << (pos + n);
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (x < n - 2 and y < n - 2 and ene[(y + 1)*n + (x + 1)]) {   //右下
		int m = min(n - x - 1, n - y - 1);
		Bit bit = vec[2][m - 1] << ((y + 2)*n + (x + 2)) & fri;
		if (bit.any()) {
			int pos = get_first(bit);
			bit = vec[2][pos / n - y - 1] << ((y + 1)*n + (x + 1));
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (2 <= x and 2 <= y and ene[(y - 1)*n + (x - 1)]) {   //左上
		int m = min(x, y);
		Bit bit = vec[2][m - 1] << ((y - m)*n + (x - m)) & fri;
		if (bit.any()) {
			int pos = get_last(bit);
			bit = vec[2][y - pos / n - 1] << (pos + n + 1);
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (2 <= x and y < n - 2 and ene[(y + 1)*n + (x - 1)]) {   //左下
		int m = min(x, n - y - 1);
		Bit bit = r_shift(vec[3][m - 1], (y + 2)*n + (x - 2) - (n - 1)) & fri;
		if (bit.any()) {
			int pos = get_first(bit);
			bit = r_shift(vec[3][pos / n - y - 1], (y + 1)*n + (x - 1) - (n - 1));
			if ((bit&empty).none())mask |= bit;
		}
	}
	if (x < n - 2 and 2 <= y and ene[(y - 1)*n + (x + 1)]) {   //右上
		int m = min(n - x - 1, y);
		Bit bit = r_shift(vec[3][m - 1], ((y - m)*n + (x + m)) - (n - 1)) & fri;
		if (bit.any()) {
			int pos = get_last(bit);
			bit = r_shift(vec[3][y - pos / n - 1], pos + (n - 1) - (n - 1));
			if ((bit&empty).none())mask |= bit;
		}
	}
	return mask;
}

// (y, x)に置けるかどうか
int ReversiBoard::can_put(int y, int x, int color) {
	if (y < 0 or y >= n or x < 0 or x >= n or !can[y*n + x])return 0;
	return get_mask(y, x, color).any();
}

// 置けるマスの数(color.count(), -color.count())
P ReversiBoard::can_put_cnt(int color) {
	P res = P(0, 0);
	for (int pos = can.find_first(); pos != can.npos; pos = can.find_next(pos)) {
		if (get_mask(pos / n, pos%n, color).any())res.first++;
		if (get_mask(pos / n, pos%n, -color).any())res.second++;
	}
	return res;
}

// 確定石の一次元処理
Bit ReversiBoard::get_rev(Bit black_s, Bit white_s) {
	Bit rev = Bit(n);
	if ((black_s | white_s).count() == n)return rev.flip();

	vector<int> v;  // 色を格納
	for (int i = 0; i < n;i++) {
		if (black_s[i])v.push_back(BLACK);
		if (white_s[i])v.push_back(WHITE);
		if (!black_s[i] and !white_s[i])v.push_back(EMPTY);
	}

	int l = 0, r = n - 1, color;
	color = v[0];
	if (v[0] != EMPTY) {  // 左からつながりを見る
		for (int i = 0;i < n;i++) { if (color != v[i])break; rev.set(i); }
	}
	color = v[n - 1];
	if (v[n - 1] != EMPTY) {  // 右からつながりを見る
		drep(i, n - 1, 0) { if (color != v[i])break; rev.set(i); }
	}
	return rev;
}

// 確定石の数取得(color.count(), -color.count())
P ReversiBoard::get_establish(int color) {
	Bit black_s(n), white_s(n), rev(n*n);
	vector<int> u[4];
	for (int i = 0; i < n;i++) {
		u[0].push_back((0)*n + (i));  // 上
		u[1].push_back((i)*n + (0));  // 左
		u[2].push_back((i)*n + (n - 1));  // 右
		u[3].push_back((n - 1)*n + (i));  // 下
	}
	for (int i = 0; i < 4;i++) { // 4辺の確定石を見る
		black_s.reset(); white_s.reset();
		for (int j = 0; j < n;j++) {
			if (black[u[i][j]])black_s.set(j);
			if (white[u[i][j]])white_s.set(j);
		}
		Bit res = get_rev(black_s, white_s);
		for (int j = 0; j < n; j++)if (res[j])rev.set(u[i][j]);
	}

	P ans = P((rev&black).count(), (rev&white).count());
	if (color == WHITE)ans = P(ans.second, ans.first);
	return ans;
}

// (y, x)に置いた時の開放度の取得
int ReversiBoard::get_open(int y, int x, int color) {
	Bit mask = get_mask(y, x, color);
	int cnt = 0;
	for (int pos = mask.find_first(); pos != mask.npos; pos = mask.find_next(pos)) {
		cnt += (extension & (sur << (pos / n)*(n + 2) + pos % n)).count();
	}
	return cnt;
}

// (y, x)に置く
void ReversiBoard::put(int y, int x, int color) {
	history.push(History{ black, white, can }); turn_cnt++;
	if (y == -1)return;
	Bit mask = get_mask(y, x, color);
	if (color == BLACK)black.set(y*n + x);
	if (color == WHITE)white.set(y*n + x);
	black ^= mask;
	white ^= mask;
	empty = (black | white).flip();
	can.reset(y*n + x);
	set_extension();
	rep(i, 3)rep(j, 3) {
		int yy = y + (i - 1), xx = x + (j - 1);
		if (yy < 0 or yy >= n or xx < 0 or xx >= n or can[yy*n + xx])continue;
		if (empty[yy*n + xx])can.set(yy*n + xx);
	}
}

// n手戻る
void ReversiBoard::undo() {
	History his = history.top(); history.pop();
	black = his.black_h;
	white = his.white_h;
	empty = (black | white).flip();
	can = his.can_h;
	set_extension();
}