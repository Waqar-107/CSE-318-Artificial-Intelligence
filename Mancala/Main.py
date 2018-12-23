from Player import Player
from Board import Board

board = Board()
p1 = Player(0,2)
p2 = Player(1,2)

while True:
    # player1
    if board.gameOver():
        break

    print("player1's turn")
    bn = p1.getNextMove(board)

    # player2
    if board.gameOver():
        break

    print("player2's turn")
    bn = p2.getNextMove(board)

