import sys

initialStone = 4
bin_quantity = 6


class Board(object):
    def __init__(self):
        self.storage = [0] * 3
        self.bin = [[initialStone for j in range(bin_quantity + 1)] for i in range(3)]

    def printBoard(self):

        # player2 -> the row is in the upper row
        # in player2's perspective his bin-1 == bin-6 of player1
        print('\t', end='')
        for i in range(bin_quantity, 0, - 1):
            sys.stdout.write('%2d ' % self.bin[2][i])

        # player2's storage is in left, player1's in right
        print()
        sys.stdout.write('%2d ' % self.storage[2])
        for i in range(2 + bin_quantity * 3):
            print(' ', end='')
        sys.stdout.write('%2d\n' % self.storage[1])

        # player1
        print('\t', end='')
        for i in range(1, bin_quantity + 1):
            sys.stdout.write('%2d ' % self.bin[1][i])
        print('\n')

    def updateBoard(self, playerNo, binNo):
        turn = -1
        tot = self.bin[playerNo][binNo]
        self.bin[playerNo][binNo] = 0

        otherPlayer = 1
        if playerNo == 1:
            otherPlayer = 2

        div = bin_quantity * 2 + 1
        q = tot // div
        r = tot % div

        # ------------------------------------------------------
        # each of them would get at least q(possibly 0)
        for i in range(bin_quantity):
            self.bin[1][i] += q
            self.bin[2][i] += q

        self.storage[playerNo] += q
        # ------------------------------------------------------

        # ------------------------------------------------------
        # fill own bins
        print("bin", binNo, "tot", tot, "q", q, "r", r)
        for i in range(binNo + 1, bin_quantity + 1):
            if r > 0:
                self.bin[playerNo][i] += 1
                r -= 1

                # if the last piece ends up in an empty slot
                # then we get this one as well as opponents stones in the same slot
                # note that the opposite slot must not be empty
                if r == 0 and self.bin[playerNo][i] == 1 and \
                                self.bin[otherPlayer][bin_quantity - i + 1] > 0:
                    self.storage[playerNo] += (self.bin[otherPlayer][bin_quantity - i + 1] + 1)
                    self.bin[playerNo][i] = 0
                    self.bin[otherPlayer][bin_quantity - i + 1] = 0

        # print(st1,fn1,inc1,r,playerNo)
        # self.printBoard()
        # fill own storage
        if r > 0:
            self.storage[playerNo] += 1
            r -= 1
            if r == 0:
                turn = playerNo

        # fill the others
        for i in range(1, bin_quantity + 1):
            if r > 0:
                self.bin[otherPlayer][i] += 1
                r -= 1
        # print(st2, fn2, inc2,r, playerNo)
        # if turn is not -1 then the current player has been given chance again
        if turn == -1:
            turn = otherPlayer

        return turn

    def gameOver(self):
        if self.storage[1] + self.storage[2] == initialStone * bin_quantity * 2:
            if self.storage[1] > self.storage[2]:
                print("player-1 wins")
            elif self.storage[1] == self.storage[2]:
                print("tie")
            else:
                print("player-2 wins")

            return True

        return False
