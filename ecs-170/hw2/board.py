## IMPORT OTHER FILE(S):
from car import Car

# ============================== #
#           CLASS BOARD          #
# ============================== #
class Board:
    ## Necessary items of class Board
    def __init__(self, start, cars, g, h, path):
        self.board = self.board_list(start)
        self.cars = cars
        self.g = g
        self.h = h
        self.path = path


    ## (NON-RETURN) FUNCTION TO GET INFORMATION OF CAR (name, direction, head & tail positions)
    def parse(self):
        # Get explored states based on True/False value
        explored = self.boolean_board()

        # Parse to get car position
        for i in range(0,6):
            for j in range(0,6):
                # If current car position has been explored, ignore and continue with another position
                if explored[i][j]:
                    continue
                
                # If current position is empty, get all posible information of a car (with name)
                if self.board[i][j] != "-":
                    # Car name
                    str = self.board[i][j]

                    # Car direction
                    direction = self.get_direction(i, j, str, explored)

                    # get car size
                    car_size = self.get_car_size(i, j, direction, str, explored)

                    # get head & tail positions of the car
                    front = [i, j]
                    back = self.get_back(i, j, direction, car_size)

                    # store information of car in car (under class Car)
                    car = Car(str, direction, front, back)
                    self.cars[str] = car

                # Set explored to True after the position has been parsed
                explored[i][j] = True
    

# ============= UTILITY FUNCTIONS for CARS ============== #

    # (RETURN) FUNCTION TO GET DIRECTION OF A CAR
    def get_direction(self, row, col, str, explored):
        # Case HORIZONTAL
        if col + 1 < 6 and not explored[row][col + 1] and self.board[row][col + 1] == str:
            return "Horizontal"
        # Case VERTICAL
        elif row + 1 < 6 and not explored[row + 1][col] and self.board[row + 1][col] == str:
            return "Vertical"

    # (RETURN) FUNCTION TO GET THE TAIL POSITION OF THE CAR
    def get_back(self, row, col, direction, length):
        if (direction == "Horizontal"):
            return [row, col + length - 1]
        elif (direction == "Vertical"):
            return [row + length - 1, col]

    # (RETURN) FUNCTION TO GET THE SIZE OF THE CAR
    def get_car_size(self, row, col, direction, str, explored):
        length = 1 # smallest size of car initialization
        
        # Case HORIZONTAL: check length via column
        if (direction == "Horizontal"):
            for j in range(col + 1, 6):
                # No need to get car size if not the right car or car explored
                if self.board[row][j] != str or explored[row][j]:
                    break
                
                length += 1
                explored[row][j] = True
        
        # Case VERTICAL: check length via row
        elif (direction == "Vertical"):
            for i in range(row + 1, 6):
                # No need to get car size if not the right car or car explored
                if self.board[i][col] != str or explored[i][col]:
                    break
                
                length += 1
                explored[i][col] = True

        return length


# ============= OTHER FUNCTIONS ============== #

    ## (RETURN) FUNCTION TO TURN EACH ROW OF BOARD TO A LIST
    def board_list(self, start):
        row_list = []
        for row in start:
            row_list.append(list(row))
        return row_list

    ## (RETURN) FUNCTION TO MAKE A BOOLEAN BOARD FOR EXPLORED USE
    def boolean_board(self):
        boolean_board = []
        for i in range(0,6):
            row = []
            for j in range(0,6):
                row.append(False)
            boolean_board.append(row)
        return boolean_board

    ## (NON-RETURN) FUNCTION TO DISPLAY BOARD IN THE RESULT
    def display(self):
        for row in self.board:
            print("".join(row))
        
    ## (RETURN) CHECK IF THE CAR X HAS REACHED THE STATE (column 2-5, row = 2)
    def is_goal(self):
        car_x = self.cars["X"]
        return car_x.back == [2,5]

    ## (RETURN) CHECK IF A POSITION ON THE BOARD IS "-"
    def is_empty(self, i, j):
        return self.board[i][j] == "-"