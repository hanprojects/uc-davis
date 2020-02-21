#! usr/bin/env python

# from gen import get_matrix, get_thresholds --> TEST CASES

# FUNCTION to get number of components by horizontal based on threshold:
def get_horizontal(matrix, threshold, components): # mD array, list, list
    
    # Loop each row (item left -> right)
    for row in matrix:
        found_comp = False # set found_comp to False as default
        num_components = 0 # initialize number of components to 0 since the components are not found yet
        
        # Inner-loop each item of each row:
        for item in row:
            # if the item of the rown is larger or equal to the assigned threshold,
            # it means we found a component -> set found_comp to True
            if item >= threshold:
                found_comp = True
            # if we already found a component from the previous loop,
            # increase the number of components,
            # and set the found_comp back to False for the next item
            elif found_comp:
                num_components += 1
                found_comp = False
        # END OF INNER-LOOP
        
        # if the last loop of found_comp is True, increase number of components by 1
        if found_comp:
            num_components += 1
        
        # add the number of components to list "components":
        components.append(num_components)

# FUNCTION to get number of components by vertical based on assigned threshold: 
def get_vertical(matrix, threshold, components): # mD array, list, list
    
    # Loop each column i in matrix (up -> down by using 2-dimension array):  
    for i in range(0, len(matrix[0])):
        found_comp = False # set found_comp to False as default
        num_components = 0 # initialize number of components to 0 since no components
        
        # Inner-loop each item of each column i:
        for j in range(0,len(matrix)):
            # if the item is larger/equal than the assigned threshold, 
            # set we find a component -> so set found_comp to True 
            if matrix[j][i] >= threshold:
                found_comp = True
                
            # if the next item is not larger than the threshold, 
            # but the previous item is, increase the number of components by 1
            # and set found_comp back to False for the next item check
            elif found_comp:
                num_components += 1
                found_comp = False
        # END OF INNER-FOR-LOOP
                
        # if found_comp from the last check is True, 
        # increase the number of components by 1
        if found_comp:
            num_components += 1
        
        # add the number of components to the list "components"
        components.append(num_components)

# FUNCTION to do the NW-SE diagonal
# start in bottom left and iterate top left to bottom right
def top_left_to_bottom_right(matrix, threshold, components): # mD array, list, list
    nr = len(matrix) # set number of rows to length of matrix
    nc = len(matrix[0]) # set number of columns to length of a row
    found_comp = False # set found_comp to False as default
    
    # (first small-big half) Loop each row k from last to first row
    for k in reversed(range(nr)):
        i = k # start from the last row
        j = 0 # start from the first column
        num_components = 0 # initialize the number of components to 0
        
        # Inner-while the current row and column is not out of range
        while (i < nr and j < nc):
            # if an item based on the NW-SE diagonal is larger/equal to the assigned threshold, 
            # set found_comp to True
            if matrix[i][j] >= threshold:
                found_comp = True
            # else if we already found a component from the previous item, 
            # but not just now, increase the number of components by 1
            # and set the found_comp back to False
            elif found_comp:
                num_components += 1
                found_comp = False
            # go to next item of NW-SE diagonal by increase [row,col] combo by 1:
            i += 1
            j += 1
            # END OF INNER-WHILE
      
        # if the last check we have a component,
        # increase the number of components by 1
        if found_comp:
            num_components += 1
        found_comp = False # set found_comp to False for the next use
        components.append(num_components) # add the number of components to the list "components" 
    # END OF 1st FOR LOOP

    # (second big-small half) Loop each row from the first row to last row || second column to last column
    for k in range(1, nc):
        i = 0 # start from first row
        j = k # based on k (first k is 1)
        num_components = 0 # initialize number of components to 0
        
        # Inner-while when i and j are not out of range: 
        while (j < nc and i < nr):
            # if each item of the row based on the second-half of the NW-SE is larger/equal to the threshold, 
            # then we found a component
            if matrix[i][j] >= threshold:
                found_comp = True
            # else, if the current item is not larger than the threshold
            # but the previous item is, increase the num_components by 1
            elif found_comp:
                num_components += 1
                found_comp = False # set found_comp back to False for future use
            # go to the next diagonal, and keep doing the inner-while loop
            i += 1
            j += 1
        # END OF INNER-WHILE
        
        # if the last check we have a component (found_comp = True),
        # increase the number of components by 1
        if found_comp:
            num_components += 1
        found_comp = False # set found_comp back to False for future use
        components.append(num_components) # add the number of components to list "components"
    # END OF 2nd FOR LOOP
# END OF NW-SE FUNCTION
    
