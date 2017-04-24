#ex1
from math import sqrt
import decimal
Dec = decimal.Decimal

file = open("ex2.dat","r") 
lines = file.readlines() 
decimal.getcontext().prec = 10

def GM(Data):
    n = Dec(len(Data))
    gm = 1
    for d in Data:
        gm*=d**(Dec(1)/n)
    return(gm)

lenloc1,lenloc2 = 0,0
GM1, GM2 = Dec(1), Dec(1)



Location1 = [Dec(line[line.index(";")+5:]) for line in lines[3:-1] if int(line[line.index(";")+2]) == 1]
Location2 = [Dec(line[line.index(";")+5:]) for line in lines[3:-1] if int(line[line.index(";")+2]) == 2]

GM1 = GM(Location1)
GM2 = GM(Location2)

print(GM1, GM2)
file.close()
