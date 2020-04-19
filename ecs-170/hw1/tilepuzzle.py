# -------------------------------------- #
#       Student: HAN NGUYEN              #
#       SID: 917278789                   #
#       ECS 170 - Homework 1             #
#       Spring 2020 - Prof. Kurt Eiselt  #
# ---------------------------------------#

# $ python3.8
# from tilepuzzle import tilepuzzle
# tilepuzzle([[2,8,3],[1,0,4],[7,6,5]],[[1,2,3],[8,0,4],[7,6,5]])

## LIBRARY IMPORTS:
import copy

## FUNCTION tilepuzzle():
def tilepuzzle(start,goal):
    print(reverse(statesearch([start],goal,[])))
    
## FUNCTION statesearch():
def statesearch(unexplored,goal,path):
    if unexplored == []:
        return []
    elif goal == head(unexplored):
        return cons(goal,path)
    elif goal in tail(unexplored):
        return cons(goal,path)
    
    # If path exceeds 50 steps, return empty:
    if len(path) > 50: 
        return []
        
    else:
        result = statesearch(generateNewStates(head(unexplored)),goal,cons(head(unexplored),path))
        if result != []:
            # check for cycles
            states = []
            for i in result:
              if i not in states:
                states.append(i)
            return states
        else:
            return statesearch(tail(unexplored),goal,path)

## FUNCTION to generateNewUp/Down/Right/Left:
# Up
def Up(currState):
    currState = copy.deepcopy(currState)
    result = []
    for i in range(1,3):
        for j in range(0,3):
            if currState[i][j] == 0:
                currState[i][j] = currState[i-1][j]
                currState[i-1][j] = 0
                result.append(currState)
                break
    return(result)

# Down
def Down(currState):
    currState = copy.deepcopy(currState)
    result = []
    for i in range(0,2):
        for j in range(0,3):
            if currState[i][j] == 0:
                currState[i][j] = currState[i+1][j]
                currState[i+1][j] = 0
                result.append(currState)
                break
    return(result)

# Right
def Right(currState):
    currState = copy.deepcopy(currState)
    result = []
    for i in range(0,3):
        for j in range(0,2):
            if currState[i][j] == 0:
                currState[i][j] = currState[i][j+1]
                currState[i][j+1] = 0
                result.append(currState)
                break
    return(result)

# Left
def Left(currState):
    currState = copy.deepcopy(currState)
    result = []
    for i in range(0,3):
        for j in range(1,3):
            if currState[i][j] == 0:
                currState[i][j] = currState[i][j-1]
                currState[i][j-1] = 0
                result.append(currState)
                break
    return(result)

## FUNCTION generateNewStates():
def generateNewStates(currState):
    states = Up(currState)+Right(currState)+Down(currState)+Left(currState)
    return(states)

## OTHER FUNCTIONS:
def reverse(st):
    return st[::-1]
    
def head(lst):
    return lst[0]

def tail(lst):
    return lst[1:]

def cons(item,lst):
    return [item] + lst
