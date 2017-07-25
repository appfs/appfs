JFLAGS = -g -d .
JC = javac
SOURCES=$(shell find . -maxdepth 2 -type f -name "*.java")
CLASSES=$(SOURCES:.java=.class)
DOCDIR=doc
FILE=$(HOME)/SP/world666.gph
TEST1=$(HOME)/SP/b15.gph
TEST1=$(HOME)/SP/i640-345.gph
TEST2=$(HOME)/SP/es10kaz.gph
TEST3=$(HOME)/SP/world666.gph
TEST4=$(HOME)/SP/big8.gph
TERMINALS=100
THREADS=4
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