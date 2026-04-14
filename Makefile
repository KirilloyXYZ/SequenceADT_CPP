all: lab_final_app

lab_final_app: main.o
	g++ main.o -o lab_final_app

main.o: main.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Bit.hpp BitSequence.hpp
	g++ -c main.cpp

tests: tests.o
	g++ tests.o -o tests

tests.o: tests.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Bit.hpp BitSequence.hpp SequenceTests.hpp BitTests.hpp
	g++ -c tests.cpp

run: lab_final_app
	./lab_final_app

run_tests: tests
	./tests

clean:
	del /Q *.o 2>NUL
	del /Q lab_final_app.exe 2>NUL
	del /Q tests.exe 2>NUL