ONE = hello
CC = g++

PREF_ONE = ./differentiator/
PREF_FILE = ./work_with_files/
PREF_OBJ = ./obj/
PREF_ALL_OBJ = ./all_obj/
PREF_FILE_OBJ = ./file_obj/

ONE_CPP = $(wildcard $(PREF_ONE)*.cpp)
FILE_CPP = $(wildcard $(PREF_FILE)*.cpp)
ALL_CPP = $(wildcard *.cpp)
ONE_OBJ = $(patsubst $(PREF_ONE)%.cpp, $(PREF_OBJ)%.o, $(ONE_CPP))
FILE_OBJ = $(patsubst $(PREF_FILE)%.cpp, $(PREF_FILE_OBJ)%.o, $(FILE_CPP))
ALL_OBJ = $(patsubst %.cpp, $(PREF_ALL_OBJ)%.o, $(ALL_CPP))

$(ONE) : $(ONE_OBJ) $(ALL_OBJ) $(FILE_OBJ)
	$(CC) $(ONE_OBJ) $(ALL_OBJ) $(FILE_OBJ) -o $(ONE)

$(PREF_OBJ)%.o : $(PREF_ONE)%.cpp
	$(CC) -c $< -o $@

$(PREF_FILE_OBJ)%.o : $(PREF_FILE)%.cpp
	$(CC) -c $< -o $@

$(PREF_ALL_OBJ)%.o : %.cpp
	$(CC) -c $< -o $@

clean : 
	rm $(ONE) $(PREF_OBJ)*.o