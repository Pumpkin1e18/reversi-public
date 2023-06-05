#include "header.h"

#ifndef REVERSIBOARD
#define REVERSIBOARD

class ReversiBoard{
public:
  int n, turn_cnt;
  struct History{Bit black_h, white_h, can_h;};
  stack<History> history; // 過去の盤面保存用
  Bit black, white, empty, can, sur, extension;
  vector<Bit> vec[4];  // vec[方向][長さ]
  
  ReversiBoard(); // 初期化
  ReversiBoard(int N);  // 初期化(サイズ指定)
  void set_extension(); // マージン1のBit作成
  void init(int N); // 初期化の処理
  void print_board(int isInfo = 0); // boardの表示
  void print_bit(const Bit &bit); // Bitの二次元表示
  Bit get_mask(int y, int x, int color);  // (y, x)に置いた時の変える位置のBit取得
  int can_put(int y, int x, int color); // (y, x)に置けるかどうか
  P can_put_cnt(int color);  // 置けるマスの数(color.count(), -color.count())
  Bit get_rev(Bit black_s, Bit white_s);  // 確定石の一次元処理
  P get_establish(int color); // 確定石の数取得(color.count(), -color.count())
  int get_open(int y, int x, int color);  // (y, x)に置いた時の開放度の取得
  void put(int y, int x, int color);  // (y, x)に置く
  void undo();  // n手戻る
};

#endif