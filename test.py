import csv
from random import randint
import math

current = []

xbound = int(input("upper x bound of frame: "))
ybound = int(input("upper y bound of frame: "))
radius = int(input("radius of disks: "))
iterate = int(input("number of iterations: "))

def create_disk(x,y,r):
#recursive function that makes non-intersecting disks by ensuring that distance between centers is longer than or equal to two times the radius
	temp = (randint(-x,x),randint(-y,y),r)
	for i in current:
		if math.sqrt(((temp[0]-i[0])**2)+((temp[1]-i[1])**2)) < 2*r:
			create_disk(x,y,r)
	return temp

def create_list(x,y,r,it):
#compiles list of non-intersecting disks
	for i in range(it):
		current.append(create_disk(x,y,r))
	return(current)

def csv_write(array_list):
#writes finished list to a csv file
	with open("arrrays.csv","w",newline="") as f:
		write = csv.writer(f)
		for i in array_list:
			write.writerow(i)

csv_write(create_list(xbound,ybound,radius,iterate))
