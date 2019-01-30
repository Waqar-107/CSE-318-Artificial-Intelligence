from Player import Player
from Board import Board
from Constants import *

# required only when minimax algo is used
heuristicNo = heuristic4
W1 = 100
W2 = 50
W3 = 0
W4 = 0
depth = 5

board = Board()
p1 = Player(1, alpha_beta_pruning, heuristic2, W1, W2, W3, W4, 9)
p2 = Player(2, alpha_beta_pruning, heuristic2, W1, W2, W3, W4, 9)

turn = 1

# when True is passed to gameOver it will show result
while board.gameOver(True) == False:
    #board.printBoard()
    #print("player -", turn, "\'s turn")

    if turn == 1:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)
