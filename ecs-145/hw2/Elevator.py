from SimPy.Simulation import *
from random import Random, expovariate, uniform

class G:  # globals
   Rnd = Random(12345)
   elevProc = None  # elevator process
   passProc = None  # passenger process

class passClass(Process):
   arrvs = [] ## queue of arrivals
   numPassTot = 0 ## total number of passengers
   def __init__(self):
      Process.__init__(self)
      self.passArrvRate = float(sys.argv[1])

   def Run(self):
      while True:
        ## generate a new passenger
        passClass.numPassTot += 1
        ## append arrival time to rows
        passClass.arrvs.append(now())
        ## passenger has arrived so if elevator was asleep, reactivate 
        if elevClass.dead:
            reactivate(G.elevProc)
        ## yield random arrival
        yield hold, self, G.Rnd.expovariate(self.passArrvRate)

class elevClass(Process):
    waitTime = 0
    overCap = 0.0
    lenArr = 0
    dead = False
    numElevTot = 0

    def __init__(self):
        Process.__init__(self)
        self.elevCap = int(sys.argv[2])
        ## to get lambda l, we can simply divide by mean
        ## since E(x) = 1/l -> 1/E(x) = l for exponential distribution
        self.elevRetTime = 1/float(sys.argv[3])

    def Run(self):
        while True:
            elevClass.lenArr = len(passClass.arrvs)
            elevClass.numElevTot += 1

            ## elevator is in transit

            ## if no passangers waiting, make elevator sleep
            if elevClass.lenArr == 0:
                elevClass.dead = True
                yield passivate, self
                elevClass.dead = False

            ## there are more passengers waiting than capacity
            if elevClass.lenArr > self.elevCap:
                elevClass.overCap += 1

            ##  wait time is cumm sum of current time - time of 
            ##  each of arrival. We use min because if we hit the
            ##  elevator cap, we don't want to keep deleting arrivals.
            ##  We want those arrivals to wait until elevator returns.
            for i in range(min(self.elevCap, elevClass.lenArr)):
                elevClass.waitTime += now() - passClass.arrvs[0]
                del passClass.arrvs[0]

            ## yield a random exponential gen time
            yield hold, self, G.Rnd.expovariate(self.elevRetTime)

def main():
    ## init processes
    initialize()
    G.passProc = passClass()
    G.elevProc = elevClass()
    
    ## run simulation
    activate(G.passProc, G.passProc.Run())
    activate(G.elevProc, G.elevProc.Run())
    MaxSimtime = float(sys.argv[4])
    simulate(until=MaxSimtime)

    # print results
    print "mean passenger wait:", (elevClass.waitTime / passClass.numPassTot)
    print "prop. of visits that leave passengers behind:", (elevClass.overCap / elevClass.numElevTot)

if __name__ == "__main__": main()
