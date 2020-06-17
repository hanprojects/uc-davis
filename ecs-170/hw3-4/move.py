## LIBRARY IMPORTS
import copy


# ----------------------------------- # 
#            MOVE THE PIECE           #
# ----------------------------------- #

def move_piece(board, player, i, next_row, j, next_j):
    # Get copy from board without affecting initial board
    current_state = copy.deepcopy(board)
    
    # Get states of current and next rows
    current_move_state = list(current_state[i]) # Get current row state - make it a list to merge
    next_move_state = list(current_state[next_row]) # Get next row state - make it a list to merge
    
    # Exchange the value (empty space and piece position) after move and merge representation of row into a string
    current_move_state[j] = '-'
    current_move_state = "".join(current_move_state)
    
    next_move_state[next_j] = player
    next_move_state = "".join(next_move_state)

    # Add value of current row and next row of state from current row 1 & 2
    current_state[i] = current_move_state
    current_state[next_row] = next_move_state
    
    return current_state
