mkdir -p bin
g++ -std=c++11 -O3 -o bin/shmq_send shmq_send.cc -lrt -pthread
g++ -std=c++11 -O3 -o bin/shmq_recv shmq_recv.cc -lrt -pthread

g++ -std=c++11 -O3 -o bin/multhread_q multhread_q.cc -lrt -pthread

g++ -std=c++11 -O3 -o bin/multhread_varq multhread_varq.cc -lrt -pthread


g++ -std=c++17 -O3 -Iinclude -Llib -o bin/shmq_recv2 shmq_recv2.cc logger.cpp -lquill_wrapper -lrt -pthread -lstdc++fs
g++ -std=c++17 -O3 -Iinclude -Llib -o bin/shmq_send2 shmq_send2.cc logger.cpp -lquill_wrapper -lrt -pthread -lstdc++fs

