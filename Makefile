.PHONY: build clean

all: clean build

clean:
	rm -f CvGames

build:
	g++ src/main.cpp src/Objects3D/* src/Output/* src/Tracker/* `pkg-config --cflags --libs opencv gl` -lglut -o CvGames
