# from dust i hav come, dust i will be

with open("in.txt") as f:
    lines = f.readlines()

# remove lf
for i in range(len(lines)):
    if lines[i][len(lines[i]) - 1] == '\n':
        lines[i] = lines[i][:len(lines[i]) - 1]

itr = 0
nodes = int(lines[itr])
itr += 1

sub = {}
color = [0] * nodes
vis = [0] * nodes
adj = [[0 for j in range(nodes)] for i in range(nodes)]


def dfs(s):
    vis[s] = 1

    cs = set()
    for i in range(nodes):
        if i == s:
            continue

        if adj[s][i] == 1:
            cs.add(color[i])

    mn = 1
    while True:
        if mn not in cs:
            break
        mn += 1

    color[s] = mn
    for i in range(nodes):
        if adj[s][i] == 1 and vis[i] == 0:
            dfs(i)


if __name__ == '__main__':

    for i in range(nodes):
        sub[lines[itr]] = i
        itr += 1

    # teachers teaching subjects
    n = int(lines[itr])
    itr += 1

    for i in range(n):
        s = lines[itr].split(' ')
        itr += 1

        for j in range(len(s)):
            for k in range(j + 1, len(s)):
                adj[sub[s[j]]][sub[s[k]]] = 1
                adj[sub[s[k]]][sub[s[j]]] = 1

    # common courses taken by a student
    cmn = int(lines[itr])
    itr += 1
    for i in range(cmn):
        s = lines[itr].split(' ')
        itr += 1

        adj[sub[s[0]]][sub[s[1]]] = 1
        adj[sub[s[1]]][sub[s[0]]] = 1

    '''
        for i in range(nodes):
        for j in range(nodes):
            print(adj[i][j], end=' ')
        print()
    '''


    # csp
    for i in range(nodes):
        if vis[i] == 0:
            dfs(vis[i])

    print(max(color))
