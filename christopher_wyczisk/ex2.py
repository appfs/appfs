import sys, csv, datetime
from lxml import etree, objectify
from lxml.etree import XMLSchemaParseError, XMLSyntaxError

# Anmerkung:
# Um dieses Programm z.B. auf Ubuntu und Windows auszuführen, muss mittels PIP oder easy_install (etc.)
# lxml installiert werden. Dies Funktioniert z.B. per pip install lxml
# Sollte diese Anweisung zu folgendem Fehler führen:
#       error: command 'x86_64-linux-gnu-gcc' failed with exit status 1
# so haben Sie Python-Dev auf Ihrer Maschine nicht vollständig installiert, das Loest man auf Ubuntu 16.x z.B. per:
#       sudo apt-get update
#       apt-get install python2.7-dev
# Im Anschluss bitte pip install lxml, sollte das trotzdem nicht klappen: E-Mail an mich.
# Verwenden Sie Python3 nehmen sie python3-dev.
# Start des programms geht z.B. per:
#       python ex2.py measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml measured-1-1-0.xsd result.csv

class MeasuredDto(object):
    
    def __init__(self, datumAlsString, hourAlsString, wertAlsString):
        self.hour = hourAlsString
        self.datum = self.__getDatum(datumAlsString, hourAlsString)
        self.wert = wertAlsString
    
    def __getDatum(self, datum, hour):
        datumObj = datetime.datetime.strptime(datum, "%Y-%m-%d")
        if hour > 24:
            datumObj = datumObj + datetime.timedelta(days = 1)
            self.hour = hour - 24
        return datumObj.strftime("%Y-%m-%d")
    
    def getAsSemikolonString(self):
        string = self.datum + ";" + self.hour + ";" + self.wert
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
            xml = self.__readXml()
            self.__readMessureds(xml)
        
    def writeCsv(self):
        fobj = open(self.__csvOutputFilename, "w")
        csvWriter = csv.writer(fobj, delimiter=';')
        for measured in self.__messureds:
            csvWriter.writerow([measured.datum, measured.hour, measured.wert])
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
                wert = tag.find("{" + nsmap[None] + "}amountOfPower").get('value')
                self.__messureds.append(MeasuredDto(datum, int(hour) + int(tag.get('hour')), wert))
        
        
        
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

