import sys, csv
from lxml import etree, objectify
from lxml.etree import XMLSchemaParseError, XMLSyntaxError

# Auch diesen Code hat Christopher Wyczisk runtergesaut, da er sich
# aufgrund einer Razzia in seinem Buero bei Toll Collect nicht aufs Coden
# konzentrieren konnte.

class MeasuredDto(object):
    
    def __init__(self, datumAlsString, hourAlsString, wertAlsString):
         self.__datumAlsString = datumAlsString
         self.__hourAlsString = hourAlsString
         self.__wertAlsString = wertAlsString
         
    def getAsSemikolonString(self):
        string =  self.__datumAlsString + ";" + self.__hourAlsString + ";" + self.__wertAlsString
        return string
         
         
class EtlProzess(object):
    
    def __init__(self, xmlInputFilename, xslInputFilename, csvOutputFilename):
        self.__xmlInputFilename = xmlInputFilename
        self.__xslInputFilename = xslInputFilename
        self.__csvOutputFilename =csvOutputFilename
        self.__messureds = []
    
    def readXml(self):
        try:
            schema = self.__readXsd()
            xml = self.__readXml(schema)
            self.__readMessureds(xml)
        except (XMLSchemaParseError, XMLSyntaxError) as error:
            sys.stderr = open('error.log', 'w')
            sys.stderr.write(str(error))
            print("Warnung: Probleme wurden bemerkt und im error.log beschrieben.")
            xml = self.__readXml(schema)
            self.__readMessureds(xml)
        
    def writeCsv(self):
        fobj = open(self.__csvOutputFilename, "w")
        csvWriter = csv.writer(fobj, delimiter=';')
        for measured in self.__messureds:
            csvWriter.writerrow(measured.getAsSemikolonString())
        fobj.close()
        
    def __readXsd(self):
        schema = etree.parse(self.__xslInputFilename)
        return etree.XMLSchema(schema)
        
    def __readXml(self, schema=None):
        parser = etree.XMLParser(remove_comments=True, schema=schema)
        tree = objectify.parse(self.__xmlInputFilename, parser=parser)
        return tree.getroot()
        
    def __readMessureds(self, xml):
        nsmap = xml.nsmap
        knotenMenge = xml.find("{" + nsmap[None] + "}gasDay")
        datum = knotenMenge.get('date')
        hour = knotenMenge.get('gasDayStartHourInUTC')
        for knoten in knotenMenge:
            for tag in knoten:
                wert = tag.find("{" + nsmap[None] + "}amountOfPower")
                neuerIndex = len(self.__messureds)
                self.__messureds[neuerIndex] = MeasuredDto(datum, hour, wert)
        
        
        
# Einlesen der Konsoleneingabe
xmlInputFilename = sys.argv[1]
xslInputFilename = sys.argv[2]
csvOutputFilename = sys.argv[3]

# Bau des Etl Objekts und Datenuebertragung von .xml zu .csv
etl = EtlProzess(xmlInputFilename, xslInputFilename, csvOutputFilename)
print("Einlesen von " + xmlInputFilename + "...")
etl.readXml()
print(xmlInputFilename + " wurde gelesen. Jetzt wird das csv erstellt...")
etl.writeCsv()
print("Fertig! Das csv File " + csvOutputFilename + " wurde erstellt.")




