## LIBRARY IMPORTS
import math

## IMPORT FUNCTIONS FROM OTHER FILES
from staticboardevaluator import static_board_evaluator
from movegen import movegen

# ------------------------------------------ #
#               CLASS MOVETREE               #
# (to set up a tree of possible move boards) #
# ------------------------------------------ #
class MoveTree:
    def __init__(self, board, score=0, current_depth=0, current_move=None):
        self.board = board
        self.score = score
        self.current_depth = current_depth
        self.current_move = current_move


# -------------------------------- #
#          MINIMAX FUNCTION        #
# -------------------------------- #
def minimax(board,player,is_max_turn,current_move,current_depth,max_depth):

    # If reached the max depth assigned in oskaplayer(), get score from static board and return current tree
    if (current_depth == max_depth):
        score = static_board_evaluator(board, player, is_max_turn)
        return MoveTree(board,score,current_depth,current_move)

    # If not, then...
    else:
        # Set player and opponent
        if (player == 'w'):
            opponent = 'b'
        elif (player == 'b'):
            opponent = 'w'

        # Get possible moves from movegen() function
        possible_moves = movegen(board,player)
        
        # If max turn: 
        if (is_max_turn == True):
            best_score = -math.inf # set best score to -inf to compare with real score later on
            best_move = None # best move not found yet

            # For each possible move, get next move and next 2 moves from current depth and current move board
            for move in possible_moves:
                next_move = MoveTree(move,None,current_depth,current_move) # get next move board
                next_2_moves = minimax(move,opponent,not(is_max_turn),next_move,current_depth+1,max_depth) # get next 2 move

                score = next_2_moves.score # calculate score of next 2 moves
                best_score = max(best_score,score) # get the best score to decide the move

                # Assign current best score to next move board 
                if (best_score == score):
                    next_move.score = score
                    best_move = next_2_moves

            return best_move
        
        # If not max turn
        else:
            best_score = math.inf # set best score to +inf to compare with lowest score
            best_move = None # best move not found yet

            # For each move in possible moves, get board of next move and next 2 moves via minimax
            for move in possible_moves:
                next_move = MoveTree(move,None,current_depth,current_move) # get next move board
                next_2_moves = minimax(move,opponent,not(is_max_turn),next_move,current_depth+1,max_depth) # get board of next 2 moves

                score = next_2_moves.score # calculate score of next 2 moves
                best_score = min(best_score,score) # best score is the lowest score (since it's opponent's turn)

                # Assign current best score to next move board 
                if (best_score == score):
                    next_move.score = score
                    best_move = next_2_moves

            return best_move