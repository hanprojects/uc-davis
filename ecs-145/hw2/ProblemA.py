# --------------------------- #
# ECS 145 - Homework 2        #
# Due: Wednesday, Feb 26 2020 #
# --------------------------- #

# -------------------------------------------------------------- #
# You MUST (for full credit) have two SimPy Processes,           #
# one for the elevator and one for the passenger arrivals.       #
#                                                                #
# Your code will be run as                                       #
# % python Elevator.py passenger_arrival_rate elevator_capacity  #
# mean_elevator_return_time max_simtime                          #
# -------------------------------------------------------------- #

# !/usr/bin/env python

# ----------------------------------- #
#          LIBRARY IMPORTS            #
# ----------------------------------- #
import simpy

import itertools
import random
import sys

# ----------------------------------- #
#               CLASSES               #
# ----------------------------------- #

# (DONE) CLASS for globals
class G:
    Rnd = Random(12345)
    elevProc = None  # elevator process
    passProc = None  # passenger process

# Elevator CLASS: 
class elevClass(Process): 
    elevator_capacity = int(sys.argv[2])
    mean_elevator_return_time = float(sys.argv[3])
    max_simtime = float(sys.argv[4])

    def __init__(self): 
        Process.__init__(self)
    
    def Run(self): 
        while True: 
            yield passivate, self

# Passenger CLASS
class passClass(Process):
    def __init__(self):
        Process.__init__(self)
        self.passArrvRate = float(sys.argv[1])
        self.arrvs = [0.0] # self.arrvs will be arrivals waiting for pickup
        self.nextArrv = None  # for debugging/code verifying
    
    def Run(self):
        while True:
            yield 
            
            # reactivate(G.elevProc)

# ----------------------------------- #
#              FUNCTIONS              #
# ----------------------------------- #


# ----------------------------------- #
#                 MAIN                #
# ----------------------------------- #
def main():
    pass

# if __name__
if __name__ == '__main__': main()
