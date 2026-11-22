NAME:=inkling

SRC_DIR:=src
INCLUDE_DIR:=include
LIB_DIR:=lib
BUILD_DIR:=bin

SRC_FILES:=$(wildcard ${SRC_DIR}/*.cpp) 

BINARY:=${BUILD_DIR}/${NAME}
RELEASE_BIN:=${BUILD_DIR}/release/${NAME}

DEBUG_FLAGS:=-Wall -Wextra -g -fsanitize=address
LIB_FLAGS:= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

build ${BINARY}: ${SRC_FILES}
	mkdir -p ${BUILD_DIR}
	g++ ${SRC_FILES} ${DEBUG_FLAGS} -I${INCLUDE_DIR} -L${LIB_DIR} ${LIB_FLAGS} -o ${BINARY}

run: ${BINARY}
	${BINARY}


