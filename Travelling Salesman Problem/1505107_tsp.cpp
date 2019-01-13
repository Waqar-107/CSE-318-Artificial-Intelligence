/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>

#define dbg printf("in\n");
#define nl printf("\n");
#define SZ 200
#define W 5
#define inf 1000000000.0

#define sf(n) scanf("%d", &n)
#define sff(n, m) scanf("%d%d",&n,&m)

#define pf(n) printf("%d", n)
#define pfs(s) printf("%s", s)

#define pb push_back
#define pp pair<int, int>
#define ppd pair<double,int>

using namespace std;

struct node {
  double x, y;

  node() {}

  node(double x, double y) {
    this->x = x;
    this->y = y;
  }
};

int n; double cost;
bool vis[SZ];
double savingsTable[SZ][SZ];
node nodes[SZ];
vector<int> path;

int nearestStart[W], savingsStart[W];
double nearestNeighbourCosts[W], savingsCost[W];
vector<int> nearestNeighbourPath[W];
vector<int> savingsPath[W];

void clr() {
  memset(vis, 0, sizeof(vis));
  path.clear();
}

double dist(node u, node v) {
  double d = sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
  return d;
}

int getNearestNode(int src) {
  double mn = inf, d; int v = -1;
  for (int i = 1; i <= n; i++) {
    if (!vis[i]) {
      d = dist(nodes[src], nodes[i]);
      if (d < mn)
        mn = d, v = i;
    }
  }

  //set cost, that is a global variable used in nearest insertion
  cost = mn;

  return v;
}

int getNearestNodeRandomly(int src)
{
  vector<ppd> vec;
  double d; int v = -1;
  for (int i = 1; i <= n; i++) {
    if (!vis[i]) {
      d = dist(nodes[src], nodes[i]);
      vec.pb({d,i});
    }
  }

  sort(vec.begin(), vec.end());

  //W = 5
  int r = W;
  if(vec.size()<W)
    r = vec.size();

  int r2=rand()%r;
  return vec[r2].second;
}

double getCost() {
  int u, v;
  double d = 0.0;
  for (int i = 0; i < n; i++) {
    u = path[i];
    v = path[i + 1];

    d += (dist(nodes[u], nodes[v]));
  }

  return d;
}

void printPath() {
  for (int i = 0; i < n; i++)
    pf(path[i]), pfs("->");

  pf(path[n]), pfs("\n");
  printf("cost : %lf\n", getCost());
}


//-----------------------------------------------
/* construction heuristic : nearest neighbour
 * start from a node randomly chosen
 * go to the nearest unvisited node
 * repeat this till all are visited
 * then finally connect the last node to the path
 */
void nearestNeighbour(int src,int wh) {
  clr();

  int v = src;
  vis[src] = true;
  path.pb(src);

  while (path.size() < n) {
    if(wh==1)
      v = getNearestNode(v);
    else
      v=getNearestNodeRandomly(v);

    vis[v] = true;
    path.pb(v);
  }

  path.pb(src);
  printPath();
}
//-----------------------------------------------


//-----------------------------------------------
/* construction heuristic : insertion methods : cheapest insertion
 * start from a node randomly chosen
 * find (u,v) - an edge in the path constructed so far
 * and also find 'x' an unvisited node so that cost(u,x)+cost(x,v)-cost(u,v) is minimized
 * repeat till all are done
 */
void cheapestInsertion(int src) {
  clr();

  int u, v, x, idx;
  double mn, d;

  //----------------------------
  //i-r-i
  path.pb(src), vis[src] = true;
  v = getNearestNode(src), vis[v] = true, path.pb(v);
  path.pb(src);
  //----------------------------

  while (path.size() <= n) {
    mn = inf;
    x = -1;
    for (int i = 0; i < path.size() - 1; i++) {
      u = path[i];
      v = path[i + 1];

      for (int k = 1; k <= n; k++) {
        if (!vis[k]) {
          d = dist(nodes[u], nodes[k]) + dist(nodes[k], nodes[v]) - dist(nodes[u], nodes[v]);
          if (d < mn)
            mn = d, x = k, idx = i + 1;
        }
      }
    }

    if (x != -1) {
      path.pb(-1);
      for (int i = path.size() - 1; i > idx; i--)
        swap(path[i], path[i - 1]);

      path[idx] = x;
      vis[x] = true;
    }
  }

  printPath();
}

//-----------------------------------------------


