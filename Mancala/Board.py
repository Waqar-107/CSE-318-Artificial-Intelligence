import sys

initialStone = 25


class Board(object):
    def __init__(self):
        self.storage1 = 22
        self.storage2 = 22

        self.bin1 = [initialStone] * 6
        self.bin2 = [initialStone] * 6

    def printBoard(self):
        print('\t', end='')
        for i in range(6):
            sys.stdout.write('%2d ' % self.bin1[i])

        print()
        sys.stdout.write('%2d ' % self.storage1)
        for i in range(20):
            print(' ', end='')
        sys.stdout.write('%2d\n' % self.storage1)

        print('\t', end='')
        for i in range(6):
            sys.stdout.write('%2d ' % self.bin2[i])
        print('\n')
