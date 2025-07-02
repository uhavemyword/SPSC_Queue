#include "logger.h"

#include <ctime>
#include <chrono>
#include <string>
#include <thread>
#include <sstream>
#include <iomanip>

#include "quill/Frontend.h"
#include "quill_wrapper/quill_wrapper.h"
#include "quill/core/FrontendOptions.h"
#include "quill/core/LogLevel.h"

static quill::Logger *CreateConsoleLogger() {
    std::string loggerName = "console_logger";
    QuillWrapper::SetupConsoleLogger("%(time) %(log_level:<5) [%(thread_id)] %(short_source_location:<20) %(message)");

    auto *logger = quill::Frontend::get_logger(loggerName);

    // set the log level of the logger to debug (default is info)
    logger->set_log_level(quill::LogLevel::Info);

    // 需要先打日志让rdtsc生效
    QUILL_LOG_INFO(logger, "Quill ConsoleLogger created!");

    // The function `quill::detail::BackendManager::instance().convert_rdtsc_to_epoch_time(quill::detail::rdtsc())`
    // will return a valid timestamp only after the backend worker has started and processed
    // at least one log with `ClockSourceType::Tsc`.
    // This is because the Rdtsc clock is lazily initialized by the backend worker on the first log message.
    // To ensure at least one log message is processed, we call flush_log here.
    logger->flush_log();

    auto now = QuillWrapper::GetEpochTime();
    while (now == 0) {
        QUILL_LOG_INFO(logger, "Wait for rdtsc clock take effect...");
        logger->flush_log();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        now = QuillWrapper::GetEpochTime();
    }

    // 确保时间OK
    auto clock = std::chrono::system_clock::now();
    auto clockNs = std::chrono::duration_cast<std::chrono::nanoseconds>(clock.time_since_epoch()).count();
    // 有些系统精度不足，比较1毫秒即可，也方便debug版本用
    if (abs((int64_t)now - clockNs) > 1000'000) {
        QUILL_LOG_ERROR(logger, "rdtsc clock is different with system clock, rdtsc: {}, system clock: {}", now, clockNs);
    }

    return logger;
}

quill::Logger *MyLogger::GetConsoleLogger() {
    static quill::Logger *s_consoleLogger = CreateConsoleLogger();
    return s_consoleLogger;
}
