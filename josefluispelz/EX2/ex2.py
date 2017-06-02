import sys

try:
    import cElementTree as ET
except ImportError:
    try:    #Python 2.5 need to import a different module
        import xml.etree.cElementTree as ET
    except ImportError:
        sys.stderr = "Failed to import cElementTree from any known place\n)"
        print(sys.stderr)

filename = sys.argv[-1]

try:
    dom = ET.parse(open(filename, "r"))
    root = dom.getroot()
except:
    sys.stderr = "Unable to open and parse input definition file: {}".format(filename)
    print(sys.stderr)

def find_in_tree(tree, node):
    found = tree.find(node)
    if found == None:
        print("No {} in file".format(node))
        found = []
    return found

csv = ""

for gasDay in root:
    if str(gasDay.tag).split("}")[-1] == "gasDay":

        date = gasDay.get("date")
        starthour = int(gasDay.get("gasDayStartHourInUTC"))

        for boundaryNode in gasDay:
            if str(boundaryNode.tag).split("}")[-1] == "boundaryNode":

                for time in boundaryNode:
                    if str(time.tag).split("}")[-1] == "time":

                        HH = int(time.get("hour"))
                        value = int(time.find("{http://gaslab.zib.de/kwpt/measured}amountOfPower").get("value"))

                        csv += "{}; {}; {}\n".format(date,HH+starthour,value)

print(csv[:-1])