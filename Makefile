# COMMENTS BEGIN WITH A HASH

# THE NAME OF YOUR PROJECT
PROJECT = NBodySimulator
# ALL CPP COMPILABLE IMPLEMENTATION FILES THAT MAKE UP THE PROJECT
SRC_FILES = src/main.cpp src/simulation_config.cpp src/nbody_system2d.cpp src/body_io.cpp src/run_logger.cpp
# ALL HEADER FILES THAT ARE PART OF THE PROJECT
H_FILES = include/body_io.h include/body2d.hpp include/nbody_system2d.h include/real_type.hpp include/run_logger.h include/simulation_config.h include/vec2.hpp
# ANY OTHER RESOURCES FILES THAT ARE PART OF THE PROJECT
REZ_FILES = bodies.csv trajectories.csv config.txt final.txt
# YOUR USERNAME
USERNAME = zachary_meyer

# NO EDITS BELOW THIS LINE
CXX = g++
CXXFLAGS = -Iinclude
CXXFLAGS_DEBUG = -g
CXXFLAGS_WARN = -Wall -Wextra -Wconversion -Wdouble-promotion -Wunreachable-code -Wshadow -Wpedantic
CPPVERSION = -std=c++17

OBJECTS = $(SRC_FILES:.cpp=.o)

ARCHIVE_EXTENSION = zip

ifeq ($(shell echo "Windows"), "Windows")
	TARGET = $(PROJECT).exe
	DEL = del
	ZIPPER = tar -a -c -f
	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
	Q =
	INC_PATH = C:/mingw64/include/
	LIB_PATH = C:/mingw64/lib/
	RPATH =
else
	TARGET = $(PROJECT)
	DEL = rm -f
	ZIPPER = tar -acf
	Q= "
	INC_PATH = /usr/local/include/
	LIB_PATH = /usr/local/lib/

	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		CXXFLAGS += -D LINUX
		RPATH =
	endif
	ifeq ($(UNAME_S),Darwin)
		CXXFLAGS += -D OSX
		RPATH = -Wl,-rpath,/Library/Frameworks -Wl,-rpath,$(LIB_PATH)
	endif

	ifeq ($(shell tar --version | grep -o "GNU tar"), GNU tar)
		ARCHIVE_EXTENSION = tar.gz
	endif

	ZIP_NAME = $(PROJECT)_$(USERNAME).$(ARCHIVE_EXTENSION)
endif

LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(RPATH) -L$(LIB_PATH) $(LIBS)

.cpp.o:
	$(CXX) $(CPPVERSION) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(CXXFLAGS_WARN) -o $@ -c $< -I$(INC_PATH)

clean:
	del /F /Q $(TARGET)
	del /F /Q src\*.o

depend:
	@sed -i.bak '/^# DEPENDENCIES/,$$d' Makefile
	@$(DEL) sed*
	@echo $(Q)# DEPENDENCIES$(Q) >> Makefile
	@$(CXX) -MM $(SRC_FILES) >> Makefile

submission:
	@echo "Creating submission file $(ZIP_NAME) ..."
	@echo "...Zipping source files:   $(SRC_FILES) ..."
	@echo "...Zipping header files:   $(H_FILES) ..."
	@echo "...Zipping resource files: $(REZ_FILES)..."
	@echo "...Zipping Makefile..."
	$(ZIPPER) $(ZIP_NAME) $(SRC_FILES) $(H_FILES) $(REZ_FILES) Makefile
	@echo "...$(ZIP_NAME) done!"

.PHONY: all clean depend submission

# DEPENDENCIES
main.o: main.cpp
