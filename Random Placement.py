import random

circles = []


def makeCircle(r):
  x = 5 #random.random()*(38.75-2*r)+r
  y = 10 #random.random()*(38.75-2*r)+r
  print("checking... \nx: ",x,"\ny: ",y,"\nr: ",r)
  for each in circles:
    #this is statement is never the case, even when inside() is hardcoded to return a False
    if not inside(each,x,y,r): #reason why there is no data to file
      return circles.append([x,y,r])
    
  return circles.append([x,y,r]) ##Bypassing the condition outputs to file fine, the problem is in the inside fucntion

     

def inside(c,x,y,r): #intersection fucntion
    intersect = False
    #Bug: The'c' inside of the circles[c][0] is a list and not an integer, which caused the error
    #Potential Solution: put circles.index(c) instead of c to use and integer value
    dist = ( (circles[circles.index(c)][0] - x)**2 + (circles[circles.index(c)][1] - y)**2 )**0.5
    if dist < circles[circles.index(c)][2] + r:        
        intersect = True
    return False #hardcoded a False return to see how makeCircle() would react
    
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
    with open("s.csv",'w') as doc: #change the filename path according to your file's folder
        doc.write(data)

def main():
    for n in range(5):
        makeCircle(1)
    circles.append([1,2,"ptasetse"]) #just to check that data was being added to the list
    print(circles)
    saveToFile()
main()
