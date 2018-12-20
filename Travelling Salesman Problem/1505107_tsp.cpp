/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>

#define dbg printf("in\n");
#define nl printf("\n");
#define N 200
#define inf 1000000000

#define sf(n) scanf("%d", &n)
#define sff(n, m) scanf("%d%d",&n,&m)

#define pf(n) printf("%d", n)
#define pfs(s) printf("%s", s)

#define pb push_back

using namespace std;

struct node {
  int x, y;

  node() {}

  node(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

int n, cost;
int vis[N];
node nodes[N];
vector<int> path;

void clr() {
  memset(vis, 0, sizeof(vis));
  path.clear();
}

int dist(node u, node v) {
  return ((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}

int getNearestNode(int src) {
  int mn = inf, v = -1, d;
  for (int i = 1; i <= n; i++) {
    if (!vis[i]) {
      d = dist(nodes[src], nodes[i]);
      if (d < mn)
        mn = d, v = i;
    }
  }

  //set cost, that is a global variable used in nearest insertion
  cost = d;

  return v;
}

float printPath(bool p) {
  int u, v;
  float d = 0;
  for (int i = 0; i < n; i++) {
    u = path[i];
    v = path[i + 1];

    d += sqrt(dist(nodes[u], nodes[v]) * 1.0);
  }

  if (p) {
    for (int i = 0; i < n; i++)
      pf(path[i]), pfs("->");

    pf(path[n]), pfs("\n");
    printf("cost : %f\n", d);
  }

  return d;
}


//-----------------------------------------------
/* construction heuristic : nearest neighbour
 * start from a node randomly chosen
 * go to the nearest unvisited node
 * repeat this till all are visited
 * then finally connect the last node to the path
 */
void nearestNeighbour(int src) {
  clr();

  int v = src;
  vis[src] = true;
  path.pb(src);

  while (path.size() < n) {
    v = getNearestNode(v);

    vis[v] = true;
    path.pb(v);
  }

  path.pb(src);
  printPath(true);
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

  int u, v, x;
  int mn, idx, d;

  path.pb(src);
  vis[src] = true;

  v = getNearestNode(src);
  vis[v] = true;
  path.pb(v);

  path.pb(src);

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

  printPath(true);
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
  int mn, d, idx;

  path.pb(src), vis[src] = true;
  v = getNearestNode(src), vis[v] = true, path.pb(v);
  path.pb(src);

  while (path.size() <= n) {
    //seletion step
    mn = inf;
    for (int i = 1; i <= n; i++) {
      if (vis[i]) {
        v = getNearestNode(i);
        if (cost < mn)
          mn = cost, x = v;
      }
    }

    //insertion step : find where to insert x
    mn = inf, idx = -1;
    for (int i = 0; i < path.size() - 1; i--) {
      u = path[i];
      v = path[i + 1];

      d = dist(nodes[u], nodes[x]) + dist(nodes[x], nodes[v]) - dist(nodes[u], nodes[v]);

      if (d < mn)
        mn = d, idx = i + 1;
    }

    if (idx != -1) {
      path.pb(-1);

      for (int i = path.size(); i > idx; i--)
        swap(path[i], path[i - 1]);

      path[idx] = x;
      vis[x] = true;
    }
  }

  printPath(true);
}
//-----------------------------------------------


//-----------------------------------------------
void twoOpt(int src) {
  clr();

  //make the initial route
  pfs("initial route determined using cheapest insertion heuristic\n");
  nearestInsertion(src);

  bool f;
  float new_cost;
  float best_cost = printPath(false);

  while (true) {

    f = false;

    //the starting node os not eligible to swap
    for (int i = 1; i < path.size() - 1; i++) {
      for (int k = i + 1; k < path.size() - 1; k++) {

        //two opt-swap
        //step 1: take path[0] to path[i-1]
        //step 2: take path[i] to path[k] in reverse order
        //step 3: take the rest - path[k+1] to the end

        //just reversing will do
        reverse(path.begin() + i, path.begin() + k);
        new_cost = printPath(false);

        if (new_cost < best_cost) {
          best_cost = new_cost, f = true;
          break;  //as the algo says -> go to start
        }

        else
          reverse(path.begin() + i, path.begin() + k);
      }

      if (f)break;
    }

    if (!f)break;
  }

  printPath(true);
}
//-----------------------------------------------

//-----------------------------------------------
//-----------------------------------------------

int main() {
  freopen("in.txt", "r", stdin);

  int i, j, k;
  int x, y;

  sf(n);
  for (i = 1; i <= n; i++) {
    sff(x, y);
    nodes[i] = {x, y};
  }

  //k should be random from (1-n)
  k = 1;

  pfs("-------------------------------------------\n");
  pfs("nearest neighbour heuristic\n");
  nearestNeighbour(k);
  pfs("-------------------------------------------\n\n");

  pfs("-------------------------------------------\n");
  pfs("nearest insertion heuristic\n");
  nearestInsertion(k);
  pfs("-------------------------------------------\n\n");

  pfs("-------------------------------------------\n");
  pfs("cheapest insertion heuristic\n");
  cheapestInsertion(k);
  pfs("-------------------------------------------\n\n");

  pfs("-------------------------------------------\n");
  pfs("2-opt improvement heuristic\n");
  twoOpt(k);
  pfs("-------------------------------------------\n\n");

  return 0;
}
