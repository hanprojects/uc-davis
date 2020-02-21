#! usr/bin/env python
# from gen import get_matrix, get_thresholds

def get_horizontal(matrix, threshold, components):
    for row in matrix:
        found_comp = False
        num_components = 0
        for item in row:
            if item >= threshold:
                found_comp = True
            elif found_comp:
                num_components += 1
                found_comp = False
        if found_comp:
            num_components += 1
        components.append(num_components)
                
def get_vertical(matrix, threshold, components):
    for i in range(0, len(matrix[0])):
        found_comp = False
        num_components = 0
        for j in range(0,len(matrix)):
            if matrix[j][i] >= threshold:
                found_comp = True
            elif found_comp:
                num_components += 1
                found_comp = False
        if found_comp:
            num_components += 1
        components.append(num_components)

# start in bottom left and iterate top left to bottom right
def top_left_to_bottom_right(matrix, threshold, components):
    
    nr = len(matrix)
    nc = len(matrix[0])
    found_comp = False
    # starts at last row and iterates up the rows stopping after the top left corner
    for k in reversed(range(nr)):
        i = k
        j = 0
        num_components = 0
        # while the current row index is above or is the bottom row, go right one down one
        while (i < nr and j < nc):
            if matrix[i][j] >= threshold:
                found_comp = True
            elif found_comp:
                num_components += 1
                found_comp = False
            i += 1
            j += 1
        if found_comp:
            num_components += 1
        found_comp = False
        components.append(num_components)

    # starts at the second col and iterates right across all the cols
    for k in range(1, nc):
        i = 0
        j = k
        num_components = 0
        # while the current col index is left of or is rightmost col, go right one down one
        while (j < nc and i < nr):
            if matrix[i][j] >= threshold:
                found_comp = True
            elif found_comp:
                num_components += 1
                found_comp = False
            i += 1
            j += 1
        if found_comp:
            num_components += 1
        found_comp = False
        components.append(num_components)

# start in top left and iterate bottom left to top right
def top_right_to_bottom_left(matrix, threshold, components):
    # starts at top row and iterates down the rows stopping at the bottom left corner
    nr = len(matrix)
    nc = len(matrix[0])
    found_comp = False
    for k in range(0, nr):
        i = k
        j = 0
        num_components = 0
        # while the current row index is below or is the top row, go right one up one
        while (i >= 0 and j < nc):
            if matrix[i][j] >= threshold:
                found_comp = True
            elif found_comp:
                num_components += 1
                found_comp = False
            i -= 1
            j += 1
        if found_comp:
            num_components += 1
        found_comp = False
        components.append(num_components)


    for k in range(1, nc) :
        i = nr-1
        j = k
        num_components = 0
        # while the current col index is left of or is the rightmost row, go right one up one
        while (j < nc and i >= 0):
            if matrix[i][j] >= threshold:
                found_comp = True
            elif found_comp:
                num_components += 1
                found_comp = False
            i -= 1
            j += 1
        if found_comp:
            num_components += 1
        found_comp = False
        components.append(num_components)




def tda(imgFile,nr,nc,thresholds):
    try:
        fp = open(imgFile)
    except:
        print("File not present")
        return

    matrix = []
    for row in fp.readlines():
        r = []
        for item in row.split(" "):
            try:
                ## Remove newlines in case where conversion fails due to num items in row being too few
                item = item.replace("\n", "")
                num = int(item)
                if num < 0:
                    print("An element in the file is not a nonnegative integer")
                    return
                r.append(num)
            except:
                print("An element in the file is not a nonnegative integer")
                return
        if len(r) != nc:
            print("The number of elements in a line is inconsistent with nr and nc")
            return
        matrix.append(r)
    if len(matrix) != nr:
        print("The number of elements in a line is inconsistent with nr and nc")
        return

    components = []

    for threshold in thresholds:
        get_horizontal(matrix, threshold, components)
        get_vertical(matrix, threshold, components)
        top_left_to_bottom_right(matrix,threshold,components)
        top_right_to_bottom_left(matrix, threshold,components)
    return components


# print(tda("test1.txt",3,4,[10,10,10]))

"""                            CONTRIBUTIONS: 
 “Everyone met together to write the program. Zach propositioned key ideas 
 for the algorithm we used, Atul helped refine the logic, Jamie came up with 
 the method for diagonal iteration, and Han helped catch some key bugs 
 when testing”
 """
