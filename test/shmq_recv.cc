#include <bits/stdc++.h>
#include <thread>
#include "tscns.h"
#include "cpupin.h"
#include "shmq_msg.h"

TSCNS tscns;

int main()
{
    tscns.init();

    std::thread t([&]
                  {
        while(true) {
        tscns.calibrate();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        } });

    if (!cpupin(5))
    {
        exit(1);
    }

    MsgQueue *q = getMsgQueue();

    Msg *msg = nullptr;
    while (1)
    {
        while ((msg = q->front()) == nullptr)
            ;
        long latency = tscns.rdtsc();
        long latency_ns = tscns.tsc2ns(latency);
        std::cout << "recv: " << msg->buf << " ,latency: " << latency - msg->ts << " ,ns: " << latency_ns - msg->ns << std::endl;
        q->pop();
    }

    return 0;
}
