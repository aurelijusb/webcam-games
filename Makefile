.PHONY: build clean

all: clean build

clean:
	rm -f CvGames

build:
	g++ -std=c++11 src/main.cpp src/Helpers/* src/Objects3D/* src/Output/* src/Tracker/* `pkg-config --cflags --libs opencv gl` -lglut -o CvGames
