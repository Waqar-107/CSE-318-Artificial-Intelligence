from Player import Player
from Board import Board

board = Board()
p1 = Player(0,2)
p2 = Player(1,2)

turn = 0
while board.gameOver() == False:
    board.printBoard()
    print("player -",turn+1,"\'s turn")

    if turn == 0:
        bn = p1.getNextMove(board)
    else:
        bn = p2.getNextMove(board)

    turn = board.updateBoard(turn, bn)