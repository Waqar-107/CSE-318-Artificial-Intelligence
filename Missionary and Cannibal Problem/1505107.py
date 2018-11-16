# from dust i have come, dust i will be

import time

boat_capacity = None
missionary = None
cannibal = None

ans_found = False
timeout = False

start_time = 0

goal = None

# after 40 seconds we would stop expanding and assume ans not found
time_threshold = 60
size_threshold = int(1e8)  # max size of nodes

closedList = set()
openList = list()

dx = []
dy = []


class state:
    def __init__(self, ml, cl, mr, cr, d):
        self.ml = ml
        self.cl = cl
        self.mr = mr
        self.cr = cr

        self.d = d
        self.parent = None

    def goalReached(self):
        if self.ml == 0 and self.cl == 0:
            return True
        else:
            return False

    def isValid(self):
        if (self.ml > 0 and self.cl > self.ml) or (self.mr > 0 and self.cr > self.mr):
            return False
        else:
            return True

    def __eq__(self, other):
        return self.ml == other.ml \
               and self.cl == other.cl and self.mr == other.mr \
               and self.cr == other.cr and self.d == other.d

    def __hash__(self):
        return hash((self.ml, self.cl, self.mr, self.cr, self.d))

    def print(self):
        print(self.ml, self.cl, self.mr, self.cr)


def bfs():
    # d=0 for left->right, d=1 for right->left
    current_state = state(missionary, cannibal, 0, 0, 0)
    openList.append(current_state)

    global start_time
    global ans_found
    global goal

    start_time = time.time()

    while openList:
        current_state = openList.pop(0)
        closedList.add(current_state)

        if current_state.goalReached():
            ans_found = True
            goal = current_state
            break

        if len(closedList) >= size_threshold:
            break

        elapsed_time = time.time() - start_time

        if elapsed_time >= time_threshold:
            global timeout
            timeout = True
            break

        if current_state.d == 0:
            for i in range(len(dx)):
                if current_state.ml - dx[i] >= 0 and current_state.cl - dy[i] >= 0:
                    new_state = state(current_state.ml - dx[i], current_state.cl - dy[i], current_state.mr + dx[i],
                                      current_state.cr + dy[i], 1)

                    if (new_state.isValid()) and (new_state not in closedList) and (new_state not in openList):
                        openList.append(new_state)
                        new_state.parent = current_state

        else:
            for i in range(len(dx)):
                if current_state.mr - dx[i] >= 0 and current_state.cr - dy[i] >= 0:
                    new_state = state(current_state.ml + dx[i], current_state.cl + dy[i], current_state.mr - dx[i],
                                      current_state.cr - dy[i], 0)

                    if (new_state.isValid()) and (new_state not in closedList) and (new_state not in openList):
                        openList.append(new_state)
                        new_state.parent = current_state


def dfs(current_state):
    global ans_found
    global goal
    global timeout

    # if goal reached
    if current_state.goalReached():
        ans_found = True
        goal = current_state
        return

    elapsed_time = time.time() - start_time
    if elapsed_time >= time_threshold:
        return

    if len(closedList) >= size_threshold:
        return

    closedList.add(current_state)

    if current_state.d == 0:
        for i in range(len(dx)):
            if timeout or ans_found:
                break

            if current_state.ml - dx[i] >= 0 and current_state.cl - dy[i] >= 0:
                new_state = state(current_state.ml - dx[i], current_state.cl - dy[i], current_state.mr + dx[i],
                                  current_state.cr + dy[i], 1)

                if (new_state.isValid()) and (new_state not in closedList):
                    new_state.parent = current_state
                    dfs(new_state)

    else:
        for i in range(len(dx)):
            if timeout or ans_found:
                break

            if current_state.mr - dx[i] >= 0 and current_state.cr - dy[i] >= 0:
                new_state = state(current_state.ml + dx[i], current_state.cl + dy[i], current_state.mr - dx[i],
                                  current_state.cr - dy[i], 0)

                if (new_state.isValid()) and (new_state not in closedList):
                    new_state.parent = current_state
                    dfs(new_state)


def solve():
    global ans_found
    global timeout
    global start_time
    global goal

    path = []
    # --------------------------------------------------------------------------
    # bfs
    ans_found = False
    timeout = False

    bfs()

    elapsed_time = time.time() - start_time
    print("time taken by bfs:", elapsed_time, " seconds. nodes expanded: ", len(closedList))
    if ans_found:
        print("solution found")
        current_state = goal

        while current_state:
            path.append(current_state)
            current_state = current_state.parent

        d = 0
        for i in range(len(path) - 1, 0, -1):
            print(path[i].ml, path[i].cl, path[i].mr, path[i].cr, end="     ")

            if d == 0:
                print("-->", end="   ")
            else:
                print("<--", end="   ")

            d = 1 - d
            print(path[i - 1].ml, path[i - 1].cl, path[i - 1].mr, path[i - 1].cr)
    else:
        print("no solution")
    # --------------------------------------------------------------------------

    # --------------------------------------------------------------------------
    # dfs
    closedList.clear()
    current_state = state(missionary, cannibal, 0, 0, 0)
    start_time = time.time()
    ans_found = False
    timeout = False
    goal = None

    dfs(current_state)

    elapsed_time = time.time() - start_time
    print("time taken by dfs:", elapsed_time, "seconds. nodes expanded: ", len(closedList))
    if ans_found:
        print("solution found")
        path.clear()

        current_state = goal
        while current_state:
            path.append(current_state)
            current_state = current_state.parent

        d = 0
        for i in range(len(path) - 1, 0, -1):
            print(path[i].ml, path[i].cl, path[i].mr, path[i].cr, end="     ")

            if d == 0:
                print("-->", end="   ")
            else:
                print("<--", end="   ")

            d = 1 - d
            print(path[i - 1].ml, path[i - 1].cl, path[i - 1].mr, path[i - 1].cr)
    else:
        print("no solution")
        # --------------------------------------------------------------------------


if __name__ == "__main__":
    missionary, cannibal, boat_capacity = map(int, input().split())
    # missionary = 3
    # cannibal = 3
    # boat_capacity = 2

    # generate the possible changes and fill up dx and dy
    for i in range(1, boat_capacity + 1):
        for j in range(i + 1):
            dx.append(j)
            dy.append(i - j)

    solve()

'''
https://puzzling.stackexchange.com/questions/55154/when-do-missionaries-and-cannibals-problems-have-solutions
'''
