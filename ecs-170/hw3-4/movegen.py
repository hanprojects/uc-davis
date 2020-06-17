####### INSTRUCTIONS ON TERMINAL ########

# $ cd movegen
# $ python3.8
# $ from movegen import movegen
# $ movegen(['wwww','---','--','---','bbbb'], 'w')

# Please make sure all files are in a same directory


## LIBRARY IMPORTS
import copy

## FILE IMPORTS OF FUNCTIONS
from white import white_move, generate_new_white_moves, generate_new_white_jumps
from black import black_move, generate_new_black_moves, generate_new_black_jumps


# -------------------------------- #
#          MOVE GENERATION         #
# -------------------------------- #
def movegen(board, player):
    num_row = len(board) # Get number of rows
    states = [] # Get empty states to restore possible moves from WHITE or BLACK

    # If player is WHITE, get WHITE moves/jumps
    if (player == 'w'):
        white_move(board, num_row, states)

    # If player is BLACK, get BLACK moves/jumps
    if (player == 'b'):
        black_move(board, num_row, states)

    return states

# =========== TEST CASES ============ #
# movegen(['wwww','---','--','---','bbbb'], 'w')
