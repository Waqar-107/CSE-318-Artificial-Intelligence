from Board import Board
from Constants import *


class Player(object):
    def __init__(self, playerNo, playerType, heuristicNo, W1, W2, W3, W4):
        self.playerNo = playerNo
        self.playerType = playerType
        self.tempBoard = Board()

        # if alpha-beta pruning is used then these are required
        self.heuristicNo = heuristicNo
        self.W1 = W1
        self.W2 = W2
        self.W3 = W3
        self.W4 = W4

        self.otherPlayer = 1
        if playerNo == 1:
            self.otherPlayer = 2

    def getNextMove(self, board):
        if self.playerType == human:
            return self.__getHumanMove(board)

        elif self.playerNo == alpha_beta_pruning:
            return self.__MiniMax(board)

    def __MiniMax(self, board):
        return 0

    def evaluateNode(self, board):
        return 0

    def __getHumanMove(self, board):
        while True:
            bn = int(input())

            if bn <= 0 or bn > bin_quantity:
                print("input out of range")
            elif board.bin[self.playerNo][bn] == 0:
                print("empty bin :", bn)
            else:
                return bn

    def __HeuristicOne(self, board):
        return board.storage[self.playerNo] - board.storage[self.otherPlayer]

    def __HeuristicTwo(self, board):
        stones_on_my_side = sum(board.bin[self.playerNo][1:bin_quantity + 1])
        stones_on_opponents_side = sum(board.bin[self.otherPlayer][1:bin_quantity + 1])
        stones_in_my_storage = board.storage[self.playerNo]
        stones_in_opponents_storage = board.storage[self.otherPlayer]

        ret = self.W1 * (stones_in_my_storage - stones_in_opponents_storage) + \
              self.W2 * (stones_on_my_side - stones_on_opponents_side)
        return ret

    def __HeuristicThree(self, board):
        additional_move_earned = 0
        return self.__HeuristicTwo(board) + self.W3 * additional_move_earned

    def __HeuristicFour(self, board):
        stones_captured = 0
        return self.__HeuristicThree(board) + self.W4 * stones_captured
