# ============= INSTRUCTIONS ON TERMINAL ================ #

# $ cd <path of this folder
# $ cd python3.8
# >>> from oskaplayer import oskaplayer
# >>> oskaplayer(['wwww','---','--','---','bbbb'], 'w', 2)


## IMPORT FUNCTIONS FROM OTHER FILES
from minimax import minimax, MoveTree

# -------------------------------- #
#        OSKAPLAYER FUNCTION       #
# -------------------------------- #

def oskaplayer(board, player, depth):
    # Current board is initial board with information from class MoveTree
    current_board = MoveTree(board)
    # Best move is the best move after some depth calculated by MiniMax
    best_move = minimax(board,player,True,current_board,0,depth)

    # If not reach the end of the tree, best move becomes previous move and continues to explore
    while (best_move.current_depth > 0):
        best_move = best_move.current_move
    
    # When finished, print the board with the best move
    print(best_move.board)


# =========== TEST CASES ============ #
# oskaplayer(['wwww','---','--','---','bbbb'], 'w', 2)
