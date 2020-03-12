from SimPy.Simulation import *
from random import Random, expovariate, uniform

class G:  # globals
   Rnd = Random(12345)
   elevProc = None  # elevator process
   passProc = None  # passenger process

## Passenger class
class passClass(Process):
   arrvs = [] ## queue of arrivals
   numPassTot = 0 ## total number of passengers
   
   def __init__(self):
      Process.__init__(self) ## start the Process
      self.passArrvRate = float(sys.argv[1]) ## get passenger arrival rate from input

   def Run(self):
      while True:
        ## generate a new passenger (add to the number of total passengers)
        passClass.numPassTot += 1
        ## append passenger arrival time to rows
        passClass.arrvs.append(now())
        ## passenger has arrived so if elevator was asleep, reactivate 
        if elevClass.dead:
            reactivate(G.elevProc)
        ## yield a random exponential arrival rate of passenger
        yield hold, self, G.Rnd.expovariate(self.passArrvRate)

## Elevator Class
class elevClass(Process):
    waitTime = 0
    overCap = 0.0 ## over capacity
    lenArr = 0 ## length of arrivals
    dead = False
    numElevTot = 0 ## number of total elevators

    def __init__(self):
        Process.__init__(self)
        self.elevCap = int(sys.argv[2])
        ## to get lambda l, we can simply divide by mean
        ## since E(x) = 1/l -> 1/E(x) = l for exponential distribution, which means to get mean elevator return time, we have needed to do 1/(total return time)
        self.elevRetTime = 1/float(sys.argv[3]) ## calculate the elevator return time by 1/E(x), E(x) = mean elevator return time

    def Run(self):
        while True:
            elevClass.lenArr = len(passClass.arrvs) ## length of arrivals = length of the total passenger arrivals
            elevClass.numElevTot += 1 ## when elevator runs, increment the number of total elevators

            ## elevator is in transit

            ## if no passangers waiting, make elevator sleep
            if elevClass.lenArr == 0: # no passenger arrivals at all
                elevClass.dead = True # set elevator dead mode to TRUE
                yield passivate, self # have the Elevetor process wait until awakened by Process of passenger
                elevClass.dead = False # set elevator dead mode back to FALSE

            ## there are more passengers waiting than elevator capacity, then increment the number of over-capacity time:
            if elevClass.lenArr > self.elevCap:
                elevClass.overCap += 1

            ## We use min because if we hit the
            ## elevator cap, we don't want to keep deleting arrivals.
            ## We want those arrivals to wait until elevator returns.
            for i in range(min(self.elevCap, elevClass.lenArr)):
                elevClass.waitTime += now() - passClass.arrvs[0] ##  wait time is cumm sum of
                  ## current time - time of each arrival
                del passClass.arrvs[0]

            ## yield a random exponential gen time
            yield hold, self, G.Rnd.expovariate(self.elevRetTime)

def main():
    ## init processes
    initialize()
    G.passProc = passClass()
    G.elevProc = elevClass()
    
    ## run simulation
    activate(G.passProc, G.passProc.Run()) ## activate the Passenger Process and Run
    activate(G.elevProc, G.elevProc.Run()) ## activate the Elevator Process and Run
    MaxSimtime = float(sys.argv[4]) ## get maximum simulation time from input
    simulate(until=MaxSimtime) ## simulate the process until maximum simulation time

    # print results
    print "mean passenger wait:", (elevClass.waitTime / passClass.numPassTot) ## get the mean passenger wait (waitTime/totalPassengers)
    print "prop. of visits that leave passengers behind:", (elevClass.overCap / elevClass.numElevTot) ## the proportion of elevators visits that leave passengers behind
      ## due to overcapacity = (number of overcapacity)/(number of elevators)

if __name__ == "__main__": main() # Call main function
