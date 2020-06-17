## LIBRARY IMPORTS
import copy


# ----------------------------------- # 
#            JUMP THE PIECE           #
# ----------------------------------- #
def jump_piece(board, player, i, j, opponent_pos, jump_pos, next_row, next_2_rows):
    # Get copy from board without affecting initial board
    current_state = copy.deepcopy(board)
    
    # Get states of current row, next row and next 2 rows
    current_jump_state = list(current_state[i]) # before move
    next_jump_state = list(current_state[next_row])
    next_2_jump_state = list(current_state[next_2_rows]) # look ahead
    
    # Exchange the value (empty space and piece position) after move and merge representation of row into a string
    current_jump_state[j] = '-'
    current_jump_state = "".join(current_jump_state)

    next_jump_state[opponent_pos] = '-'
    next_jump_state = "".join(next_jump_state)

    next_2_jump_state[jump_pos] = player
    next_2_jump_state = "".join(next_2_jump_state)

    # Add value of current row and next row of state from current row 1 & 2 and look-ahead row
    current_state[i] = current_jump_state
    current_state[next_row] = next_jump_state
    current_state[next_2_rows] = next_2_jump_state
    
    return current_state


# ----------------------------------- # 
#       GENERATE POSSIBLE JUMPS       #
# ----------------------------------- #

## (RETURN) FUNCTION TO GET POSSIBLE JUMPS AT BOTH HALVES (WHITE HALF-TOP OR BLACK HALF-BOTTOM):
def possible_jumps(board, player, opp, i, j, num_space, next_row, next_2_rows):
    states = []

    # BOTTOM HALF / Get possible jumps
    if (len(board[i]) > len(board[next_row])):
        states = possible_jumps_bottom_half(board, player, opp, i, j, num_space, next_row, next_2_rows)

    # TOP HALF / Get possible jumps
    elif (len(board[i]) < len(board[next_row])):
        states = possible_jumps_top_half(board, player, opp, i, j, num_space, next_row, next_2_rows)

    return states


# ----------------------------------- # 
#   POSSIBLE JUMPS FROM BOTTOM HALF   #
# ----------------------------------- #
def possible_jumps_bottom_half(board, player, opp, i, j, num_space, next_row, next_2_rows, states=[]):
    
    ## If the piece is not on the edges 
    if ((j != 0) and (j != num_space)):
        # If the piece is closest to the farthest left edge and jump space is empty, get new state of jump
        if (j - 1 == 0) and (board[next_row][j] == opp) and (board[next_2_rows][j] == "-"):
            new_state = jump_piece(board, player, i, j, j, j, next_row, next_2_rows)
            states.append(new_state)

        # If piece is clost to the farthest right edge and jump space is empty, get new state of jump
        elif (j + 1 == num_space) and (board[next_row][j-1] == opp) and (board[next_2_rows][j-2] == "-"):
            new_board = jump_piece(board, player, i, j, j-1, j-2, next_row, next_2_rows)
            states.append(new_board)
            
        # If the piece is located anywhere else in the middle of the row
        else:
            # If the next row pieces (left/right) are opponent and look-ahead row position is empty, jump the piece
            if (board[next_row][j-1] == opp and board[next_row][j] == opp) and (board[next_2_rows][j-2] == "-" and board[next_2_rows][j] == "-"):
                new_state = jump_piece(board, player, i, j, j-1, j-2, next_row, next_2_rows) # new state of opponent left
                new_state_2 = jump_piece(board, player, i, j, j, j, next_row, next_2_rows) # new state of opponent right

                # Add new states to "states"
                states.append(new_state)
                states.append(new_state_2)
            
            # If the next row piece is opponent (left only) and look-ahead row is empty, jump the piece
            elif (board[next_row][j-1] == opp) and (board[next_2_rows][j-2] == "-"):
                new_state = jump_piece(board, player, i, j, j-1, j-2, next_row, next_2_rows)
                states.append(new_state)
            
            # If the next row piece is opponent (right only) and look-ahead row is empty, jump the piece
            elif (board[next_row][j] == opp) and (board[next_2_rows][j] == "-"):                    
                new_state = jump_piece(board, player, i, j, j, j, next_row, next_2_rows)
                states.append(new_state)

    ## If the piece is located at the furthest left edge of the row
    elif (j == 0):
        # If the space of the next row on the right is opponent and look-ahead space is occupied, jump the piece
        if (board[next_row][j] == opp) and (board[next_2_rows][j] == "-"):
            new_state = jump_piece(board, player, i, j, j, j, next_row, next_2_rows)
            states.append(new_state)                    

    ## If the piece is located at the furthest right edge of the row
    elif (j == num_space):
        # If the space of the next row on the left is opponent and look-ahead space is occupied, jump the piece
        if (board[next_row][j-1] == opp) and (board[next_2_rows][j-2] == "-"):
            new_state = jump_piece(board, player, i, j, j-1, j-2, next_row, next_2_rows)
            states.append(new_state)
    
    return states


# ----------------------------------- # 
#     POSSIBLE JUMPS FROM TOP HALF    #
# ----------------------------------- #
def possible_jumps_top_half(board, player, opp, i, j, num_space, next_row, next_2_rows, states=[]):
    
    # If next row right/left pieces are opponent and look-ahead right/left pieces are empty, jump the piece
    if (board[next_row][j] == opp) and (board[next_row][j+1] == opp) and (board[next_2_rows][j] == "-") and (board[next_2_rows][j+2] == "-"):
        new_state = jump_piece(board, player, i, j, j, j, next_row, next_2_rows) # new state for right pieces
        new_state_2 = jump_piece(board, player, i, j, j+1, j+2, next_row, next_2_rows) # new states for left pieces
        
        # Add new states to state
        states.append(new_state)
        states.append(new_state_2)

    # If next row space (right only) is opponent and look-ahead space (right only) is empty, jump the piece
    elif (board[next_row][j] == opp) and (board[next_2_rows][j] == "-"):
        new_state = jump_piece(board, player, i, j, j, j, next_row, next_2_rows)
        states.append(new_state)

    # If next row space (left only) is opponent and look-ahead space (left only) is empty, jump the piece
    elif (board[next_row][j+1] == opp) and (board[next_2_rows][j+2] == "-"):
        new_state = jump_piece(board, player, i, j, j+1, j+2, next_row, next_2_rows)
        states.append(new_state)

    return states