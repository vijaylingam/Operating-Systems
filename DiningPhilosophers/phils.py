# Author: Vijay
# DiningPhilosophers Problem without Deadlock
import threading
import random
import time
import math

class Philosopher(threading.Thread):
    running = True
 
    def __init__(self, name, leftFork, rightFork, forknumber):
        threading.Thread.__init__(self)
        self.name = name
        self.leftFork = leftFork
        self.rightFork = rightFork
        self.forknumber = forknumber
 
    def run(self):
        while(self.running):
            x = math.floor(random.uniform(2,7))
            print('%s thinks for %d seconds' % (self.name, x))
            time.sleep(x) #think for x seconds
            print('%s is hungry' % self.name)
            self.eat()
 
    def eat(self):
        fork1, fork2, forknumber = self.leftFork, self.rightFork, self.forknumber
 
        while self.running:
            fork1.acquire(True)
            print('%s grabs fork %d (left)' %(self.name, (forknumber-1)%5))
            locked = fork2.acquire(False)
            if locked: break
            fork1.release()
            print('%s swaps forks' % self.name)
            fork1, fork2 = fork2, fork1
        else:
            return
 
        self.dining()
        fork2.release()
        fork1.release()
 
    def dining(self):			
        print('%s starts eating '% self.name)
        time.sleep(random.uniform(1,10))
        print('%s ends eating and releases forks %d (left) and %d (right).' %(self.name, (self.forknumber-1)%5, (self.forknumber+1)%5))
 
def Main():
    forks = [threading.Lock() for n in range(5)] #create 5 forks
    participants = [] #our 5 philosophers
    for i in range(5):
        participants.append(input('Enter the name of Philosopher'+ str(i+1) + ': '))
    philosophers= [Philosopher(participants[i], forks[i%5], forks[(i+1)%5], i) for i in range(5)]
    Philosopher.running = True #loop forever
    for p in philosophers: p.start()
 
Main()