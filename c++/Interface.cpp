#include "ReversiBoard.h"
#include "Player.h"
#include "header.h"


//chcp 65001

signed main(){
  printf("●〇REVERSI〇●\n");
  // 盤面の大きさ(4, 6, 8), プレイヤーの選択
  const int n = 8;
  ReversiBoard board(n);
  PlayerHuman player1(n, BLACK);
  PlayerAlphaBeta player2(n, WHITE);
  
  // 変数の宣言
  struct History{int turn, cnt;};
  stack<History> history;
  int turn = BLACK, cnt = 0, act;
  
  while(1){
    if(cnt >= 2)break;
    // 盤面情報表示
    printf("turn cnt:%lld\n", board.turn_cnt);
    printf("turn is %s\n", turn == BLACK ? "〇BLACK〇" : "●WHITE●");
    board.print_board();
    P est = board.get_establish(BLACK);
    printf("Establish B:%lld W:%lld\n", est.first, est.second);
    printf("BLACK:%d, WHITE:%d\n", board.black.count(), board.white.count());
    // プレイヤーの行動取得
    if(turn == BLACK)act = player1.get_act(board);
    if(turn == WHITE)act = player2.get_act(board);
    // undoの処理
    if(act < -1){
      if(history.empty()){printf("error\n\n\n");continue;}
      int size = min(-act-1, (int)history.size());
      printf("undo %lld move\n\n\n", size);
      rep(i,size)board.undo();
      rep(i,size-1)history.pop();
      turn = history.top().turn, cnt = history.top().cnt;
      history.pop();
      continue;
    }
    history.push(History{turn, cnt});
    turn = -turn;
    // パス
    if(act == -1){
      board.put(-1, -1, -1);
      printf("pass\n\n\n");
      cnt++;continue;
    }
    // 置く
    board.put(act/n, act%n, -turn);
    printf("%s", -turn == BLACK ? "BLACK" : "WHITE");
    printf(" puted (%lld %lld)\n\n\n", (act/n)+1, (act%n)+1);
    cnt = 0;
  }
  
  // ゲームが終わった時の表示
  int score_b = board.black.count(), score_w = board.white.count();
  if(score_b == score_w)printf("●〇RESULT〇●\n");
  if(score_b > score_w)printf("〇RESULT〇\n");
  if(score_b < score_w)printf("●RESULT●\n");
  board.print_board();
  printf("BLACK:%lld, WHITE:%lld\n", score_b, score_w);
  if(score_b == score_w)printf("DRAW\n");
  if(score_b > score_w)printf("Winner:BLACK\n");
  if(score_b < score_w)printf("Winner:WHITE\n");
}
