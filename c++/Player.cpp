#include "ReversiBoard.h"
#include "Player.h"
#include "header.h"

// Random
/*--------------------------------------------------------------------------*/
PlayerRandom::PlayerRandom(int N, int ROLE){  // 初期化
  srand((unsigned int)time(NULL));
  n = N;role = ROLE;
}

int PlayerRandom::get_act(ReversiBoard BOARD){  // 行動取得
  board = BOARD;
  Bit can = board.can;
  vector<int> ans;
  for(int pos = can.find_first();pos != can.npos;pos = can.find_next(pos)){
    if(board.can_put(pos/n, pos%n, role))ans.push_back(pos);
  }
  return ans.size() == 0 ? -1 : ans[rand()%ans.size()];
}
/*--------------------------------------------------------------------------*/



// Human
/*--------------------------------------------------------------------------*/
PlayerHuman::PlayerHuman(int N, int ROLE){  // 初期化
  n = N;role = ROLE;
}

int PlayerHuman::get_act(ReversiBoard BOARD){ // 行動取得
  board = BOARD;
  string str;
  int y = -1, x = -1, size = 0;
  Bit can = board.can;
  for(int pos = can.find_first();pos != can.npos;pos = can.find_next(pos)){
    if(board.can_put(pos/n, pos%n, role))size++;
  }
  
  while(true){
    printf("input:");getline(cin, str);
    y = x = -1;
    if((str == "pass" or str == "PASS") and size == 0)return -1;  // pass
    if(str.substr(0, 4) == "undo" or str.substr(0, 4) == "UNDO"){ // undo
      int num = 0;
      repi(i,4,str.size()){
        if(0 <= str[i]-'0' and str[i]-'0' <= 9)num = num*10+str[i]-'0';
        if(num > n*n)break;
      }
      num = max(num, 1LL);
      return -(num+1);
    }
    rep(i,str.size())if(1 <= str[i]-'0' and str[i]-'0' <= n){ // put
      if(y == -1){y = str[i]-'0';continue;}
      if(x == -1){
        x = str[i]-'0';
        if(board.can_put(y-1, x-1, role))return (y-1)*n+(x-1);
        break;
      }
    }
    printf("invalid operation\n");
  }
  
}
/*--------------------------------------------------------------------------*/




// AlphaBeta
/*--------------------------------------------------------------------------*/
PlayerAlphaBeta::PlayerAlphaBeta(int N, int ROLE){  // 初期化
  n = N;role = ROLE;max_depth = 6;
}

int PlayerAlphaBeta::get_score(int base){
	if(role == BLACK and board.black.none())return -1000;
	if(role == WHITE and board.white.none())return -1000;
  if(last <= 6){
    int score = (int)board.black.count()-(int)board.white.count();
    if(role == WHITE)score = -score;
    return score;
  }
  int score = 0, cnt = 0;P point;
  Bit bit(n*n);bit.set(0);bit.set(n-1);bit.set(n*n-1);bit.set(n*(n-1));
  if(last > 16){
    if(role == BLACK)cnt = (board.white&bit).count();
    if(role == WHITE)cnt = (board.black&bit).count();
    score -= cnt*10;
  }
  // point = board.can_put_cnt(role);
  // score += point.first-point.second;
  point = board.get_establish(role);
  score += 2*(point.first-point.second);
  return base+score;
}

int PlayerAlphaBeta::alphabeta(int alpha, int beta, int color, int depth, int score){
  if(depth == max_depth)return get_score(score);
  
  Bit can = board.can;
  bool visited = false;
  if(color == role){  // 自分の手番
    for(int pos = can.find_first();pos != can.npos;pos = can.find_next(pos)){
      if(!board.can_put(pos/n, pos%n, color))continue;
      int add = board.get_open(pos/n, pos%n, color);
      board.put(pos/n, pos%n, color);
      int res = alphabeta(alpha, beta, -color, depth+1, score-add);
      board.undo();
      if(depth == 0 and alpha < res)best = pos;
      alpha = max(alpha, res);
      visited = true;
      if(alpha >= beta)return alpha;
    }
    if(!visited)return alphabeta(alpha, beta, -color, depth+1, score-5);
    return alpha;
  }else{  // 相手の手番
    for(int pos = can.find_first();pos != can.npos;pos = can.find_next(pos)){
      if(!board.can_put(pos/n, pos%n, color))continue;
      int add = board.get_open(pos/n, pos%n, color);
      board.put(pos/n, pos%n, color);
      int res = alphabeta(alpha, beta, -color, depth+1, score+add);
      board.undo();
      beta = min(beta, res);
      visited = true;
      if(alpha >= beta)return beta;
    }
    if(!visited)return alphabeta(alpha, beta, -color, depth+1, score+5);
    return beta;
  }
}

int PlayerAlphaBeta::get_act(ReversiBoard BOARD){ // 行動取得
  board = BOARD;
  best = -1;
  last = (board.black|board.white).flip().count();
  alphabeta(-INF, INF, role, 0, 0);
  return best;
}
/*--------------------------------------------------------------------------*/

