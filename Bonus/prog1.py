import threading
import random
import time
import math
import random

# count = 0
# x = []
# class work(threading.Thread):
# 	running = True
# 	def __init__(self, Matrix, count):
# 		threading.Thread.__init__(self)
# 		self.Matrix = Matrix
# 		self.count = count

# 	def run(self):
# 		while(self.running):
# 			sum = 0
# 			for row in range(len(self.Matrix)):
# 				for column in range(len(self.Matrix)):
# 					if self.Matrix[row][column] == 1:
# 						sum += 1
# 			count = sum
# 			#print(sum)
# 			if sum > 0:
# 			 	x.append(sum)
# 			self.running = False					

# def main():
# 	N = 3
# 	Matrix = [[0 for x in range(N)] for y in range(N)]
# 	#initialize Matrix
# 	for i in range(N):
# 		for j in range(N):
# 			Matrix[i][j] = random.randint(0,1)
# 	print(Matrix)		
# 	forks = [threading.Lock() for n in range(5)]
# 	count = 0
# 	matric= [work(Matrix, count) for i in range(N)]

# 	for p in matric: x.append(p.start())
# 	print(x)
# main()		

#Thread(target=call_script, args=(scriptA + argumentsA))

from threading import Thread
count = 0
x = []
B = []
Matrix = []

def closure(U, V):
	matrix = [[0 for x in range(len(U))] for y in range(len(U))]
	for i in range(len(U)):
		for j in range(len(U)):
			for k in range(len(U)):
				if U[i][k] == V[k][j]:
					matrix[i][j] = 1
					break
	return matrix

def work(Matrix, count, M):
    # some code
	sum = 0
	for row in range(len(Matrix)):
		for column in range(len(Matrix)):
			if Matrix[row][column] == 1:
				sum += 1			
	x.append(sum)
	power = 2
	temp = Matrix
	while(True):
		if(power >= M):
			print("Matrix: ",temp)
			break
		else:
			B = closure(temp,temp)	
			#print(B)
			temp = B
			power = power*2
	#print(B)
threadlist = []

def main():
	M = int(input("Enter the value for M (for creating MxM Matrix): "))
	N = 4
	N = int(input("Enter the value of N (number of threads): "))

	# Initialize Matrix
	for y in range(M):
		Matrix.append([0 for x in range(M)])

	for i in range(M):
		for j in range(M):
			Matrix[i][j] = random.randint(0,1)

	# Printing M Matrix
	print("Matrix M:")
	for row in Matrix:
		print(' '.join(str(x) for x in row))
	
	count = 0

	#initialize N threads
	for i in range(N):
		th = Thread(target=work, args=(Matrix, count, M))
		threadlist.append(th)

	#start each thread in threadlist
	for thread in threadlist:
		thread.start()
	# t1.start()
	# t2.start()
	# t3.start()
	for t in threadlist:
		t.join()

	print("The number of 1's in Matrix M: ", x[0])

	print("MATRIX:")
	for row in Matrix:
		print(' '.join(str(x) for x in row))

main()