//-----------------------------------------------
/* construction heuristic : insertion methods : nearest insertion
 * start from a node randomly chosen
 * find the city v for which dist(u,v) is min and build partial tour (u,v) -> u is the src node where we started
 * selection step - find a node x that is not visited and v that is visited so that cost(x,v) is minimized
 *      -> for all the nodes visited, get nearest node for all of them and select the node that has the least dist
 * insertion step - find (u,v) so that cost(u,x)+cost(x,v)-cost(u,v) is minimized. insert x between u and v
 * repeat until all are traversed
 * https://cs.stackexchange.com/questions/88933/how-does-the-nearest-insertion-heuristic-for-tsp-work
 */
void nearestInsertion(int src) {
  clr();

  int u, v, x;
  double mn, d; int w, w0;

  //----------------------------
  //i-r-i
  path.pb(src), vis[src] = true;
  v = getNearestNode(src), vis[v] = true, path.pb(v);
  path.pb(src);
  //----------------------------

  while (path.size() <= n) {
    //selection step - finding overall closest node
    mn = inf, x = -1;
    for (int i = 1; i <= n; i++) {
      if (vis[i]) {
        v = getNearestNode(i);
        if (cost < mn)
          mn = cost, x = v;
      }
    }

    //insertion step : find where to insert x
    mn = inf, w = -1;
    for (int i = 0; i < path.size() - 1; i++) {
      u = path[i];
      v = path[i + 1];

      d = dist(nodes[u], nodes[x]) + dist(nodes[x], nodes[v]) - dist(nodes[u], nodes[v]);

      if (d < mn)
        mn = d, w = v, w0 = u;
    }

    if (w != -1) {
      path.pb(x);

      for (int i = path.size() - 1;; i--) {
        swap(path[i], path[i - 1]);

        if (path[i] == w)
          break;
      }

      vis[x] = true;
    }
  }

  printPath();
}
//-----------------------------------------------


//-----------------------------------------------
void savingsHeuristic(int src) {
  clr();
  memset(savingsTable,0,sizeof(savingsTable));
 
  //max savings
  pp fr; double mx=0, mx2;
  for(int i = 1; i <= n; i++)
  {
    if(i == src)continue;

    for(int j = i+1; j <= n; j++)
    {
      if(j == src)continue;

      double d = dist(nodes[src], nodes[i]) + dist(nodes[src], nodes[j]) - dist(nodes[i], nodes[j]);
      savingsTable[i][j] = savingsTable[j][i] = d;

      if(d > mx){
        mx=d;
        fr={i,j};
      }
    }
  }

  deque<int> q;
  q.push_back(fr.first); q.push_back(fr.second);
  vis[fr.first] = 1, vis[fr.second] = 1, vis[src] = 1;

  int u,v,x,y;
  while(q.size() < n - 1)
  {
    x=y=-1;
    u=q.front();
    v=q.back();

    //find edge for u
    mx=0;
    for(int i=1;i<=n;i++){
      if(!vis[i])
      {
        if(savingsTable[u][i] > mx)
          mx=savingsTable[u][i], x=i;
      }
    }

    //find edge for v
    mx2=0;
    for(int i=1;i<=n;i++){
      if(!vis[i])
      {
        if(savingsTable[v][i] > mx2)
          mx2=savingsTable[v][i], y=i;
      }
    }

    if(x==y)
    {
      if(mx > mx2)
        vis[x]=1, q.push_front(x);
      else
        vis[y]=1, q.push_back(y);
    }

    else
    {
      vis[x]=1, q.push_front(x);
      vis[y]=1, q.push_back(y);
    }
  }

  q.push_front(src), q.push_back(src);

  for(int e : q)
    path.pb(e);

  printPath();
}


