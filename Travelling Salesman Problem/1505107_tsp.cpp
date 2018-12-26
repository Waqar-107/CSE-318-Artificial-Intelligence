/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>

#define dbg printf("in\n");
#define nl printf("\n");
#define SZ 200
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
int vis[SZ];
node nodes[SZ];
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
  cost = mn;

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
  int mn, d, w, w0;

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

  printPath(true);
}
//-----------------------------------------------


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
  clr();

  //make the initial route
  pfs("initial route determined using nearest neighbour heuristic\n");
  nearestNeighbour(src);

  bool f;
  float new_cost;
  float best_cost = printPath(false);

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
        new_cost = printPath(false);

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

  printPath(true);
}
//-----------------------------------------------

//-----------------------------------------------
void threeOpt(int src) {
  clr();

  //make the initial route
  pfs("initial route determined using nearest neighbour heuristic\n");
  nearestNeighbour(src);

  bool f;
  float new_cost;
  float best_cost = printPath(false);

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
          new_cost = printPath(false);
          if (best_cost > new_cost) {
            best_cost = new_cost, f = true;
            break;
          }
          reverse(path.begin() + L, path.begin() + M + 1);
          //--------------------------------------------------------------

          //--------------------------------------------------------------
          //2. reverse J-K
          reverse(path.begin() + J, path.begin() + K + 1);
          new_cost = printPath(false);
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
          new_cost = printPath(false);
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
          new_cost = printPath(false);
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
          new_cost = printPath(false);
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
          new_cost = printPath(false);
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
          new_cost = printPath(false);
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

  printPath(true);
}
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

  pfs("-------------------------------------------\n");
  pfs("3-opt improvement heuristic\n");
  threeOpt(k);
  pfs("-------------------------------------------\n\n");

  return 0;
}
