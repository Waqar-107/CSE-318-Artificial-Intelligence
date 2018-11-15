# from dust i have come, dust i will be

import time

boat_capacity = 2
missionary = None
cannibal = None

ans_found = False
timeout = False

start_time = 0
elapsed_time = 0

goal = None

# after 40 seconds we would stop expanding and assume ans not found
time_threshold = 60
size_threshold = int(1e8)  # max size of nodes

expanded = set()


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
    q = list()
    q.append(current_state)

    global start_time
    start_time = time.time()

    while q:
        current_state = q.pop(0)
        expanded.add(current_state)

        if current_state.goalReached():
            global ans_found
            ans_found = True

            global goal
            goal = current_state

            break

        if len(expanded) >= size_threshold:
            break

        global elapsed_time
        elapsed_time = time.time() - start_time

        if elapsed_time >= time_threshold:
            global timeout
            timeout = True
            break

        dx = [2, 0, 1, 1, 0]
        dy = [0, 2, 1, 0, 1]

        if current_state.d == 0:
            for i in range(5):
                if current_state.ml - dx[i] >= 0 and current_state.cl - dy[i] >= 0:
                    new_state = state(current_state.ml - dx[i], current_state.cl - dy[i], current_state.mr + dx[i],
                                      current_state.cr + dy[i], 1)

                    if (new_state.isValid()) and (new_state not in expanded) and (new_state not in q):
                        q.append(new_state)
                        new_state.parent = current_state

        else:
            for i in range(5):
                if current_state.mr - dx[i] >= 0 and current_state.cr - dy[i] >= 0:
                    new_state = state(current_state.ml + dx[i], current_state.cl + dy[i], current_state.mr - dx[i],
                                      current_state.cr - dy[i], 0)

                    if (new_state.isValid()) and (new_state not in expanded) and (new_state not in q):
                        q.append(new_state)
                        new_state.parent = current_state


def solve():
    global ans_found
    global timeout
    global start_time
    global elapsed_time
    global goal

    ans_found = False
    timeout = False

    bfs()

    elapsed_time = time.time() - start_time
    print("time taken by bfs:", elapsed_time, "nodes expanded: ", len(expanded))
    if ans_found:
        print("solution found")
        path = []

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

            d=1-d
            print(path[i - 1].ml, path[i - 1].cl, path[i - 1].mr, path[i - 1].cr)


    else:
        print("no solution")


if __name__ == "__main__":
    # missionary, cannibal = map(int, input().split())

    missionary = 3
    cannibal = 3
    solve()
