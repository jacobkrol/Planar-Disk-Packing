import random

circles = []

def makeCircle(r):
    x = random.random()*(38.75-2*r)+r
    y = random.random()*(38.75-2*r)+r
    print("checking... \nx: ",x,"\ny: ",y,"\nr: ",r)
    for each in circles:
        if not inside(each,x,y,r):
            circles.append([x,y,r])

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
            else:
                data += ","
    print("Data:\n"+data)
    with open("/Users/jacobkrol/Desktop/My Circle Data.csv",'w') as doc:
        doc.write(data)

def main():
    for n in range(5):
        makeCircle(1)
    print(circles)
    saveToFile()
main()
