JFLAGS = -g -d .
JC = javac
CLASSES=$(shell find . -maxdepth 1 -type f -name "*.java")
DOCDIR=doc
 
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
