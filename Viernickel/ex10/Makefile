JFLAGS = -g -d .
JC = javac
SOURCES=$(shell find . -maxdepth 2 -type f -name "*.java")
CLASSES=$(SOURCES:.java=.class)
DOCDIR=doc
FILE=/home/merlin/APPFS/files/ex10/i640-345.gph
TERMINALS=100
THREADS=1
JMEM=3g

%.class: %.java
	$(JC) $(JFLAGS) $<

default: sources

.PHONY: sources
sources: $(CLASSES)
 
.PHONY: clean
clean:
	$(RM) $(CLASSES)
 
.PHONY: doc
doc:
	mkdir -p $(DOCDIR)
	doxygen Doxyfile
	

.PHONY: test
test :
	java -Xmx$(JMEM) ex10 $(FILE) $(TERMINALS) $(THREADS)