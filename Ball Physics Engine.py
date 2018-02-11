# IMPORT MATH FOR SQRT
import math


# SCALED VALUES FOR FORCES
class scaledVals(object):
    def __init__(self,u,g,h,d):
        self.up = u
        self.grav = g
        self.horiz = h
        self.down = d


# CREATE PALATTE OBJECT
class createPalatte(object):
    def __init__(self,h,w):
        self.height = h
        self.width = w
        self.area = h*w


# DECLARE GLOBAL VARS
circles = []
scale = scaledVals(0.1, 0.5, 0.03, 0.5)  #up,grav,horiz,down #Ivan: Changed some of these variable around
palatte = createPalatte(38.75,38.75)


# DETERMINE IF CIRCLES INTERSECT
def inside(c1,c2):
    distX = circles[c1][0] - circles[c2][0]
    distY = circles[c1][1] - circles[c2][1]
    rsum = circles[c1][2] + circles[c2][2]
    dist = math.sqrt( distX**2 + distY**2 )
    return dist < rsum


# CREATE DISK
def makeCircle():
    r = 1                   #temporarily only radius 1
    x = palatte.width/2     #centers new circle
    y = r + 11              #slightly offsets new circle from top of palatte
    dx = dy = 0             #set dx and dy starting at zero
    circles.append([ x,y,r,dx,dy ])
    return None


# UPDATE CIRCLES
def update():
    for i in range(len(circles)):  # x, y, r, dx, dy

        #Gravity
        circles[i][4] += scale.grav
        circles[i][1] += circles[i][4]

        #Floor Boundary
        if circles[i][1] > palatte.height - circles[i][2]:
            circles[i][1] = palatte.height - circles[i][2]

        #Ceiling Boundary
        if circles[i][1] < circles[i][2]:
            circles[i][1] += scale.down
            

        #Sides Boundary
        if circles[i][0] > palatte.width - circles[i][2]:
            circles[i][0] = palatte.width - circles[i][2]
        if circles[i][0] < circles[i][2]:
            circles[i][0] = circles[i][2]

        #Intersecting
        for j in range(len(circles)):
            if j==i:
                continue

            if inside(i,j):
                if circles[i][0] < circles[j][0]:
                    circles[i][0] += -scale.horiz
                    circles[j][0] += scale.horiz
                    
                      
                    
                else:
                    circles[i][0] += scale.horiz
                    circles[j][0] += -scale.horiz
                    
                
                circles[i][4] = 0

                while inside(i,j):
                    if circles[i][1] < circles[j][1]:
                        circles[i][1] += -scale.up
                        
                        
                    else:
                        circles[j][1] += -scale.up


# SAVE TO .CSV FILE
def saveToFile():
    data = ""
    for disk in range(len(circles)):
        for entry in range(5):
            data += str(circles[disk][entry])
            if entry < 4:
                data += ","
            elif disk < len(circles)-1:
                data += "\n"
    with open("se.txt",'w') as doc:
        doc.write(data)


# RUN PROGRAM
def main():

    circleTime = 50
    settleTime = 1000 #changed settle time
    numCircles = 100 #changed circles: algorithm is exponential/polynomial time, after 200 or so circles, it takes a long time to calculate

    for circleCount in range(numCircles):
        makeCircle()
        for timeCount in range(circleTime):
            update()
    for timeCount in range(settleTime):
        update()

    saveToFile()

main()