//-----------------------------------------------
void randomizedSavingsHeuristics(int src){
  clr();
  memset(savingsTable,0,sizeof(savingsTable));
  
  pp fr; double mx=0;
  for(int i = 1; i <= n; i++)
  {
    if(i == src)continue;

    for(int j = i+1; j <= n; j++)
    {
      if(j == src)continue;

      double d = dist(nodes[src], nodes[i]) + dist(nodes[src], nodes[j]) - dist(nodes[i], nodes[j]);
      savingsTable[i][j] = savingsTable[j][i] = d;
      
      if(d >= mx){
        mx=d;
        fr={i,j};
      }
    }
  }

  deque<int> q;
  q.push_back(fr.first); q.push_back(fr.second);
  vis[fr.first] = 1, vis[fr.second] = 1, vis[src] = 1;

  int u,v,x,y;
  vector<ppd> vec, wec;

  while(q.size() < n - 1)
  {
    x=y=-1;
    u=q.front();
    v=q.back();

    //find edge for u
    for(int i=1;i<=n;i++){
      if(!vis[i])
        vec.pb({savingsTable[u][i], i});
    }

    //find edge for v
    for(int i=1;i<=n;i++){
      if(!vis[i])
        wec.pb({savingsTable[v][i], i});
    }

    sort(vec.begin(),vec.end(),greater<ppd>());
    sort(wec.begin(),wec.end(),greater<ppd>());

    if(vec.size()>=W){
      x=rand()%W;
      y=rand()%W;
    }

    else{
      x=rand()%vec.size();
      y=rand()%wec.size();
    }

    if(vec[x].second==wec[y].second)
    {
      if(vec[x].first==wec[y].first)
        vis[vec[x].second]=1, q.push_front(vec[x].second);
      else
        vis[wec[y].second]=1, q.push_back(wec[y].second);
    }

    else
    {
      vis[vec[x].second]=1, q.push_front(vec[x].second);
      vis[wec[y].second]=1, q.push_back(wec[y].second);
    }

    vec.clear(), wec.clear();
  }

  q.push_front(src), q.push_back(src);

  for(int e : q)
    path.pb(e);

  printPath();
}


//-----------------------------------------------
/* repeat until no improvement is made
 * start:
 * best_distance = cost of current route
 * for i = 1 to path.end - 1
 *    for k = i + 1 to path.end
 *      2-opt swap
 *      if distance is improved then goto to start
 */
void twoOpt(int src) {
  //clr();

  //make the initial route
  //pfs("initial route determined using nearest neighbour heuristic\n");
  //nearestNeighbour(src, 1);

  bool f;
  double new_cost;
  double best_cost = getCost();

  while (true) {

    f = false;

    for (int i = 1; i < path.size() - 2; i++) {
      for (int k = i + 1; k < path.size() - 1; k++) {
        //two opt-swap
        //step 1: take path[0] to path[i-1]
        //step 2: take path[i] to path[k] in reverse order
        //step 3: take the rest - path[k+1] to the end

        //just reversing will do
        reverse(path.begin() + i, path.begin() + k + 1);
        new_cost = getCost();

        if (new_cost < best_cost) {
          best_cost = new_cost, f = true;
          break;  //as the algo says -> go to start
        } else {
          reverse(path.begin() + i, path.begin() + k + 1);
        }
      }

      if (f)break;
    }

    if (!f)break;
  }

  printPath();
}
//-----------------------------------------------

//-----------------------------------------------
void threeOpt(int src) {
  clr();

  //make the initial route
  pfs("initial route determined using nearest neighbour heuristic\n");
  nearestNeighbour(src, 1);

  bool f;
  double new_cost;
  double best_cost = getCost();

  int I, J, K, L, M, N;

  while (true) {
    f = false;

    // the last and the first node is the starting node so we leave them alone and operate on the other edges
    for (int i = 0; i < path.size() - 1; i++) {
      for (int j = i + 2; j < path.size() - 1; j++) {
        for (int k = j + 2; k < path.size() - 1; k++) {
          I = i, J = i + 1, K = j, L = j + 1, M = k, N = k + 1;
          // the three edges are I-J, K-L and M-N -> there are 2^3-1 solutions
          // (L-M), (J-K), (J-K, L-M), (J-K,L-M,)

          //--------------------------------------------------------------
          //1. reverse L-M
          reverse(path.begin() + L, path.begin() + M + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + L, path.begin() + M + 1);
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //2. reverse J-K
          reverse(path.begin() + J, path.begin() + K + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          //reverse(path.begin() + J, path.begin() + K + 1);
          //same range needs to be reversed in the next variant so comment the above line
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //3. reverse J-K, then L-M
          //we comment out the line below as (J-K) has already been reversed in the previous variant
          //reverse(path.begin() + J, path.begin() + K + 1);
          reverse(path.begin() + L, path.begin() + M + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + L, path.begin() + M + 1);
          reverse(path.begin() + J, path.begin() + K + 1);
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //4. reverse J-M, then L-M, then J-K
          reverse(path.begin() + J, path.begin() + M + 1);
          reverse(path.begin() + L, path.begin() + M + 1);
          reverse(path.begin() + J, path.begin() + K + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + J, path.begin() + K + 1);
          //reverse(path.begin() + L, path.begin() + M + 1);
          //reverse(path.begin() + J, path.begin() + M + 1);
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //5. reverse J-M, then L-M
          //reverse(path.begin() + J, path.begin() + M + 1);
          //reverse(path.begin() + L, path.begin() + M + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + L, path.begin() + M + 1);
          //reverse(path.begin() + J, path.begin() + M + 1);
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //6. reverse J-M, then J-K
          //reverse(path.begin() + J, path.begin() + M + 1);
          reverse(path.begin() + J, path.begin() + K + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + J, path.begin() + K + 1);
          //reverse(path.begin() + J, path.begin() + M + 1);
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //7. reverse J-M
          //reverse(path.begin() + J, path.begin() + M + 1);
          new_cost = getCost();
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + J, path.begin() + M + 1);
          //--------------------------------------------------------------
        }

        if (f)break;
      }

      if (f)break;
    }

    if (!f)break;
  }

  printPath();
}
//-----------------------------------------------

