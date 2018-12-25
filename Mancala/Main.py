from Player import Player
from Board import Board
from Constants import *

# required only when minimax algo is used
heuristicNo = None
W1 = None
W2 = None
W3 = None
W4 = None

board = Board()
p1 = Player(1, human, heuristicNo, W1, W2, W3, W4)
p2 = Player(2, human, heuristicNo, W1, W2, W3, W4)

turn = 1
while board.gameOver() == False:
    board.printBoard()
    print("player -", turn, "\'s turn")

    # the selected bin is 0'indexed sent from player
    if turn == 1:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)
