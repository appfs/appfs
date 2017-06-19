JFLAGS = -g
JC = javac
JVM= java 
BUILD_DOCS=ON


.SUFFIXES: .java .class

.java.class:
	$(JC) $(JFLAGS) $*.java
CLASSES = ex2.java

MAIN = ex2

default: classes run

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN) measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml

doc: 
	doxygen Doxyfile
check:
	java -jar /home/mona/Downloads/checkstyle-7.8-all.jar -c /home/mona/Downloads/checkstyle-7.8-all/sun_checks.xml  ex2.java

coverage:
	java -jar /home/mona/Downloads/JCOV_BUILD/jcov_2.0/jcov.jar Instr ex2.class
	java -classpath .:/Downloads/JCOV_BUILD/jcov_2.0/jcov_file_saver.jar ex2 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml
	
