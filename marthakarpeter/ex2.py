# ex1 APPFS Martha Karpeter 367847

import sys
import xml.etree.ElementTree as ET 

file = sys.argv[-1]

tree = ET.parse(open(file, "r"))
root = tree.getroot()


def find(tree, node):
    found = tree.find(node)
    if found == None:
        print("No {} in file".format(node))
        found = []
    return found

csv = ""


for gasDay in root:
    if str(gasDay.tag).split("}")[-1] == "gasDay":
        date = gasDay.get("date")
        startH = int(gasDay.get("gasDayStartHourInUTC"))
        for bdyNode in gasDay:
            if str(bdyNode.tag).split("}")[-1] == "boundaryNode":
                for time in bdyNode:
                    if str(time.tag).split("}")[-1] == "time":
                        HH = int(time.get("hour"))%25
                        value = int(time.find("{http://gaslab.zib.de/kwpt/measured}amountOfPower").get("value"))
                        csv += "{}; {}; {}\n".format(date,HH+startH,value)
print(csv)

# gasDayStartHourInUTC -> during winter: 5, summer 4 o'clock
# HH in {1..25}
# amountOfPower max length = 1024