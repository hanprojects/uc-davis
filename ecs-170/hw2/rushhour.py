## QUICK INSTRUCTIONS ON TERMINAL:
# 
# $ cd 917278789
# $ python3.8
# >>> from rushhour import rushhour
# >>> rushhour(0, ["--B---","--B---","XXB---","--AA--","------","------"])


# IMPORT OTHER FILES
from car import Car
from board import Board
from astar import PriorityQueue
from astar import AStar

# ============================== #
#      rushhour() FUNCTION       #
# ============================== #
def rushhour(heuristic, start):
    board = Board(start,cars=dict(), g=0, h=0, path=[])
    board.parse() # Get information of cars on board

    # Apply A-Star heuristics to solve the board
    a_star = AStar(heuristic, board)
    path, states_explored = a_star.solution(board)
    
    # Print result step by step
    result(path, states_explored)


## (NON-RETURM) FUNCTION to print board, moves and explored states
def result(path, states_explored):
    # Display step by step 
    for state in path:
        state.display() # From class Board
        print("\n")

    # Print total moves and total states explored
    print("Total moves:", len(path) - 1)
    print("Total states explored:", states_explored)


# ================ TEST CASES ================== #
# rushhour(0, ["--B---","--B---","XXB---","--AA--","------","------"])
# rushhour(0, ["--B---","--B---","XXBA--","---A--","------","------"])