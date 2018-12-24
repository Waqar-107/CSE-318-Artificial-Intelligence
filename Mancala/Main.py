from Player import Player
from Board import Board

alpha_beta_pruning = 1
human = 2
heuristic1 = 3
heuristic2 = 4
heuristic3 = 5
heuristic4 = 6
bin_quantity = 6

board = Board()
p1 = Player(1, human)
p2 = Player(2, heuristic1)

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
