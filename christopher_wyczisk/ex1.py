
import math, types

# Diesen Code hat Christopher Wyczisk runtergesaut.

class GeometrischesMittelDerWerteZweiterLocations(object):
    
    def __init__(self):
        self.__productFromValuesOfLocation1 = 1.0
        self.__productFromValuesOfLocation2 = 1.0
        self.__values1 = []
        self.__values2 = []
        self.countRowsOfLocation1 = 0
        self.countRowsOfLocation2 = 0
        self.countAllRows = 0
    
    def leseDaten(self):
        fobj = open("../exercise_1/ex1.dat", "r")
        for line in fobj:
            self.countAllRows += 1
            inhaltDerLineAlsSplit = line.rstrip().split(";")
            if len(inhaltDerLineAlsSplit) == 3:
                try:
                    value = float(inhaltDerLineAlsSplit[2])
                    if inhaltDerLineAlsSplit[1].strip() == '1':
                        self.__values1.append(value)
                        self.countRowsOfLocation1 += 1
                    elif inhaltDerLineAlsSplit[1].strip() == '2':
                        self.__values2.append(value)
                        self.countRowsOfLocation2 += 1
                except:
                    pass
        fobj.close()
        
    def getGeoMittel(self, location):
        prod = 1.0
        if location == 1:
            for value in self.__values1:
                if value > 0:
                    prod *= math.pow(value, float(1) / float(self.countRowsOfLocation1))
        if location == 2:
            for value in self.__values2:
                if value > 0:
                    prod *= math.pow(value, float(1) / float(self.countRowsOfLocation2))
        return prod
    
geoObj = GeometrischesMittelDerWerteZweiterLocations()
geoObj.leseDaten()

print("File ex1.dat with " + str(geoObj.countAllRows) + " lines")
print("Valid values Loc1: " + str(geoObj.countRowsOfLocation1) + " with GeoMean: " + str(geoObj.getGeoMittel(1)))
print("Valid values Loc2: " + str(geoObj.countRowsOfLocation2) + " with GeoMean: " + str(geoObj.getGeoMittel(2)))
