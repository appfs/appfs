# Installiere Doxygen
sudo apt-get install doxygen

# Installiere folgende gcc Version
gcc version 4.9-std=c++14

# Installiere cppcheck und lcov
sudo apt-get install cppcheck
sudo apt-get install lcov 

# Zusatzinfo
Um dieses Programm z.B. auf Ubuntu und Windows auszufuehren, muss mittels PIP oder easy_install (etc.)
lxml installiert werden. Dies Funktioniert z.B. per pip install lxml
Sollte diese Anweisung zu folgendem Fehler fuehren:
   error: command 'x86_64-linux-gnu-gcc' failed with exit status 1
so haben Sie Python-Dev auf Ihrer Maschine nicht vollstaendig installiert, das Loest man auf Ubuntu 16.x z.B. per:
   sudo apt-get update
   sudo apt-get install python2.7-dev
Im Anschluss bitte sudo pip install lxml, sollte das trotzdem nicht klappen: E-Mail an mich.
Verwenden Sie Python3 nehmen sie python3-dev.
Start des programms geht z.B. per:
   python ex2.py measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml measured-1-1-0.xsd result.csv