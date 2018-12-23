import sys

initialStone = 4


class Board(object):
    def __init__(self):
        self.storage = [0] * 2
        self.bin = [[initialStone] * 6] * 2

    def printBoard(self):
        print('\t', end='')
        for i in range(6):
            sys.stdout.write('%2d ' % self.bin[0][i])

        print()
        sys.stdout.write('%2d ' % self.storage[0])
        for i in range(20):
            print(' ', end='')
        sys.stdout.write('%2d\n' % self.storage[1])

        print('\t', end='')
        for i in range(6):
            sys.stdout.write('%2d ' % self.bin[1][i])
        print('\n')

    def updateBoard(self, playerNo, binNo):
        turn = -1
        tot = self.bin[playerNo][binNo]
        self.bin[playerNo][binNo] = 0

        q = tot / 13
        r = tot % 13

        # starting from binNo go counter-clockwise
        for i in range(6):
            self.bin[0][i] += q
            self.bin[1][i] += 1

        self.storage[playerNo] += q

        st = binNo
        if playerNo == 0:
            # fill own bins
            for i in range(binNo, -1, -1):
                if r > 0:
                    self.bin[playerNo][i] += 1

                    # if the last piece ends up in an empty slot
                    # then we get this one as well as opponents stones in the same slot
                    if r == 1 and self.bin[playerNo][i] == 1:
                        self.storage[playerNo] += (self.bin[1-playerNo][i] + 1)
                        self.bin[playerNo][i] = 0

                    r -= 1
            # fill won storage
            if r > 0:
                self.storage[playerNo] += 1
                if r == 1:
                    turn = playerNo
                r -= 1

            # fill the others
            for i in range(6):
                if r > 0:
                    self.bin[playerNo][i] += 1
                    r -= 1




    def gameOver(self):
        if self.storage[0] + self.storage[1] == initialStone * 6 * 2:
            if self.storage[0] > self.storage[1]:
                print("player-1 wins")
            elif self.storage[0] == self.storage[1]:
                print("tie")
            else:
                print("player-2 wins")

            return True

        return False
