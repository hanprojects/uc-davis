## LIBRARY IMPORTS
import copy

## IMPORT FUNCTIONS FROM OTHER FILES
from move import move_piece
from jump import possible_jumps


# --------------------------------------- # 
#       BLACK PIECE MOVE GENERATION       #
# --------------------------------------- #
def black_move(board, num_row, states):
    # Get number of space in a row
    for i in range(num_row-1,-1,-1): # loop from the bottom
        num_space = len(board[i])

        # For every space in a row, if a piece is BLACK, get possible moves and jumps of that piece
        for j in range(len(board[i])):
            if (board[i][j] == "b"):
                # Get BLACK moves, except for top row and current space
                moves = generate_new_black_moves(board, i, j, 0, num_space-1)
                # Add all items in list "moves" to add to list "states"
                states.extend(moves)

                # Get BLACK jumps, except for bottom row and current space
                jumps = generate_new_black_jumps(board, i, j, num_row-1, num_space-1)
                # Add all items in list "jumps" to add to list "states"
                states.extend(jumps)

# --------------------------------------- # 
#        GENERATE NEW BLACK MOVES         #
# --------------------------------------- #
def generate_new_black_moves(board, i, j, num_row, num_space):
    states = []
    
    # If current row is not the bottom row
    if (i != num_row):

        ## If piece is not on the edges 
        if ((j != 0) and (j != num_space)):
            
            # # If there is empty space in the next row (diagonally left/right), move the BLACK piece
            if ((board[i-1][j-1] == '-') and (board[i-1][j] == '-')):
                new_state_1 = move_piece(board, "b", i, i-1, j, j-1) # new left
                new_state_2 = move_piece(board, "b", i, i-1, j, j) # new state right

                # Add new states to "states"
                states.append(new_state_1)
                states.append(new_state_2)

            # If there is only empty space in the next row (diagonally left), move the BLACK piece
            elif (board[i-1][j-1] == '-'):
                new_board = move_piece(board, "b", i, i-1, j, j-1)
                states.append(new_board)

            # If there is only empty space in the next row (diagonally right), move the BLACK piece
            elif (board[i-1][j] == '-'):
                new_state = move_piece(board, "b", i, i-1, j, j)
                states.append(new_state)

        # ## If current BLACK piece is at the edge of the row
        else:
           
            # TOP HALF
            if (len(board[i]) > len(board[i-1])):
                # If the BLACK piece is located at the farthest left of the row and first space of next row is empty, move the BLACK piece
                if (j == 0) and (board[i-1][j] == '-'):
                    new_state = move_piece(board, "b", i, i-1, j, j)
                    states.append(new_state)
                
                # If the BLACK piece is located at the farthest right of the row and last space of next row is empty, move the BLACK piece
                elif (j == num_space) and (board[i-1][j-1] == '-'):
                    new_state = move_piece(board, "b", i, i-1, j, j-1)
                    states.append(new_state)
                
            # BOTTOM HALF
            else:
                # If the BLACK piece is in the edge of the row
                if ((j == 0) or (j == num_space)):
                    
                    # If there is empty space in the next row (diagonally left/right), move the BLACK piece
                    if ((board[i-1][j] == '-') and (board[i-1][j+1] == '-')):
                        new_state_1 = move_piece(board, "b", i, i-1, j, j) # new state of move (left)
                        new_state_2 = move_piece(board, "b", i, i-1, j, j+1) # new state of move (right)
                            
                        ## Add new states to "states"
                        states.append(new_state_1)
                        states.append(new_state_2)

                    # If there is empty space in the next row (only diagonally left), move BLACK piece
                    elif (board[i-1][j] == '-'):
                        new_state = move_piece(board, "b", i, i-1, j, j)
                        states.append(new_state)

                    # If there is empty space in the next row (only diagonally right), move BLACK piece
                    elif (board[i-1][j+1] == '-'):
                        new_state = move_piece(board, "b", i, i-1, j, j+1)
                        states.append(new_state)

    return states


# --------------------------------------- # 
#        GENERATE NEW BLACK JUMPS         #
# --------------------------------------- #
def generate_new_black_jumps(board, i, j, num_row, num_space):
    opp = 'w' # opponent is WHITE
    states = [] # create empty list to store states

    # If the WHITE piece not in last two rows (requirement for being able to jump)
    if (i > 1):
        new_state = possible_jumps(board, "b", opp, i, j, num_space, i-1, i-2) # new state of jump / look ahead
        states.extend(new_state) # add items of new state into "states"

    return states
