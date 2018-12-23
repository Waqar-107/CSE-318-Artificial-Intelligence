from Player import Player
from Board import Board

board = Board()
p1 = Player(0,2)
p2 = Player(1,2)

turn = 1
while board.gameOver() == False:
    board.printBoard()
    print("player -",turn,"\'s turn")

    # the selected bin is 0'indexed sent from player
    if turn == 1:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)