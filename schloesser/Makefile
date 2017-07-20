
coverage: 
	make -C ex05 run
	make -C ex08 run
	make -C ex09 run
	make -C ex10 run
	gcov ex*/*.c library/*.c -o . 
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage

doc:
	doxygen dox-conf

clean:
	rm -rf *.info coverage* html* latex* *.c.gcov
