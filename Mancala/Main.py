from Player import Player
from Board import Board
from Constants import *

# required only when minimax algo is used
heuristicNo = heuristic4
W1 = 30
W2 = 20
W3 = 5
W4 = 5
depth = 5

board = Board()
p1 = Player(1, alpha_beta_pruning, heuristic1, W1, W2, W3, W4, 5)
p2 = Player(2, alpha_beta_pruning, heuristic4, W1, W2, W3, W4, 10)

turn = 1

# when True is passed to gameOver it will show result
while board.gameOver(True) == False:
    board.printBoard()
    print("player -", turn, "\'s turn")

    if turn == 1:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)

'''
some observations...
when using heuristic 2 -> W1 > W2 wins. That means prioritizing "stone diff. of storage"
is better than prioritizing "stone diff. in sides"

when using heuristic 3 -> kept W1 > W2 for both players,  the player having greater
W3 won

'''
