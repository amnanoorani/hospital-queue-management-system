# Hospital Queue Management System

A priority queue based patient scheduler in C++. Each patient gets a token and is called according to urgency instead of first come first served. Free doctors are assigned automatically to keep the waiting time low.

## Classes
- Patient: token, name, age, urgency and arrival time
- PriorityQueue: waiting list kept sorted by urgency (enqueue, dequeue)
- Doctor: free or busy status and number of patients seen
- Hospital: adds patients, assigns doctors and runs the time simulation

## Features
- 3 urgency levels: Critical, Serious, Normal
- Same urgency patients are served in arrival order
- Next patient goes to the free doctor who has seen the fewest patients
- Average and longest waiting time statistics

## Run
```
make run
```
or
```
g++ -std=c++17 hospital_queue.cpp -o hospital_queue
./hospital_queue
```
