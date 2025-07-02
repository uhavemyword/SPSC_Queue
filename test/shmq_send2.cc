#include <bits/stdc++.h>
#include <thread>
#include "cpupin.h"
#include "shmq_msg.h"

#include "logger.h"

int64_t ConvertToHHmmssSSSSSSSSS(int64_t nanoseconds)
{
    nanoseconds = nanoseconds % NS_PER_DAY;
    int64_t hours = nanoseconds / 3600 / NS_PER_SEC;
    int64_t minutes = (nanoseconds / 60 / NS_PER_SEC) % 60;
    int64_t seconds = (nanoseconds / NS_PER_SEC) % 60;
    int64_t nanos = nanoseconds % NS_PER_SEC;

    return (hours * 10000 + minutes * 100 + seconds) * 1'000'000'000 + nanos;
}

int main()
{
    QuillWrapper::SetupConsoleLogger("ConsoleLogger");
    LOGI("Quill ConsoleLogger created!");
    auto now = NOW();
    std::cout << "now: " << now << " i.e. " << ConvertToHHmmssSSSSSSSSS(now) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    now = NOW();
    std::cout << "now: " << now << " i.e. " << ConvertToHHmmssSSSSSSSSS(now) << std::endl;

    if (!cpupin(8))
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
        msg->ns = NOW();
        q->push();
    }

    return 0;
}
