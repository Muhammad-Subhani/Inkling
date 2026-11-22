NAME:=inkling
CC:=g++

SRC_DIRS:= src
INCLUDE_DIR:=include
LIB_DIR:=lib
BUILD_DIR:=bin

SRC:= $(foreach D, $(SRC_DIRS), $(wildcard ${D}/*.cpp))
OBJ:= $(SRC:%=${BUILD_DIR}/%.o)

BINARY:=${BUILD_DIR}/${NAME}
RELEASE_BIN:=${BUILD_DIR}/release/${NAME}

CFLAGS:=-Wall -Wextra
DEBUG_FLAGS:= -g -fsanitize=address
LIB_FLAGS:= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

${BINARY}: ${OBJ}
	${CC} ${OBJ} -o $@ -L${LIB_DIR} ${LIB_FLAGS} ${DEBUG_FLAGS}


${BUILD_DIR}/%.cpp.o: %.cpp 
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -I${INCLUDE_DIR} -c $< -o $@

run: ${BINARY}
	${BINARY}

.PHONY: ${BINARY}
