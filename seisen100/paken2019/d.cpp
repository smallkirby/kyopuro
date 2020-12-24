#include<bits/stdc++.h>

#define IMAX 1<<29
#define PI 3.14159265359

using namespace std;

template <class T>
using Graph = vector<vector<T>>;

/******** Utility ***************/

int bsize(int bits, int _max){
  int c = 0;
  for(int ix=0; ix!=_max; ++ix)
    if((bits&(1<<ix)) !=0){
      ++c;
    }
  return c;
}

long factrial(long n){
  assert(n >= 0);
  if(n <= 1)
    return 1;
  long _sum = 1;
  for(int ix=0; ix!=(n-1); ++ix)
    _sum *= n-ix;
  return _sum;
}

template<typename T>
void show(vector<T> v)
{
  for(auto _v: v)
    cout << _v << " ";
  cout << endl;
}

template<typename T>
void show(vector<T> v, int maxrow)    // 適度に改行入れるver
{
  for(int ix=0; ix!=v.size(); ++ix){
    if(ix!=0 && ix%maxrow==0)
      cout << endl;
    cout << v[ix] << " ";
  }
  cout << endl;
}

template<typename T>
void show2(vector<vector<T>> v)
{
  int ix = 0;
  for(auto __v: v){
    cout << "[" << ix++ << "] (";
    for(auto _v: __v){
      cout << _v << ", ";
    }
    cout << ")" << endl;
  }
}

// G: Graph
// v: 次に訪れる頂点
// d: 最初に頂点を見つけたタイムスタンプ(行きがけ順)
//    -1で初期化されている必要がある
// f: ある頂点の隣接リストを探索し終えた時刻のタイムスタンプ(帰りがけ順)
//    -1で初期化されている必要がある
// timestamp: タイムスタンプ
// @return: vの隣接頂点の探索に要した時間
template<typename T>
void dfs(const Graph<T> &G, int v, vector<T> &d, vector<T> &f, T &timestamp)
{
  d[v] = timestamp++;
  for(auto next_v: G[v]){   // 隣接する頂点を探索
    if(d[next_v] != -1){  // 既に探索済み
      continue;
    }else{
      dfs(G, next_v, d, f, timestamp); // 次の頂点をDFS
    }
  }
  f[v] = timestamp++;
}

// G: Graph
// dist: startからの最小距離
// start: start地点
template<typename T>
void bfs(const Graph<T> &G, vector<int> &dist, const int start)
{
  // init
  queue<int> que;
  dist.assign(dist.size(), -1);
  dist[start] = 0;
  que.push(start);

  // BFS
  while(!que.empty()){
    int v = que.front();
    que.pop();
    for(auto next_v: G[v]){
      if(dist[next_v] != -1)  // 訪問済み
        continue;
      dist[next_v] = dist[v] + 1;
      que.push(next_v);
    }
  }
}

// グラフの周りに余白をつけていること前提
void genGraph(Graph<int> &G, vector<string> &board, int R, int C)
{
  for(int row=1; row!=R-1; ++row){
    for(int col=1; col!=C-1; ++col){
      if(board[row][col] == '#')
        continue;
      if(board[row-1][col] == '.')
        G[(row-1)*C + col].push_back(row*C + col);
      if(board[row][col-1] == '.')
        G[(row)*C + col-1].push_back(row*C + col);
      if(board[row][col+1] == '.')
        G[(row)*C + col+1].push_back(row*C + col);
      if(board[row+1][col] == '.')
        G[(row+1)*C + col].push_back(row*C + col);
    }
  }
}

/******** end of Utility ***************/

int main(void)
{
  // input
  int N;        // 5xNの旗 <=999
  cin >> N;
  string tmpstr;
  vector<vector<int>> S(5, vector<int>(N));     // R=0 B=1 W=2 #=3
  for(int ix=0; ix!=5; ++ix){
    cin >> tmpstr;
    for(int jx=0; jx!=N; ++jx){
      auto c = tmpstr[jx];
      if(c == 'R')
        S[ix][jx] = 0;
      else if(c == 'B')
        S[ix][jx] = 1;
      else if(c == 'W')
        S[ix][jx] = 2;
      else if(c == '#')
        S[ix][jx] = 3;
    }
  }

  // init
  /*
  dp[i][j]は、第i列(0-index)までを塗りつぶし、且つ第i列をj色(0-index)で塗りつぶす時の合計塗り替えマス個数
  i列をj色で塗りつぶせない場合には0を入れて、以降これを伝播する
  */
  vector<vector<int>> dp(N, vector<int>(3, IMAX));    // 黒は要らない
  for(int color=0; color<=2; ++color){
    int _count = 0;
    for(int row=0; row!=5; ++row){
      if(S[row][0] != color)
        ++_count;
    }
    dp[0][color] = _count;
  }

  // main
  for(int col=0; col<N-1; ++col){
    for(int color=0; color<=2; ++color){                    // col+1列目をcolorで塗る
      for(int prev_color=0; prev_color<=2; ++prev_color){   // col列目がprev_colorだったとする
        if(color != prev_color){                            // col+1列目と違う色のときのみカウント
          int C = 0;
          for(int row=0; row!=5; ++row)                     // 塗り替えるマスの個数をカウント
            if(S[row][col+1] != color)
              ++C;
          dp[col+1][color] = min(dp[col+1][color], dp[col][prev_color] + C);
        }
      }
    }
  }

  // print
  //show2(dp);
  cout << *min_element(dp[N-1].begin(), dp[N-1].end()) << endl;
}