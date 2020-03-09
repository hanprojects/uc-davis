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

library(ggplot2)
library(gganimate)
library(dplyr)
library(lattice) # in case use lattice format

## choices()
choices <- function() {
  userInput <- readline(prompt = '1. Change tuning value\n2. Zoom in/out\n3. Run animation\n4. Quit and Save\n-->')
  return(userInput)
}

## 1. changeTuning()
changeTuning <- function() {
  newTuning <- as.integer(readline(prompt = "Enter new tuning value: "))
  return(newTuning)
}

## 2a. zoomIn()
zoomIn <- function() {
}

## 2b. zoomOut()
zoomOut <- function() {
}

## 3. runAnimation()
runAnimation <- function() {
}

## 4. quitSave()
quitSave <- function() {
  break
}

## exploreShape()
exploreShape <- function(x, estMethod, tuning, twoAtATime) {
  
  while(TRUE) {
    ## estMethod cases
    if (twoAtATime == FALSE) {
      if (estMethod == 'density')
        plot(density(x,bw=tuning))
      if (estMethod == 'hist')
       plot(hist(x,breaks=tuning))
    }
    
    userInput <- choices()
    
    if (userInput == '1') {
      newTuning <- changeTuning()
      exploreShape(x, estMethod, newTuning, twoAtATime)
    }
  }
  
  return(densEst)
}
