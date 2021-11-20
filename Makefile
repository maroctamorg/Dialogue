CC = g++
CFLAGS = -std=c++17
INCLUDE = 
LINKER_FLAGS = 
SOURCES = pseudocode.cpp game/game.cpp
SOURCE_OBJECT_FILES = $(shell basename -a ${SOURCES:.cpp=.o})
TARGET = dialogue

all: build run

build: compile link

compile:
	@echo "\nCompiling App..."
	${CC} -c $(patsubst %, src/%, $(SOURCES)) ${INCLUDE} ${CFLAGS}
	mv ${SOURCE_OBJECT_FILES} build/

link:
	@echo "\nLinking App..."
	${CC} -o ${TARGET} $(patsubst %, build/%, ${SOURCE_OBJECT_FILES}) ${LINKER_FLAGS}
	mv ${TARGET} build/

run:
	@echo "\nRunning App..."
	build/${TARGET}

cleanup:
	@echo "\nCleaning up App..."
	rm $(patsubst %, build/%, ${SOURCE_OBJECT_FILES})
