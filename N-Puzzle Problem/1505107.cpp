/*** from dust i have come, dust i will be ***/

#include<bits/stdc++.h>

#define dbg printf("in\n")
#define nl printf("\n")
#define pp pair<int,int>

using namespace std;

class node
{
public:
    int board[4][4], n;
    int zx, zy;

    node() {}

    node(int n)
    {
        this->n = n;
    }

    void assignNodeValue(node other)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                board[i][j] = other.board[i][j];
        }
    }

    bool operator==(const node &other) const
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (board[i][j] != other.board[i][j])
                    return false;
            }
        }

        return true;
    }

    bool operator<(const node &other) const
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (board[i][j] < other.board[i][j])
                    return true;
                else if (board[i][j] == other.board[i][j])
                    continue;
                else
                    return false;
            }
        }

        return false;
    }

    void print()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                printf("%d ", board[i][j]);

            printf("\n");
        }

        printf("\n");
    }
};

int hammingDist(node u, node g)
{
    int n = u.n, cnt = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (u.board[i][j] && u.board[i][j] != g.board[i][j])
                cnt++;
        }
    }

    return cnt;
}

int manhattanDist(node u, node g)
{
    int cnt = 0;

    vector <pp> gl(u.n * u.n), self(u.n * u.n);

    for (int i = 0; i < u.n; i++)
    {
        for (int j = 0; j < u.n; j++)
        {
            gl[g.board[i][j]] = {i, j};
            self[u.board[i][j]] = {i, j};
        }
    }

    for (int i = 1; i < self.size(); i++)
        cnt += (abs(self[i].first - gl[i].first) + abs(self[i].second - gl[i].second));

    return cnt;
}

int linearConflict(node u, node g)
{
    int cnt=0;
    bool f;
    pp temp;

    set<pp> s;
    //----------------------------------------------------------
    //line->row
    for(int i=0; i<u.n; i++)
    {
        for(int j=0; j<u.n; j++)
        {
            for(int k=j+1; k<u.n; k++)
                s.insert({g.board[i][k],g.board[i][j]});
        }
    }

    for(int i=0; i<u.n; i++)
    {
        for(int j=0; j<u.n; j++)
        {

        }
    }
    //----------------------------------------------------------


    return cnt;
}

int getHeuristicValue(int type, node u, node g)
{
    if (type == 1)
        return hammingDist(u, g);
    else if (type == 2)
        return manhattanDist(u, g);
    else if (type == 3)
        return  manhattanDist(u,g)+2*linearConflict(u, g);
}

bool inside(int r, int c, int n)
{
    if (r >= 0 && c >= 0 && r < n && c < n)
        return true;
    return false;
}

map<node, bool> closedList;
map<node, int> cost;
map <node, node> parent;

int explored;
int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};

void clr()
{
    explored=0;
    closedList.clear();
    cost.clear();
    parent.clear();
}

void A_Star(node startNode, node goalNode, int h)
{
    int x, y, ucost, pcost, hcost;
    priority_queue <pair<int, node>> openList;

    openList.push({0, startNode});

    node dummyParent(0);
    parent[startNode] = dummyParent;
    cost[startNode] = 0;
    explored++;

    while (!openList.empty())
    {
        //node having the lowest "f"
        node u = openList.top().second;

        //remove current from open-list and add to closed-list
        openList.pop();
        closedList[u] = true;
        ucost = cost[u];

        //goal reached
        if (u == goalNode)
            break;

        //traverse the neighbors
        for (int i = 0; i < 4; i++)
        {
            x = u.zx + dx[i];
            y = u.zy + dy[i];

            if (inside(x, y, u.n))
            {
                node newNode(u.n);
                newNode.assignNodeValue(u);
                newNode.zx = x;
                newNode.zy = y;

                swap(newNode.board[x][y], newNode.board[u.zx][u.zy]);

                //if in closed-list then done
                if (closedList[newNode])
                    continue;

                hcost = getHeuristicValue(h, newNode, goalNode);
                pcost = ucost + 1 + hcost;

                //if 0 then a new node has been discovered, else is in the queue already
                //so we push if a better cost is acquired
                if (cost[newNode] == 0 || cost[newNode] > ucost + 1)
                {
                    parent[newNode] = u;
                    cost[newNode] = ucost + 1;
                    openList.push({-pcost, newNode});
                    explored++;
                }
            }
        }
    }

    while(openList.size())
        openList.pop();
}

bool solvable(node u)
{
    int cnt = 0;
    vector<int> vec;

    for (int i = 0; i < u.n; i++)
    {
        for (int j = 0; j < u.n; j++)
        {
            if (u.board[i][j])
                vec.push_back(u.board[i][j]);
        }
    }

    for (int i = 0; i < vec.size() - 1; i++)
    {
        for (int j = i + 1; j < vec.size(); j++)
        {
            if (vec[i] > vec[j])
                cnt++;
        }
    }

    vec.clear();

    //if the board size is odd, then each legal move changes the number of\
    inversions by an even number, thus if it has odd inversion then it is not solvable
    if (u.n % 2)
        {
            if (cnt % 2)
                return false;
            else
                return true;
        }
        else
        {
            //row of the blank
            cnt += u.zx;
            if (cnt % 2)
                return true;
            else
                return false;
        }
}

int main()
{
    freopen("in.txt", "r", stdin);
    //freopen("out.txt","w",stdout);

    int i, j, k;
    int n, m;
    vector <node> path;

    while (1)
    {
        scanf("%d", &n);

        if (n < 3 || n > 4)
            break;

        node startNode(n);
        node goalNode(n);
        node tempNode;

        k = 1;
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                goalNode.board[i][j] = k++;
                scanf("%d", &startNode.board[i][j]);
                if (startNode.board[i][j] == 0)
                    startNode.zx = i, startNode.zy = j;
            }
        }

        goalNode.board[n - 1][n - 1] = 0;
        //cout<<linearConflict(startNode,goalNode)<<endl;

        if (!solvable(startNode))
        {
            printf("puzzle not solvable.\n");
            continue;
        }

        string h[]= {" Hamming\n"," Manhattan\n"," Linear Conflict\n"};

        for(i=0; i<2; i++)
        {
            clr(); path.clear();
            printf("---------------------------------------\n");
            cout<<"heuristic :"<<h[i];

            A_Star(startNode, goalNode, i + 1);

            tempNode=goalNode;
            while (tempNode.n)
            {
                path.push_back(tempNode);
                tempNode = parent[tempNode];
            }

            reverse(path.begin(), path.end());

            for (node p : path)
                p.print();

            printf("analysis--->\n");
            printf("cost : %d\n",path.size()-1);
            printf("nodes expanded : %d\n",closedList.size());
            printf("nodes explored: %d\n",explored);
            printf("---------------------------------------\n");
        }
    }

    return 0;
}
