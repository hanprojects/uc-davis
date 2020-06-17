## LIBRARY IMPORTS
import math


# ------------------------------------------ #
#           STATIC BOARD EVALUATOR           #
#  (to calculate score based on board state) #
# ------------------------------------------ #
def static_board_evaluator(board,player,is_max_turn):
    
    if (is_max_turn == True):
        player_to_evaluate = player
    
    else:
        if (player == 'w'):
            player_to_evaluate = 'b'
        elif (player == 'b'):
            player_to_evaluate = 'w'

    # Get opponent, start end and opposite end based on player name
    if (player_to_evaluate == 'w'):
        opp = 'b'
        start_end = 0
        opposite_end = len(board) - 1
    
    elif (player_to_evaluate == 'b'):
        opp = 'w'
        start_end = len(board) - 1
        opposite_end = 0

    # Get number of pieces of player in a row
    player_piece_pos = {}

    for i in range(len(board)):
        for j in range(len(board[i])):
            if (board[i][j] == player_to_evaluate):
                if i in player_piece_pos:
                    player_piece_pos[i] += 1
                else:
                    player_piece_pos[i] = 1
            
    # Get number of pieces of opponents in a row
    opp_piece_pos = {}

    for i in range(len(board)):
        for j in range(len(board[i])):
            if (board[i][j] == opp):
                if i in opp_piece_pos:
                    opp_piece_pos[i] += 1
                else:
                    opp_piece_pos[i] = 1

    # Set player win point to +inf (Max wins) or -inf (Max loses)
    player_win = math.inf
    player_lose = -math.inf

# =================== WIN CASES ====================== #
    
# Case 1: NO OPPONENT LEFT ON THE BOARD
    if (not opp_piece_pos):
        return player_win

# Case 2: ANY PLAYER'S PIECE REACHES THE OPPOSITE END
    elif (len(player_piece_pos) == 1) and (list(player_piece_pos)[0] == opposite_end):
        return player_win


# =================== LOSE CASES ====================== #

# Case 1: ALL PLAYER'S PIECES ARE OUT OF THE BOARD
    elif (not player_piece_pos):
        return player_lose

# Case 2: ANY OPPONENT'S PIECE REACHES THE PLAYER'S END
    elif (len(opp_piece_pos) == 1) and (list(opp_piece_pos)[0] == start_end):
        return player_lose

# =========================== DRAW CASE =============================== #

    else:
        player_draw = 0 

        for row, num_of_pieces in player_piece_pos.items():
            player_draw += (row+1) * num_of_pieces
        
        for row, num_of_pieces in opp_piece_pos.items():
            player_draw += (row-5) * num_of_pieces

        return player_draw