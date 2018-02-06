import random

circles = []

def makeCircle(r,threshold):
    tries = 0
    while tries < threshold:
        safe = True
        tries += 1
        x = random.random()*(38.75-2*r)+r
        y = random.random()*(38.75-2*r)+r
        if len(circles)==0:
            circles.append([x,y,r])
            print(circles[-1])
            return True
        else:
            for each in circles:
                if inside(circles.index(each),x,y,r):
                    safe = False
        if len(circles)>0 and safe:
            circles.append([x,y,r])
            print(circles[-1])
            return True
    print(tries)
    return False

def inside(c,x,y,r):
    intersect = False
    dist = ( (circles[c][0] - x)**2 + (circles[c][1] - y)**2 )**0.5
    if dist < circles[c][2] + r:
        intersect = True
    return intersect

def saveToFile():
    data = ""
    for circle in circles:
        for entry in circle:
            data += str(entry)
            if circle[-1] == entry and circles[-1] != circle:
                data += "\n"
            elif not ( circles[-1] == circle and circle[-1] == entry ):
                data += ","
    with open("/Users/jacobkrol/Desktop/My Circle Data.csv",'w') as doc:
        doc.write(data)

def main():
    generating = True
    while generating:
        generating = makeCircle(1,100) # ( radius, threshold )
    saveToFile()
main()
