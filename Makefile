run: hospital_queue
	./hospital_queue

hospital_queue: hospital_queue.cpp
	g++ -std=c++17 -Wall hospital_queue.cpp -o hospital_queue
