## Package used to generate methods for displaying graphs
#install.packages("gganimate",dependencies=TRUE, repo="http://cran.rstudio.com/")
#install.packages("transformr", dependecies=TRUE, repo="http://cran.rstudio.com/")
library(ggplot2)
library(gganimate)
# library(transformr)


## GENERIC FUNCTIONS

## prints data of S3 object. Print of data vector is optional.
print.densEst <- function(obj, printVector = FALSE) {
   cat("Graph type is:",obj$estMethod,"\n")
   cat("Set of tunings are: ")
   print(obj$tunings)
   if (printVector) {
      cat("Data vector:\n")
      print(obj$dataVector)
   }
}

## plots function. Upon key press will close current graph and open next one
## until all are plotted.
plot.densEst <- function(obj) {
   for (i in 1:length(obj$tunings)) {
      X11()
      if (obj$estMethod == "hist") {
         hist(obj$dataVector, breaks = obj$tunings[i])
      }
      else {
         plot(density(obj$dataVector, bw = obj$tunings[i]))
      }
      readLines(file("stdin"), 1)
      graphics.off()
   }
}

## prints some basic statistical data about the data set, its est method
## and some stats about the density
summary.densEst <- function(obj) {
   cat("Estimation method: ")
   print(obj$estMethod)
   print(paste("Mean of dataset:",mean(obj$dataVector)))
   print(paste("Standard deviation of dataset:",sd(obj$dataVector)))
   print(paste("Mean of bins:",mean(obj$tunings)))
   print(paste("Standard deviation of bins",sd(obj$tunings)))
}


## HELPERS

## when two at a time, allows for displaying graphs two at a time
overlayGraphs <- function(subsetX, estMethod, graph, tuning) {
   X11()
   # DENSITY OVERLAY
   if (estMethod == "density") {
      newGraph <- density(subsetX, bw=tuning)
      currentGraph <- graph
      xlim <- c(-2000,2000)
      ylim <- range(currentGraph$y, newGraph$y)
      plot(currentGraph$x, currentGraph$y, col = 1, lwd = 2, 
           type = "l", xlim = xlim, ylim = ylim)
      lines(newGraph$x, newGraph$y, col = 2, lwd = 2)
   }
     
   # HISTOGRAM OVERLAY
   else if (estMethod == "hist") {
      newGraph <- hist(subsetX, breaks=tuning)
      currentGraph <- graph
      xlim <- range(currentGraph$breaks, newGraph$breaks)
      ylim <- c(0,max(c(currentGraph$count, newGraph$count)))
      plot(currentGraph, col = rgb(173,216,230,max = 255, alpha = 80, 
           names = "lt.blue"), xlim = xlim, ylim = ylim)
      plot(newGraph, add=TRUE, 
           col = rgb(255,192,203, max = 255, alpha = 80, 
                     names = "lt.pink"))
   }
   ## wait for keystroke before closing
   buffer <- readLines(file("stdin"), 1) 
} 

## returns S3 object that represents list of data with defined estMethod that
## can be displayed with a variety of tunings
getFinalGraphs <- function(x, estMethod) {
   ## S3 object requires 3 components:
   ## 1) The dataset x
   ## 2) The estimation method
   ## 3) The set of tunings
   
   thisEnv <- environment()
   
   ## get tuning list
   tunings <- c()
   while (TRUE) {
      print("Enter a tuning parameter you want to use for this graph or q to quit")
      tuning <- readLines(file("stdin"), 1)
      if (tuning == "q") {
         break
      }
      else if (is.na(as.integer(tuning))) {
         print("Tuning must be a non-negative integer")
      }
      else {
         tunings <- c(tunings, as.integer(tuning))
      }
   }

   ## create S3 obj
   obj <- list (
      dataVector = x,
      tunings = tunings,
      estMethod= estMethod
   )

   assign('this',obj,envir=thisEnv)
   class(obj) <- "densEst"
   return(obj)
}

## grabs values and runs animations based on supplied 
runAnimation <- function(x, estMethod) {
   print("Specify a starting tuning value ")
   start_tuning <- as.integer(readLines(file("stdin"), 1))
   print("Specify a ending tuning value")
   end_tuning <- as.integer(readLines(file("stdin"), 1))

   X11()
   if (estMethod == 'hist') {
      h <- hist(x, breaks = start_tuning, plot = FALSE)
      df <- as.data.frame(cbind(ds = start_tuning, xs = h$mids, ys = h$counts))

      for (i in start_tuning+1:end_tuning) {
        h2 <- hist(x, breaks = i, plot = FALSE)
        df2 <- as.data.frame(cbind(ds = i, xs = h2$mids, ys = h2$counts))
        df <- rbind(df, df2)
        df <- as.data.frame(df)
      }
      plot <- ggplot(data=df, aes(x=xs, y=ys)) +  
              geom_bar(stat="identity") + 
              transition_states(ds)
      show(plot)    
   }
   else {
      d1 <- density(x, bw =  start_tuning)
      d2 <- density(x, bw = end_tuning)
      df1 <- as.data.frame(cbind(ds = 1, xs = d1$x, ys = d1$y * d1$n))
      df2 <- as.data.frame(cbind(ds = 2, xs = d2$x, ys = d2$y * d2$n))
      m <- merge(x = df1, y = df2, all = TRUE)
      plot <- ggplot(m, aes(x = xs, y = ys)) + 
              geom_line() + 
              transition_states(ds)
      show(plot)
   }
}

