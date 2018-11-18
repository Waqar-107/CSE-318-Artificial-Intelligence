#define _CRT_SECURE_NO_WARNINGS

/***from dust i have come, dust i will be***/

#include<algorithm>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<functional>
#include<iomanip>
#include<iostream>
#include<map>
#include<numeric>
#include<queue>
#include<set>
#include<stack>
#include<string>
#include<unordered_set>
#include<utility>
#include<vector>

typedef long long int ll;
typedef unsigned long long int ull;

#define dbg printf("in\n");
#define nl printf("\n");
#define N 200138
#define inf 1e18

#define sf(n) scanf("%d", &n)
#define sff(n,m) scanf("%d%d",&n,&m)
#define sfl(n) scanf("%I64d", &n)
#define sffl(n,m) scanf("%I64d%I64d",&n,&m)

#define pf(n) printf("%d",n)
#define pff(n,m) printf("%d %d",n,m)
#define pffl(n,m) printf("%I64d %I64d",n,m)
#define pfl(n) printf("%I64d",n)
#define pfs(s) printf("%s",s)

#define pb push_back
#define pp pair<int,int>

using namespace std;

const int threshold = 1e8;

bool ans_found;
int target, jug1, jug2, mn;

vector<pp> ans;
set<pp> expanded;

void verdict()
{
	if (ans_found)
	{
		for (pp e : ans)
			pff(e.first, e.second), pfs("\n");
	}

	else
		pfs("No Solution\n");

	exit(0);
}

//expand the graph
void waterJug(int x, int y)
{
	if (ans_found) verdict();
	if (expanded.size() >= threshold) verdict();

	if (expanded.count({ x,y }))
		return;

	ans.pb({ x,y }); expanded.insert({ x,y });

	//answer found
	if (x == target || y == target)
		ans_found = true, verdict();

	/*8 possible ways to expand the current state
	jug1 will have greater capacity(will be handled in the code)*/

	//fill jug1 from the unlimited source
	waterJug(jug1, y);

	//fill jug1 from the unlimited source
	waterJug(x, jug2);

	//empty jug1
	waterJug(0, y);

	//empty jug2
	waterJug(x, 0);

	//pour water from jug1 to jug2
	if ((jug2 - y) > 0 && x)
	{
		mn = min(jug2 - y, x);
		waterJug(x - mn, y + mn);
	}

	//pour water from jug2 to jug1
	if ((jug1 - x) > 0 && y)
	{
		mn = min(jug1 - x, y);
		waterJug(x + mn, y - mn);
	}
	
	ans.pop_back();
}

int main()
{
	//freopen("in2.txt", "r", stdin);

	int i, j, k;
	int n, m;

	sff(jug1, jug2); sf(target);
	ans_found = false;

	waterJug(0, 0);

	pfs("No Solution\n");

	return 0;
}
