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

    if (!cpupin(2))
    {
        exit(1);
    }

    MsgQueue *q = getMsgQueue();

    Msg *msg = nullptr;
    while (1)
    {
        while ((msg = q->alloc()) == nullptr)
            ;
        std::cout << "input: " << std::flush;
        if (!(std::cin >> msg->buf))
            break;
        msg->ts = tscns.rdtsc();
        msg->ns = tscns.tsc2ns(msg->ts);
        q->push();
    }

    return 0;
}
