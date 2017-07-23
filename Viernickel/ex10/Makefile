JFLAGS = -g -d .
JC = javac
SOURCES=$(shell find . -maxdepth 2 -type f -name "*.java")
DOCDIR=doc
FILE=/home/merlin/APPFS/files/ex10/i640-345.gph
TERMINALS=100
THREADS=1
JMEM=2g

%.class: %.java
	$(JC) $(JFLAGS) $<

default: classes

classes: $(CLASSES:.java=.class)
 
.PHONY: clean
clean:
	$(RM) *.class
 
.PHONY: doc
doc:
	mkdir -p $(DOCDIR)
	doxygen Doxyfile
	

.PHONY: test
test :
	java -Xmx$(JMEM) ex10 $(FILE) $(TERMINALS) $(THREADS)