# FUNCTION to do the NE-SW diagonal
def top_right_to_bottom_left(matrix, threshold, components):
    # start from top left
    nr = len(matrix) # number of rows = length of the matrix
    nc = len(matrix[0]) # number of columns = length of a row
    found_comp = False # set found_comp = False as default
    
    # (first small-big half) Loop each row i from first to last, column from left to right
    for k in range(0, nr):
        i = k # row i is based on k, initialized to 0
        j = 0 # column j is initialized to 0
        num_components = 0 # initialize the number of components to 0
        
        # Inner-While when i and j are not out of range:
        while (i >= 0 and j < nc):
            # when the item of the diagonal is larger/equal to the threshold, 
            # we found a component
            if matrix[i][j] >= threshold:
                found_comp = True
            # else, if the current item is not larger/equal to the threshold, 
            # but the previous item is, then increase the number of components by 1
            elif found_comp:
                num_components += 1
                found_comp = False # set found_comp to False for future use 
            # the reason to decrease i is because it jumps back and forth the previous row
            # when going to the item of the next column
            i -= 1 # i can be out of range at first, but not out of for loop
            j += 1
        # END OF INNER-WHILE
        
        # if the last check we have a component, increase num_comp by 1
        if found_comp:
            num_components += 1
        found_comp = False # set found_comp to False for future use
        components.append(num_components) # add the number of components of list "components"
    # END OF 1st FOR LOOP
    
    # (second big-small half) Loop each column k from second to last || from last row to first row
    for k in range(1, nc) :
        i = nr-1 # start from last row
        j = k # start from second
        num_components = 0 # initialize number of components to 0
        
        # Inner-while loop when i and j is not out of range: 
        while (j < nc and i >= 0):
            # if the item is larger/equal to threshold, 
            # we found a component
            if matrix[i][j] >= threshold:
                found_comp = True
            # else, if the current item is not larger/equal to the threshold,
            # but the previous item is, increase the num comp by 1
            elif found_comp:
                num_components += 1
                found_comp = False # set found_comp to False for future use default
            # the reason to decrease i is because it jumps back and forth the previous row
            # when going to the item of the next column
            i -= 1 # i can be out of range at first, but not get out of for loop
            j += 1
        # END OF INNER-WHILE
        
        # if the last check we have a component, increase the num comp by 1
        if found_comp:
            num_components += 1
        found_comp = False # set found_comp to False as default for future use
        components.append(num_components) # add number of components to list "components"
    # END OF 2nd FOR LOOP       
# END OF NE-SW FUNCTION 

# (RETURN / TDA) FUNCTION to return full list of component counts [a series of elements that we have appended]
def tda(imgFile,nr,nc,thresholds): # (md Array, number of rows, number of columns, threshold lists)
    try:
        fp = open(imgFile) # try to open imgFile
    except:
        print("File not present") # print out File not found if cannot open file
        return # return nothing since it is a return function

    matrix = [] # initialize matrix to empty mD array
    
    # Loop each row of the matrix in the imgFile
    for row in fp.readlines():
        r = [] # initialize item of row to empty list
        
        # Inner-for loop each item in each current row item-list removed by the " "
        for item in row.split(" "):
            try:
                # Remove newlines in case where conversion fails due to num items in row being too few
                item = item.replace("\n", "")
                
                num = int(item) # set the integer version of an item in row
                
                if num < 0:
                    print("An element in the file is not a nonnegative integer")
                    return
                
                r.append(num) # append the item of row to list r
            
            except:
                print("An element in the file is not a nonnegative integer")
                return
        # END OF INNER-FOR LOOP
        
        # If the number of items in r is not equal to number of columns,
        # Print the error message
        if len(r) != nc:
            print("The number of elements in a line is inconsistent with nr and nc")
            return # return nothing

        matrix.append(r) # add r to matrix --> create new matrix for one line
    # END OF MAIN FOR-LOOP
    
    # if the length of the matrix is not equal to number of rows,
    # print error message
    if len(matrix) != nr:
        print("The number of elements in a line is inconsistent with nr and nc")
        return # return nothing

    components = [] # initialize list "components" to empty list
    
    # for each item of threshold in list "thresholds", call functions
    # to append number of components to the initialized "components" list
    for threshold in thresholds:
        get_horizontal(matrix, threshold, components)
        get_vertical(matrix, threshold, components)
        top_left_to_bottom_right(matrix,threshold,components)
        top_right_to_bottom_left(matrix, threshold,components)
        
    return components # return components
# END OF THE (RETURN) TDA FUNCTION

# print(tda("test1.txt",3,4,[10,10,10]))

"""                            CONTRIBUTIONS: 
 “Everyone met together to write the program. Zach propositioned key ideas 
 for the algorithm we used, Atul helped refine the logic, Jamie came up with 
 the method for diagonal iteration, and Han helped catch some key bugs 
 when testing”
 """
