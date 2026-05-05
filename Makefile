all: lab_final_app

lab_final_app: main.o Bit.o BitSequence.o
	g++ main.o Bit.o BitSequence.o -o lab_final_app

main.o: main.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Bit.hpp BitSequence.hpp
	g++ -c main.cpp

tests: tests.o SequenceTests.o BitTests.o Bit.o BitSequence.o
	g++ tests.o SequenceTests.o BitTests.o Bit.o BitSequence.o -o tests

tests.o: tests.cpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp Bit.hpp BitSequence.hpp SequenceTests.hpp BitTests.hpp
	g++ -c tests.cpp

SequenceTests.o: SequenceTests.cpp SequenceTests.hpp Exceptions.h DynamicArray.hpp LinkedList.hpp Sequence.hpp SequenceAlgorithms.hpp ArraySequence.hpp MutableArraySequence.hpp ImmutableArraySequence.hpp ListSequence.hpp
	g++ -c SequenceTests.cpp

BitTests.o: BitTests.cpp BitTests.hpp Exceptions.h DynamicArray.hpp Sequence.hpp SequenceAlgorithms.hpp Bit.hpp BitSequence.hpp
	g++ -c BitTests.cpp

Bit.o: Bit.cpp Bit.hpp
	g++ -c Bit.cpp

BitSequence.o: BitSequence.cpp BitSequence.hpp Bit.hpp Exceptions.h DynamicArray.hpp Sequence.hpp
	g++ -c BitSequence.cpp

run: lab_final_app
	./lab_final_app

run_tests: tests
	./tests

clean:
	del /Q *.o 2>NUL
	del /Q lab_final_app.exe 2>NUL
	del /Q tests.exe 2>NUL
