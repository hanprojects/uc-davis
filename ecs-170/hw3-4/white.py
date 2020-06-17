## LIBRARY IMPORTS
import copy

## IMPORT FUNCTIONS FROM OTHER FILES
from move import move_piece
from jump import possible_jumps


# --------------------------------------- # 
#       WHITE PIECE MOVE GENERATION       #
# --------------------------------------- #
def white_move(board, num_row, states):
    # Get number of space in a row
    for i in range(num_row):
        num_space = len(board[i])

        # For every space in a row, if a piece is WHITE, get possible moves and jumps of that piece    
        for j in range(len(board[i])):
            if (board[i][j] == "w"):
                # Get WHITE moves, except for top row and current space
                moves = generate_new_white_moves(board, i, j, num_row-1, num_space-1)
                # Add all items in list "moves" to add to list "states" 
                states.extend(moves)

                # Get WHITE jumps, except for bottom row and current space
                jumps = generate_new_white_jumps(board, i, j, num_row-1, num_space-1)
                # Add all items in list "jumps" to add to list "states"
                states.extend(jumps)


# --------------------------------------- # 
#        GENERATE NEW WHITE MOVES         #
# --------------------------------------- #
def generate_new_white_moves(board, i, j, num_row, num_space):
    states = [] # create empty list of states to store
    
    # If current row is not the top row
    if (i != num_row):
        
        ## If piece is not on the edges 
        if ((j != 0) and (j != num_space)):
                
            # If there is empty space in the next row (diagonally left/right), move the WHITE piece
            if ((board[i+1][j-1] == '-') and (board[i+1][j] == '-')):
                new_state_1 = move_piece(board, "w", i, i+1, j, j-1) # new state of left
                new_state_2 = move_piece(board, "w", i, i+1, j, j) # new state of right
                    
                # Add new states to list "states"
                states.append(new_state_1)
                states.append(new_state_2)

            # If there is only empty space in the next row (diagnonally left), move the WHITE piece
            elif (board[i+1][j-1] == '-'):
                new_state = move_piece(board, "w", i, i+1, j, j-1) # new state of left
                states.append(new_state) # add new state to list "state"

            # If there is only empty space in the next row (diagonally right), move the WHITE piece
            elif (board[i+1][j] == '-'):
                new_state = move_piece(board, "w", i, i+1, j, j) # new state of right
                states.append(new_state) # add new state to list "state"

        ## If current WHITE piece is at the edge of the row
        else:
                
            # TOP HALF
            if (len(board[i]) > len(board[i+1])):
                # If the wHITE piece is located at the farthest left of the row and first space of next row is empty, move the WHITE piece 
                if (j == 0) and (board[i+1][j] == '-'):
                    new_state = move_piece(board, "w", i, i+1, j, j) # new state of move 
                    states.append(new_state) # add new state to list "states"
                    
                # If the WHITE piece is located at the farthest right of the row and last space of next row is empty, move the WHITE piece 
                elif (j == num_space) and (board[i+1][j-1] == '-'):
                    new_state = move_piece(board, "w", i, i+1, j, j-1) # new state of move
                    states.append(new_state) # add new state to list "states"
                
            # BOTTOM HALF
            else:
                # If the WHITE piece is in the edge of the row
                if ((j == 0) or (j == num_space)):
                    
                    # If there is empty space in the next row (diagonally left/right), move the WHITE piece
                    if ((board[i+1][j] == '-') and (board[i+1][j+1] == '-')):
                        new_state_1 = move_piece(board, "w", i, i+1, j, j) # new state of move (left)
                        new_state_2 = move_piece(board, "w", i, i+1, j, j+1) # new state of move (right)
                            
                        states.append(new_state_1) # add new state 1 to "states"
                        states.append(new_state_2) # add new state 2 to "states"

                    # If there is empty space in the next row to the diagonally left, move WHITE piece
                    elif (board[i+1][j] == '-'):
                        new_state = move_piece(board, "w", i, i+1, j, j) # new state of move (left)
                        states.append(new_state) # add new state to "states"

                    # If there is empty space in the next row to the diagonally right, move WHITE piece
                    elif (board[i+1][j+1] == '-'):
                        new_state = move_piece(board, "w", i, i+1, j, j+1) # new state of move (right)
                        states.append(new_state) # add new state to "states"

    return states


# --------------------------------------- # 
#        GENERATE NEW WHITE JUMPS         #
# --------------------------------------- #

## (RETURN) FUNCTION TO GENERATE ALL NEW WHITE JUMPS
def generate_new_white_jumps(board, i, j, num_row, num_space):
    opp = 'b' # opponent is black
    states = [] # create empty list to store generated states

    # If the WHITE piece not in last two rows (requirement for being able to jump)
    if (i < num_row - 1):
        new_state = possible_jumps(board, "w", opp, i, j, num_space, i+1, i+2) # new state of jump / look ahead
        states.extend(new_state) # add items of new state into "states"
    
    return states