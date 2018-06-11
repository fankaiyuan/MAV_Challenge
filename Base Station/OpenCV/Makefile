all: 
	g++ -w -ggdb -pthread -std=c++11 -I. main.cpp objective.cpp -lxdo  -lXtst -lX11 -o Vision `pkg-config --cflags --libs opencv`


clean:
	rm -fr Vision
