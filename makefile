CC := g++

SRC_DIR = src
BUILD_DIR = build

build_dir:
	mkdir -p ${BUILD_DIR}

compile:build_dir
	${CC} -c ${SRC_DIR}/main.cpp -o ${BUILD_DIR}/main.o -I include/
	${CC} -c ${SRC_DIR}/HelloTriangleApp.cpp -o ${BUILD_DIR}/HelloTriangleApp.o -I include/

link:compile
	${CC} ${BUILD_DIR}/*.o -o ${BUILD_DIR}/main.exe -Llib -lglfw3dll -lvulkan-1
	
run:link
	${BUILD_DIR}/main
	
clean:
	rm -f ${BUILD_DIR}/*.o 
	rm -f ${BUILD_DIR}/main.exe