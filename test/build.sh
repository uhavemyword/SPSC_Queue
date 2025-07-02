mkdir -p bin
g++ -std=c++11 -O3 -o bin/shmq_send shmq_send.cc -lrt -pthread
g++ -std=c++11 -O3 -o bin/shmq_recv shmq_recv.cc -lrt -pthread

g++ -std=c++11 -O3 -o bin/multhread_q multhread_q.cc -lrt -pthread

g++ -std=c++11 -O3 -o bin/multhread_varq multhread_varq.cc -lrt -pthread

