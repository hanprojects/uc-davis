# TermProject.R
# ===============

"""
The goal of this project is to produce an R package that helps a user explore
the effects of setting different values of breaks or bw. The nature of the exploration
will be that the user will be repeatedly asked whether she wishes to change
the current value of the tuning parameter, zoom in/out, etc. After exploration,
the code will save some of the graphs that the user has found useful.

exploreShape(x,estMethod,tuning,twoAtATime)
- x: A numeric vector to be graphed.
- estMethod: Either 'hist' or 'density'.
- tuning: The intial value of either breaks or bw.
- twoAtATime: If TRUE, always display the current graph
superimposed on the previous one, to aid comparison.

The code will repeatedly loop around, giving the user the following choices:
- Give a new value of the tuning parameter.
- Zoom in/out, the former meaning to redo the graph over a more narrow range of values of x.
- Run an animation, consisting of the graph as the tuning parameter is varied
in small increments from the choppiest/wiggliest to the smoothest.
- Quit. The user will then be asked to specify some values of the tuning parameter
for which the code will save the graphs, in forms that they can later be plotted by calling plot().

The return value of the function will be an S3 object of class 'densEst'.
It will include enough information to support generic functions print(), summary() and plot().

The latter will plot all the saved plots. You can do this one at a time,
with keyboard Enter meaning 'go to the next plot,' or plot them in a lattice format.

You may place mild restictions on the user, e.g. maximum number of graphs formed.
Of course, your report should have example sessions, showing the user's input and the graphs that resulted.
"""

# =============== CODE STARTS AFTER THIS LINE =============== #

library(ggplot2)
library(gganimate)
library(dplyr)
library(pdftools)
library(lattice) # in case use lattice format

## choices()
choices <- function() {
  userInput <- readline(prompt = '1. Change tuning value\n2. Zoom in\n3. Zoom out\n4. Run animation\n5. Quit and Save\n--> ')
  return(userInput)
}

## 1. changeTuning()
changeTuning <- function() {
  newTuning <- as.integer(readline(prompt = "Enter new tuning value: "))
  return(newTuning)
}

## 2. zoomIn()
zoomIn <- function(x) {
  print("Specify a subset of graph between 0 and ",length(x))
  print("Start index")
  start_index <- as.integer(readline(prompt="--> "))
  print("End index")
  end_index <- as.integer(readline(prompt="--> "))
  ## create subset
  newX <- x[start_index:end_index]
  return (newX)
}

## 3. zoomOut()
zoomOut <- function(x, zoomed_in_x) { # ERROR: zoomed_in_x cannot be found
  print("Specify a subset of graph between 0 and ", as.character(length(x)))
  
  print("Start index")
  start_index <- as.integer(readline(prompt="--> "))
  print("End index")
  end_index <- as.integer(readline(prompt="--> "))
  
  while ((start_index >= zoomed_in_x[1] & end_index <= zoomed_in_x[length(zoomed_in_x)]) | (start_index > x[1] | end_index < x[length(x)])) {
    print("You are not zooming out! Please try again!")
    print("Start index")
    start_index <- as.integer(readline(prompt="--> "))
    print("End index")
    start_index <- as.integer(readline(prompt="--> "))
  }
  
  zoomed_out_x <- x[start_index:end_index]
  return(zoomed_out_x)
}

## 4. runAnimation()
runAnimation <- function() {
}

## 5. quitSave()
quitSave <- function() {
  break;
}

## exploreShape()
exploreShape <- function(x, estMethod, tuning, twoAtATime) {
  tuning <- as.numeric(tuning)
  list_x <- list()
  list_x[1] <- x
  
  # FALSE case
  if (twoAtATime == FALSE) {
    if (estMethod == 'density')
      plot(density(x,bw=tuning))
    if (estMethod == 'hist')
      plot(hist(x,breaks=tuning))

    while(TRUE) {
      userInput <- choices()
    
      if (userInput == '1') {
        newTuning <- changeTuning()
        exploreShape(x, estMethod, newTuning, twoAtATime)
      }
    
      else if (userInput == '2') {
        zoomed_in_x <- zoomIn(x)
        exploreShape(zoomed_in_x, estMethod, tuning, twoAtATime)
        
      }
    
      else if (userInput == '3') {
        zoomed_out_x <- zoomOut(x, zoomed_in_x)
        exploreShape(zoomed_out_x, estMethod, tuning, twoAtATime)
      }
    
      else if (userInput == '4') {
        # runAnimation()
      }
    
      else if (userInput == '5') {
        quitSave()
      }
    }
  }
  
  else if (twoAtATime == TRUE) {
    if (estMethod == 'density') {
      plot(density(x,bw=tuning))
    }
    if (estMethod == 'hist') {
      plot(hist(x,breaks=tuning))
    }
    
    while(TRUE) {
      userInput <- choices()
      
      # THIS (1) WORKS BUT MESSED UP WITH THE OLD PLOT NUMBERS
      if (userInput == '1') {
        newTuning <- changeTuning()
        oldTuning <- tuning
        if (estMethod == 'density') {
          plot(density(x,bw=oldTuning))
        }
        if (estMethod == 'hist') {
          plot(hist(x,breaks=oldTuning))
        }
        par(new=TRUE)
        exploreShape(x, estMethod, newTuning, twoAtATime)
      }
      
      else if (userInput == '2') {
        zoomed_in_x <- zoomIn(x)
        exploreShape(zoomed_in_x, estMethod, tuning, twoAtATime)
        
      }
      
      else if (userInput == '3') {
        zoomed_out_x <- zoomOut(x, zoomed_in_x)
        exploreShape(zoomed_out_x, estMethod, tuning, twoAtATime)
      }
      
      else if (userInput == '4') {
        # runAnimation()
      }
      
      else if (userInput == '5') {
        quitSave()
      }
    }
  }
}
