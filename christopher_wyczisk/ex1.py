"""
Author:  Christopher Wyczisk
Version: 1.0

DESCRIPTION

Dieses Programm wir mit "python ex1.py filename" gestartet. Es liesst die Daten aus filename ein und berechnet
fuer dessen Werten das Geometrische Mittel, fehlerhaftewerte werden herausgefiltert.
"""
 
import math, types, sys

# Diesen Code hat Christopher Wyczisk runtergesaut.

class GeometrischesMittelDerWerteZweiterLocations(object):
    """Diese Klasse berechnet das Geometrische mittel fuer zwei Locations."""
    
    def __init__(self):
        self.__values1 = []
        self.__values2 = []
        self.countRowsOfLocation1 = 0
        self.countRowsOfLocation2 = 0
        self.countAllRows = 0
    
    def leseDaten(self, filename):
        """Liesst die Daten aus dem FIle filename."""
        fobj = open(filename, "r")
        for line in fobj:
            self.countAllRows += 1
            inhaltDerLineAlsSplit = line.rstrip().split(";")
            if line[0] != '#' and len(inhaltDerLineAlsSplit) == 3:
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
        """Berechnet das Geometrische Mittel"""
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
        
filename = sys.argv[1]
geoObj = GeometrischesMittelDerWerteZweiterLocations()
geoObj.leseDaten(filename)

print("File " + filename + " with " + str(geoObj.countAllRows) + " lines")
print("Valid values Loc1: " + str(geoObj.countRowsOfLocation1) + " with GeoMean: " + str(geoObj.getGeoMittel(1)))
print("Valid values Loc2: " + str(geoObj.countRowsOfLocation2) + " with GeoMean: " + str(geoObj.getGeoMittel(2)))