int main() {
  freopen("burma14.tsp", "r", stdin);

  srand(time(NULL));

  int i, j, k;
  double x, y;
  double best, worst;
  int bst1, bst2;

  sf(n);
  for (i = 1; i <= n; i++) {
    cin >> k >> x >> y;
    nodes[k] = {x, y};
  }


  //================================================================================
  //task-1
  //================================================================================

  //nearest neighbor
  best=inf, worst=0;
  for(i=0;i<5;i++)
  {
    k=rand()%n;
    if(k==0)
      k=n;

    nearestStart[i]=k;
    nearestNeighbour(k, 1);

    for(int e : path)
      nearestNeighbourPath[i].pb(e);
    
    nearestNeighbourCosts[i]=getCost();

    best=min(best, nearestNeighbourCosts[i]);
    worst=max(worst, nearestNeighbourCosts[i]);
  }

  //for which node you get the best - required in task-2
  for(i=0;i<5;i++){
    if(nearestNeighbourCosts[i]==best)bst1=nearestStart[i];
  }

  cout<<"Nearest Neighbour Heuristic-> best case:"<<best<<" , worst case:"<< worst<<endl<<endl;

  //savings heuristic
  best=inf, worst=0;
  for(i=0;i<5;i++)
  {
    k=rand()%n;
    if(k==0)
      k=n;

    savingsStart[i]=k;
    savingsHeuristic(k);

    for(int e : path)
      savingsPath[i].pb(e);
    
    savingsCost[i]=getCost();

    best=min(best, savingsCost[i]);
    worst=max(worst, savingsCost[i]);
  }

   //for which node you get the best - required in task-2
  for(i=0;i<5;i++){
    if(savingsCost[i]==best)bst2=savingsStart[i];
  }

  cout<<"Savings Neighbour Heuristic-> best case:"<<best<<" , worst case:"<< worst<<endl<<endl;
  


  //================================================================================
  //task-2
  //================================================================================

  //randomized nearest
  //nearest neighbor
  best=inf, worst=0;
  for(i=0;i<10;i++)
  {
    nearestNeighbour(bst1, 2);
    
    best=min(best, getCost());
    worst=max(worst, getCost());
  }

  cout<<"started from: "<<bst1<<endl;
  cout<<"Randomized Nearest Neighbour Heuristic-> best case:"<<best<<" , worst case:"<< worst<<endl<<endl;
  

  //randomized savings 
  best=inf, worst=0;
  for(i=0;i<10;i++)
  {
    randomizedSavingsHeuristics(bst2);
    
    best=min(best, getCost());
    worst=max(worst, getCost());
  }

  cout<<"started from: "<<bst2<<endl;
  cout<<"Randomized Savings Heuristic-> best case:"<<best<<" , worst case:"<< worst<<endl<<endl;
  

  //================================================================================
  //task3
  //================================================================================
  //2-opt on nearest neighbour
  best=inf, worst=0; 
  for(i=0;i<5;i++)
  {
    path.clear();
    for(int e : nearestNeighbourPath[i])
      path.pb(e);

    twoOpt(nearestStart[i]);

    best=min(best, getCost());
    worst=max(worst, getCost());
  }

  cout<<"Two Opt on  Nearest Neighbour Heuristic-> best case:"<<best<<" , worst case:"<< worst<<endl<<endl;

  //2-otp on savings
  best=inf, worst=0; 
  for(i=0;i<5;i++)
  {
    path.clear();
    for(int e : savingsPath[i])
      path.pb(e);

    twoOpt(savingsStart[i]);

    best=min(best, getCost());
    worst=max(worst, getCost());
  }

  cout<<"Two Opt on  Nearest Neighbour Heuristic-> best case:"<<best<<" , worst case:"<< worst<<endl<<endl;

  return 0;
}
