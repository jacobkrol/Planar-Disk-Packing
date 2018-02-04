import csv
from random import randint
import math

current = []

xbound = int(input("upper x bound of frame: "))
ybound = int(input("upper y bound of frame: "))
radius_pri = int(input("radius of primary disks: "))
radius_sec  = int(input("radius of seconday disks: "))
iterate = int(input("number of iterations: "))

def create_disk(x,y,r,r2):
#basically the same as the one radius program except create_disk takes two radius arguments
#if the 50/50 chance of randint(0,1) being 0 is true then r becomes the seconday radius but if not, then the program continues as it would've before and not even call on the secondary radius
        if randint(0,1) == 0:
                r = r2
                
        temp = (randint(-x,x),randint(-y,y),r)
        for i in current:
                if math.sqrt(((temp[0]-i[0])**2)+((temp[1]-i[1])**2)) < r+i[2]:
                        create_disk(x,y,r)
        return temp

def create_list(x,y,r,r2,it):
        for i in range(it):
                current.append(create_disk(x,y,r,r2))
        return(current)

def csv_write(array_list):
        with open("arrrays.csv","w",newline="") as f:
                write = csv.writer(f)
                for i in array_list:
                        write.writerow(i)

csv_write(create_list(xbound,ybound,radius_pri,radius_sec,iterate))
