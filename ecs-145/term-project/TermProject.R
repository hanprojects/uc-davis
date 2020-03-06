# TermProject.R
# ---------------

# exploreShape(x,estMethod,tuning,twoAtATime)
## x: A numeric vector to be graphed.
## estMethod: Either 'hist' or 'density'.
## tuning: The intial value of either breaks or bw.
## twoAtATime: If TRUE, always display the current graph superimposed on the previous one, to aid comparison.

# The code will repeatedly loop around, giving the user the following choices:
## Give a new value of the tuning parameter.
## Zoom in/out, the former meaning to redo the graph over a more narrow range of values of x.
## Run an animation, consisting of the graph as the tuning parameter is varied in small increments from the choppiest/wiggliest to the smoothest.
## Quit. The user will then be asked to specify some values of the tuning parameter for which the code will save the graphs, in forms that they can later be plotted by calling plot().

# The return value of the function will be an S3 object of class 'densEst'. It will include enough information to support generic functions print(), summary() and plot().
# The latter will plot all the saved plots. You can do this one at a time, with keyboard Enter meaning "go to the next plot," or plot them in a lattice format.

# You may place mild restictions on the user, e.g. maximum number of graphs formed.
# Of course, your report should have example sessions, showing the user's input and the graphs that resulted.

# ----------- CODE STARTS AFTER THIS LINE ------------ #

