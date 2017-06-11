JFLAGS = -g
JC = javac
JVM= java 
BUILD_DOCS=ON


.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java
CLASSES = ex5.java Graph.java GraphEdge.java GraphNode.java

MAIN = ex5

default: classes run

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN) acycpos-1.gph

doc: 
	doxygen Doxyfile
check:
	java -jar /home/mona/Downloads/checkstyle-7.8-all.jar -c /home/mona/Downloads/checkstyle-7.8-all/sun_checks.xml  ex5.java

coverage:
	java -jar /home/mona/Downloads/JCOV_BUILD/jcov_2.0/jcov.jar Instr ex5.class
	java -classpath .:/Downloads/JCOV_BUILD/jcov_2.0/jcov_file_saver.jar ex5 acycpos-1.gph 
	
