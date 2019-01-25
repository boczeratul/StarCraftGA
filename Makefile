C = gcc
CXX = g++
#CXXFLAGS = -g -Wall
CXXFLAGS = -O2 -Wall
INCLUDE = 
TLIB = 

#-----Suffix Rules---------------------------
# set up C++ suffixes and relationship between .cc and .o files

.SUFFIXES: .cpp .c
	
.c.o:
	$(C) $(CXXFLAGS) $(INCLUDE) -c $<

.c :
	$(C) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(TLIB) 

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

.cpp :
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ -lm $(TLIB) -lg++

#-----File Dependencies----------------------

GOAL = SC2Overmind

SRC = ga.cpp myrand.cpp main.cpp mt19937ar.cpp SCTools.cpp

OBJ = $(addsuffix .o, $(basename $(SRC)))

$(GOAL): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(TLIB) -o $@ $(OBJ)

#-----Other stuff----------------------------
depend:
	makedepend $(SRC)

clean:
	rm -f $(OBJ) $(GOAL)

# DO NOT DELETE

