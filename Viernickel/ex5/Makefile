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

.PHONY: test
test :
	java ex5 test/input.txt > test/output.txt
	bash -c "diff <(head -n 2 test/correct.txt) <(head -n 2 test/output.txt)"
	@echo "--------------------------"
	@echo "Success, all tests passed."
	@echo "--------------------------"