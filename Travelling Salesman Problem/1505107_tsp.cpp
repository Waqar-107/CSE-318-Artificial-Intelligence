/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>

typedef long long int ll;
typedef unsigned long long int ull;

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

int n, m, cost;
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


void printPath() {
    int u, v;
    float d = 0;
    for (int i = 0; i < n; i++) {
        u = path[i];
        v = path[i + 1];

        d += sqrt(dist(nodes[u], nodes[v]) * 1.0);
    }

    for (int i = 0; i < n; i++)
        pf(path[i]), pfs("->");

    pf(path[n]);
    pfs("\n");
    printf("cost : %f\n", d);
}


//-----------------------------------------------
/* construction heuristic : nearest neighbour
 * start from a node randomly choosen
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
    printPath();
}
//-----------------------------------------------


//-----------------------------------------------
/* construction heuristic : insertion methods : cheapest insertion
 * start from a node randomly choosen
 *
 * */
//-----------------------------------------------


//-----------------------------------------------
/* construction heuristic : insertion methods : nearest insertion
 * start from a node randomly choosen
 * find the city j for which dist(i,j) is min and build partial tour (i, j)
 * selection step - find a node k that is not visited and j that is visited so that cost(k,j) is minimized
 *      -> for all the nodes visited, get nearest node for all of them and select the node that has the least dist
 * insertion step - find (i, j) so that cost(i,k)+cost(k,j)-cost(i,j) is minimized. insert k between i and j
 * repeat until all are traversed
 * https://cs.stackexchange.com/questions/88933/how-does-the-nearest-insertion-heuristic-for-tsp-work
 */
void nearestInsertion(int src) {
    clr();

    int mn, d, jdx;
    int i, j, k, temp;

    vis[src] = true;
    j = getNearestNode(src);

    vis[j] = true;
    path.pb(src);
    path.pb(j);

    while (path.size() < n) {

        //selection step : an overall nearest node is acquired
        mn = inf;
        k = -1;
        for (int e : path) {
            temp = getNearestNode(e);
            if (cost < mn)
                mn = cost, k = temp;
        }

        //insertion step : find (i,j) edge so that cost(i,k)+cost(k,j)-cost(i,j) is minimized
        //we are going to insert k between (i,j)
        mn = inf;
        jdx = -1;
        for (int idx = 0; idx < path.size(); idx++) {
            i = path[idx];
            if (idx == path.size() - 1)
                j = path[0];
            else
                j = path[idx + 1];

            d = dist(nodes[i], nodes[k]) + dist(nodes[k], nodes[j]) - dist(nodes[i], nodes[j]);
            if (d < mn)
                mn = d, jdx = idx;
        }

        if (jdx != -1) {
            if (jdx == path.size() - 1)
                path.pb(k);
            else {
                path.pb(-1);
                for (int idx = path.size() - 1; idx > jdx + 1; idx--)
                    swap(path[idx], path[idx-1]);

                path[jdx + 1] = k;
            }

            vis[k] = true;
        }
    }

    path.pb(src);
    printPath();
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
    pfs("-------------------------------------------\n");

    pfs("-------------------------------------------\n");
    pfs("nearest neighbour heuristic\n");
    nearestInsertion(k);
    pfs("-------------------------------------------\n");

    return 0;
}
