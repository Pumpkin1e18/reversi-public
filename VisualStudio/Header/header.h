#ifndef HEADER
#define HEADER

// game define
#define EMPTY 0
#define BLACK 1
#define WHITE -1

// include
#include <windows.h>
#include <boost/dynamic_bitset.hpp> // サイズを動的に変更するため
#include <cassert>
using namespace std;

// define
#define uint unsigned long long
#define repi(i,m,n) for(int i = m;i < n;i++)
#define drep(i,n,m) for(int i = n;i >= m;i--)
#define rep(i,n) repi(i,0,n)
#define rrep(i,n) repi(i,1,n+1)
typedef pair<int, int> P;
typedef boost::dynamic_bitset<> Bit;
const int INF = 1e9 + 7;

#endif