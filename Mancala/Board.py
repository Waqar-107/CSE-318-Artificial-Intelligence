import sys
from Constants import *


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
            self.bin[playerNo][i] += q
            self.bin[otherPlayer][i] += q

        self.storage[playerNo] += q

        '''
        special case : if the number of marbles is 13
        then the last piece would fall in the bin where we started
        and we get that piece as well as the piece in the opposite pit
        '''
        if tot == 13:
            self.storage[playerNo] += 1
            self.bin[playerNo][binNo] = 0
            self.storage[playerNo] += self.bin[otherPlayer][bin_quantity - binNo + 1]
            self.bin[otherPlayer][bin_quantity - binNo + 1] = 0
        # ------------------------------------------------------

        # ------------------------------------------------------
        # fill own bins
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

        # if there are still some left, fill own storage from the beginning
        # we started from binNo+1, so finish before it
        for i in range(1, binNo + 1):
            if r > 0:
                self.bin[playerNo][i] += 1
                r -= 1

        # if turn is not -1 then the current player has been given chance again
        if turn == -1:
            turn = otherPlayer

        return turn

    def gameOver(self, showResult):
        # print(sum(self.bin[1][1:bin_quantity + 1]), sum(self.bin[2][1:bin_quantity + 1]), "sums")
        if self.storage[1] + self.storage[2] == initialStone * bin_quantity * 2:
            if showResult:
                self.showGameResult()
            return True

        elif sum(self.bin[1][1:bin_quantity + 1]) == 0:
            self.storage[2] += sum(self.bin[2][1:bin_quantity + 1])

            for k in range(bin_quantity + 1):
                self.bin[2][k] = 0

            if showResult:
                self.showGameResult()
            return True

        elif sum(self.bin[2][1:bin_quantity + 1]) == 0:
            self.storage[1] += sum(self.bin[1][1:bin_quantity + 1])

            for k in range(bin_quantity + 1):
                self.bin[1][k] = 0

            if showResult:
                self.showGameResult()
            return True

        return False

    def checkWinner(self):
        if self.storage[1] > self.storage[2]:
            return 1
        elif self.storage[2] > self.storage[1]:
            return 2
        else:
            return 0

    def showGameResult(self):
        if self.checkWinner() == 1:
            print("player-1 wins")
        elif self.checkWinner() == 2:
            print("player-2 wins")
        else:
            print("tie")

        self.printBoard()

    def copyBoardDetail(self, board):
        self.storage[1] = board.storage[1]
        self.storage[2] = board.storage[2]

        for i in range(bin_quantity + 1):
            self.bin[1][i] = board.bin[1][i]
            self.bin[2][i] = board.bin[2][i]
