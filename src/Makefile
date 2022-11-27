CXX = g++
# CXXFLAGS = -std=c++11 -O3 -march=native
# CXXFLAGS = -std=c++11 -pg -g  -Wall -Wextra -pedantic -DDEBUG
CXXFLAGS = -std=c++11 -g  -Wall -Wextra -pedantic -DDEBUG

LIBS = loc.o \
		cell.o \
		parse.o \
		net.o \
		SA.o \
		output.o \
		perturb.o


RES = main.cpp
EXE = ../bin/hw3
TESTCASE_DIR = ../testcases
OUTPUT_DIR = ../output
VERIFY_DIR = ../verifier

.PHONY: test clean

CASE = 1
# dead space ratio
DSR = 0.15

all: $(LIBS)
	$(CXX) $(CXXFLAGS) $(RES) $(LIBS) -o $(EXE)
	-rm $(LIBS)

test:
	-rm $(OUTPUT_DIR)/*.floorplan
	time ./$(EXE) $(TESTCASE_DIR)/n$(CASE).hardblocks $(TESTCASE_DIR)/n$(CASE).nets $(TESTCASE_DIR)/n$(CASE).pl $(OUTPUT_DIR)/n$(CASE).floorplan $(DSR)
	./$(VERIFY_DIR)/verifier $(TESTCASE_DIR)/n$(CASE).hardblocks $(TESTCASE_DIR)/n$(CASE).nets $(TESTCASE_DIR)/n$(CASE).pl $(OUTPUT_DIR)/n$(CASE).floorplan $(DSR)

testall: clean all
	-rm $(OUTPUT_DIR)/*.floorplan
	time ./$(EXE) $(TESTCASE_DIR)/n100.hardblocks $(TESTCASE_DIR)/n100.nets $(TESTCASE_DIR)/n100.pl $(OUTPUT_DIR)/n100.floorplan $(DSR)
	time ./$(EXE) $(TESTCASE_DIR)/n200.hardblocks $(TESTCASE_DIR)/n200.nets $(TESTCASE_DIR)/n200.pl $(OUTPUT_DIR)/n200.floorplan $(DSR)
	time ./$(EXE) $(TESTCASE_DIR)/n300.hardblocks $(TESTCASE_DIR)/n300.nets $(TESTCASE_DIR)/n300.pl $(OUTPUT_DIR)/n300.floorplan $(DSR)


clean:
	-rm $(EXE)
	-rm $(LIBS)

benchmark: packing_gz
	-rm -rf ../HW3_grading
	tar -zxvf ../HW3_grading.tar.gz -C ../
	mkdir -p ../HW3_grading/student/111062506
	cp CS6135_HW3_111062506.tar.gz ../HW3_grading/student/111062506/
	cd ../HW3_grading && bash HW3_grading.sh

upload: packing_gz
	ssh g111062506@ic 'rm -rf ~/hw3/HW3_grading'
	scp ../HW3_grading.tar.gz g111062506@ic:~/hw3/
	ssh g111062506@ic 'cd hw3/ && tar -zxvf HW3_grading.tar.gz && mkdir HW3_grading/student/111062506'
	scp CS6135_HW3_111062506.tar.gz g111062506@ic:~/hw3/HW3_grading/student/111062506/

submit: testall packing_gz benchmark
	echo "finish"

packing_gz: testall
	-rm -rf HW3 CS6135_HW3_111062506.tar.gz
	mkdir -p HW3/src
	cp *.cpp HW3/src/
	cp *.h HW3/src/
	cp Makefile HW3/src/
	cp README HW3/src/
	cp -r ../output/ HW3/
	mkdir -p HW3/bin
	cp ../bin/hw3 HW3/bin/hw3
	# cp ../vlsi-hw3.pdf HW3/CS6135_HW3_111062506_report.pdf
	echo "below is unneccesary"
	cp -r ../testcases HW3/
	cp -r ../verifier HW3/
	tar -zcvf CS6135_HW3_111062506.tar.gz HW3/
	cp CS6135_HW3_111062506.tar.gz ../
	-rm -rf HW3

# send_cases:
# 	scp -r testcases g111062506@ic:~/hw2

# send:
# 	scp -r src u107062115@ic:~/hw2

# packing:
# 	-rm -rf CS3130_PA2 PA2_107062115.tar.gz
# 	mkdir CS3130_PA2
# 	cp -r src/ CS3130_PA2/
# 	cp Makefile CS3130_PA2
# 	tar -zcvf PA2_107062115.tar.gz CS3130_PA2/
# 	-rm -rf CS3130_PA2
