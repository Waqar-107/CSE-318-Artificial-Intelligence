alpha_beta_pruning = 1
human = 2
heuristic1 = 3
heuristic2 = 4
heuristic3 = 5
heuristic4 = 6
bin_quantity = 6


class Player(object):
    def __init__(self, playerNo, playerType):
        self.playerNo = playerNo
        self.playerType = playerType

    def getNextMove(self, board):
        if self.playerType == human:
            return self.getHumanMove(board)

        elif self.playerType == heuristic1:
            return self.getHeuristicOne(board)

        elif self.playerType == heuristic2:
            return self.getHeuristicTwo(board)

        elif self.playerType == heuristic3:
            return self.getHeuristicThree(board)

        elif self.playerType == heuristic4:
            return self.getHeuristicFour(board)

    def getHumanMove(self, board):
        while True:
            bn = int(input())

            if bn <= 0 or bn > bin_quantity:
                print("input out of range")
            elif board.bin[self.playerNo][bn] == 0:
                print("empty bin :", bn)
            else:
                return bn

    def getHeuristicOne(self, board):
        return 0

    def getHeuristicTwo(self, board):
        return 0

    def getHeuristicThree(self, board):
        return 0

    def getHeuristicFour(self, board):
        return 0
