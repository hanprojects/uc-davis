# ============================== #
#           CLASS CAR            #
# ============================== #
class Car:
    # Necessary items of class Car
    def __init__(self, str, direction, front, back):
        self.str = str
        self.direction = direction
        self.front = front
        self.back = back

    
    ## (RETURN) GET SERIES OF MOVES OF A SPECIFIC CAR
    def get_moves(self, current_state):
        # Get head and tail position of a car
        front_0 = self.front[0]
        front_1 = self.front[1]
        back_0 = self.back[0]
        back_1 = self.back[1]

        # list of moves
        moves = []

        # Case 1: When direction is HORIZONTAL, get series of moves in LEFT/RIGHT
        if (self.direction == "Horizontal"):
            # LEFT
            if (front_1 - 1 >= 0) and (current_state.is_empty(front_0, front_1 - 1)):
                moves.append("Left")
            # RIGHT
            if (back_1 + 1 < 6) and (current_state.is_empty(back_0, back_1 + 1)):
                moves.append("Right")

        # Case 2: When direction is VERTICAL, get series of moves in UP/DOWN
        elif (self.direction == "Vertical"):
            # UP
            if (front_0 - 1 >= 0) and (current_state.is_empty(front_0 - 1, front_1)):
                moves.append("Up")
            # DOWN
            if (back_0 + 1 < 6) and (current_state.is_empty(back_0 + 1, back_1)):
                moves.append("Down")

        return moves


    ## (NON-RETURN) FUNCTION TO MOVE CAR IN FOUR DIRECTIONS
    def move_car(self, movement, current_state):
        # Get current board
        board = current_state.board

        # Get position of head and tail of the car
        front_0 = self.front[0]
        front_1 = self.front[1]
        back_0 = self.back[0]
        back_1 = self.back[1]

        # RIGHT
        if (movement == "Right"):
            board[front_0][front_1] = "-"
            board[back_0][back_1 + 1] = self.str
            self.front = [front_0, front_1 + 1]
            self.back = [back_0, back_1 + 1]

        # LEFT
        elif (movement == "Left"):
            board[back_0][back_1] = "-"
            board[front_0][front_1 - 1] = self.str
            self.front = [front_0, front_1 - 1]
            self.back = [back_0, back_1 - 1]

        # UP
        elif (movement == "Up"):
            board[back_0][back_1] = "-"
            board[front_0 - 1][front_1] = self.str
            self.front = [front_0 - 1, front_1]
            self.back = [back_0 - 1, back_1]

        # DOWN
        elif (movement == "Down"):
            board[front_0][front_1] = "-"
            board[back_0 + 1][back_1] = self.str
            self.front = [front_0 + 1, front_1]
            self.back = [back_0 + 1, back_1]