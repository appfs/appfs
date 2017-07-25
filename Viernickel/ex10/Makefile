JFLAGS = -g -d .
JC = javac
SOURCES=$(shell find . -maxdepth 2 -type f -name "*.java")
CLASSES=$(SOURCES:.java=.class)
DOCDIR=doc
FILE=./check/i640-345.gph
TEST1=./check/i640-345.gph
TEST2=./check/es10kaz.gph
TEST3=./check/world666.gph
TEST4=./check/big8.gph
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
	java -Xmx$(JMEM) ex10 $(TEST1) $(TERMINALS) $(THREADS)
	java -Xmx$(JMEM) ex10 $(TEST2) $(TERMINALS) $(THREADS)
	java -Xmx$(JMEM) ex10 $(TEST3) $(TERMINALS) $(THREADS)
	java -Xmx$(JMEM) ex10 $(TEST4) $(TERMINALS) $(THREADS)
	@echo "--------------------------"
	@echo "Success, all tests passed."
	@echo "--------------------------"