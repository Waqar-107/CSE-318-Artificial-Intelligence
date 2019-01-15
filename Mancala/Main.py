from Player import Player
from Board import Board
from Constants import *

# required only when minimax algo is used
heuristicNo = heuristic4
W1 = 2
W2 = 3
W3 = 1
W4 = 4
depth = 5

board = Board()
p1 = Player(1, alpha_beta_pruning, heuristic4, W1, W2, W3, W4, 10)
p2 = Player(2, alpha_beta_pruning, heuristic2, W1, W2, W3, W4, 10)

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

'''
https://stackoverflow.com/questions/16656976/how-to-adapt-my-minimax-search-tree-to-deal-with-no-term-based-game
'''