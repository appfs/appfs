JFLAGS = -g
JC = javac
JVM= java 
BUILD_DOCS=ON


.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java
CLASSES = ex2.java

MAIN = ex2

default: classes 

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN) measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml

doc: 
	doxygen Doxyfile

