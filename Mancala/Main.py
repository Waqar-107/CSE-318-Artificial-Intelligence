from Player import Player
from Board import Board
from Constants import *

# required only when minimax algo is used
heuristicNo = heuristic1
W1 = None
W2 = None
W3 = None
W4 = None
depth = 100

board = Board()
p1 = Player(1, human, heuristicNo, W1, W2, W3, W4, depth)
p2 = Player(2, alpha_beta_pruning, heuristicNo, W1, W2, W3, W4, depth)

turn = 1

# when True is passed to gameOver it will show result
while board.gameOver(True) == False:
    board.printBoard()
    print("player -", turn, "\'s turn")

    # the selected bin is 0'indexed sent from player
    if turn == 1:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)
