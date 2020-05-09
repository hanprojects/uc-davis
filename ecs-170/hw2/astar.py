## LIBRARY IMPORTS
import copy
import heapq
import math

## IMPORT OTHER FILE(S):
from board import Board
from car import Car


# ============================== #
#      CLASS PRIORITY-QUEUE      #
# ============================== #
class PriorityQueue:
    # Create a new heap
    def __init__(self, new_heap = []):
        self.new_heap = new_heap  # turn into max heap by making priority negative
        self.node = 0

    # FUNCTION TO PUSH A NODE TO HEAP
    def push(self, state, prior_number):
        heapq.heappush(self.new_heap, (prior_number, self.node, state))
        self.node += 1 # increment 1 node after push

    # FUNCTION TO POP (REMOVE) A NODE FROM HEAP
    def pop(self):
        return heapq.heappop(self.new_heap)

    # FUNCTION TO GET THE SIZE OF THE HEAP
    def get_size(self):
        return len(self.new_heap)


# ============================== #
#          CLASS A-STAR          #
# ============================== #
class AStar:
    # Necessary items for A-Star algorithm
    def __init__(self, heuristic, start, path = []):
        self.frontier = PriorityQueue()
        self.generated_states = [start.board]
        self.heuristic = heuristic
    

    ## (NON-RETURN) FUNCTION TO SOLVE THE GAME BY USING A-STAR ALGORITHM
    def solution(self, start):
        # Get the heuristic (heuristic = 0 or 1)
        self.heuristic_choice(self.heuristic, start)
        
        # Calculate f(n) = g(n) + h(n) and push to start
        f = start.g + start.h
        self.frontier.push(start, f)

        # Add start to path in case start == goal:  
        start.path.append(start)

        # While there is still frontier, explore new states
        explored = 0
        while (self.frontier.get_size()):
            # when reach to current state, remove frontier at row 2
            current_state = self.frontier.pop()[2]
            explored += 1

            # if the current state is goal, return path
            if (current_state.is_goal()):
                return current_state.path, explored

            # otherwise, generate new states from current state, and continue to calculate f(n) = g(n) + h(n)
            else:
                new_states = self.generate_new_states(current_state)
                for st in new_states:
                    st.g = current_state.g + 1
                    self.heuristic_choice(self.heuristic, st)
                    f = st.g + st.h

                    # get path of new state = current path 
                    st.path = copy.deepcopy(current_state.path)
                    # add new state to path
                    st.path.append(st)
                    # add f to frontier of the new state
                    self.frontier.push(st, f)
    

    ## (RETURN) FUNCTION TO GENERATE NEW STATES
    def generate_new_states(self, current_state):
        new_states = []

        # For each car, get car's moves
        for name in current_state.cars:
            car = current_state.cars[name]

            moves = car.get_moves(current_state)

            # for each move, do the movement of car for the new state
            if (moves):
                for move in moves:
                    new_state = copy.deepcopy(current_state)
                    new_car = new_state.cars[name]
                    new_car.move_car(move, new_state)

                    # if the new state is not in the already generated states
                    if (new_state.board not in self.generated_states):
                        # add to new states to explore later
                        new_states.append(new_state)
                        self.generated_states.append(new_state.board)

        return new_states
    

    ## (NON-RETURN) FUNCTION TO CHOOSE A HEURISTICS TO SOLVE THE GAME
    def heuristic_choice(self, heuristic, current_state):

        # BLOCKING HEURISTICS
        if (heuristic == 0):
            board = current_state.board
            back_1 = current_state.cars["X"].back[1]
        
            blocking_cars = set()

            for col in range(back_1 + 1, 6):
                if (board[2][col] != "-" and board[2][col] not in blocking_cars):
                    blocking_cars.add(current_state.board[2][col])

            current_state.h = len(blocking_cars) + 1

        # MANHATTAN DISTANCE HEURISTICS
        elif (heuristic == 1):
            pass