## returns boolean based on if vec args is greater than start, less than end
subset <- function(vec,start,end) 
{
   if (vec >= start & vec <= end) return(TRUE) else return(FALSE)
}

getSubset <- Vectorize(subset)

## grabs subset of graph and returns it
zoomIn <- function(x, subsetX) {
   valid_index <- 0
   while(valid_index == 0)
   {
      print(paste("Specify a subset of graph between",range(subsetX)[1],
      "and",range(subsetX)[2]))
      print("Starting value")
      start_index <- as.integer(readLines(file("stdin"), 1))
      if (start_index <= range(subsetX)[1])
      {
         print("Invalid. Starting value should be larger.")
         next
      }
      print("Ending value")
      end_index <- as.integer(readLines(file("stdin"), 1))
      if(end_index >= range(subsetX)[2])
      {
         print("Invalid. Ending value should be smaller.")
         next
      }
      if(end_index == start_index)
      {
         cat("Invalid range. Max value cannot be the same as min value. Please reenter indices.\n")
         next
      }
      else if (end_index < start_index)
      {
         cat("Invalid range. Max value cannot be smaller than min value. Please reenter indices.\n")
         next
      }
      valid_index <- 1
   }
   ## display graph of subset
   return(x[getSubset(x,start_index,end_index)])
}

## handles zooming out graph
zoomOut <- function(x, subsetX) 
{
   if(range(x)[1] == range(subsetX[1]) && range(x)[2] == range(subsetX)[2])
   {
      print("Cannot zoom out. Graph is in original state.")
      return(x)
   }
   else
   {
      print("Do you want to zoom out to original graph? [y/n]")
      response <- readLines(file("stdin"),1)
      if (response == "y" || response == "Y") 
      {
         return(x)
      }
      else if (response == "n" || response == "N") 
      {
         valid_index <- 0
         while (valid_index == 0)
         {
            print(paste("Specify a subset of graph between",range(x)[1],
            "and",range(x)[2]))
            print("Start index")
            start_index <- as.integer(readLines(file("stdin"), 1))
            if (start_index >= range(subsetX)[1]) 
            {
               print("Invalid. Starting index should be smaller.")
               next
            }
            print("End index")
            end_index <- as.integer(readLines(file("stdin"), 1))
            if (end_index <= range(subsetX)[2])
            {
               print("Invalid. Ending index should be larger.")
               next
            }
            if(end_index == start_index)
            {
               cat("Invalid range. Max value cannot be the same as min value. Please reenter indices.\n")
               next
            }
            else if (end_index < start_index)
            {
               cat("Invalid range. Max value cannot be smaller than min value. Please reenter indices.\n")
               next
            }
         valid_index <- 1
         }
         ## display graph of subset
         return(x[getSubset(x,start_index,end_index)])
      }
   }
}

## grabs and returns new tuning paremeter from user. Validates input before
## returning.
getNewTuningParameter <- function() {
   while(TRUE) {
      print("Provide a new tuning paremeter")
      tuning <- as.integer(readLines(file("stdin"), 1))
      if (is.na(tuning) || tuning < 1) {
         print("Tuning must be a non-negative integer")
      }
      else {
         return(tuning)
      }
   }
}

## Grab user input to see what action they want to do. Returns integer that 
## corresponds with requested action. Input guaranteed to be valid, as function
## will validate input and ask user until valid input provided.
grabUserRequest <- function() {
   while(TRUE) {
      print("What do you want to do?")
      print("(1) Change tuning parameter")
      print("(2) Zoom in on graph")
      print("(3) Zoom out of graph")
      print("(4) Run an animation on the currently selected list")
      print("(5) Quit and specify which graphs you want to save.")
      print("(6) See graph again.")
      input <- readLines(file("stdin"), 1)

      if (is.na(as.integer(input)) || as.integer(input) < 1 ||
          as.integer(input) > 6) {
         print("Invalid choice...")
      }
      else {
         return(as.integer(input))
      }
   }
}

## Used to explore potential curves of a dataset, allowing for iterative testing
## by the user. Will save graphs that user wants.
##
## Args:
## x (Vector): numeric vector to be graphed
## estMethod (string): Type of method. Should be 'hist' or 'density'
## tuning: breaks or bw, based on estMethod
exploreShape <- function(x,estMethod,tuning,twoAtATime) {
   ## subset of graph we are graphing. Be default is the entire graph
   subsetX <- x   

   while (TRUE) {
      ## Allow user to view plot until they close the window
      X11()
      print("Displaying graph...")
      if (estMethod == 'hist') {
         graph <<- hist(subsetX, breaks = tuning) # global instance allows overlays
         plot(graph)
      }
      else {
         graph <<- density(subsetX, bw = tuning)
         plot(graph)
      }
      
      ## once closed, see what the user want to do
      input <- grabUserRequest()
      graphics.off() 
      if (input == 1) {
         tuning <- getNewTuningParameter()

         ## handle plotting overlays
         if (twoAtATime) {
            overlayGraphs(subsetX, estMethod, graph, tuning)  
         }
      }
      else if (input == 2) {
         subsetX <- zoomIn(x, subsetX)
      }
      else if (input == 3) {
         subsetX <- zoomOut(x, subsetX)       
      }
      else if (input == 4) {
         runAnimation(x, estMethod)
         while (!is.null(dev.list())) Sys.sleep(1)
      }
      else if (input == "5") {
         return(getFinalGraphs(x, estMethod));
      }
      else if (input == 6) {
         next
      }
   }
}

obj <- exploreShape(c(1:100), 'density', 5, TRUE)

summary(obj)
