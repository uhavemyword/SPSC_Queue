#pragma once

#include <cstdint>

#define QUILL_DISABLE_NON_PREFIXED_MACROS
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill_wrapper/quill_wrapper.h"

class MyLogger
{
public:
  static quill::Logger *GetConsoleLogger();
};

#define CHECK_LOG(pattern, ...) pattern, ##__VA_ARGS__

#define LOGD(pattern, ...) QUILL_LOG_DEBUG(MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGI(pattern, ...) QUILL_LOG_INFO(MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGW(pattern, ...) QUILL_LOG_WARNING(MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGE(pattern, ...) QUILL_LOG_ERROR(MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGDL(intervalNs, pattern, ...) QUILL_LOG_DEBUG_LIMIT(std::chrono::nanoseconds{intervalNs}, MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGIL(intervalNs, pattern, ...) QUILL_LOG_INFO_LIMIT(std::chrono::nanoseconds{intervalNs}, MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGWL(intervalNs, pattern, ...) QUILL_LOG_WARNING_LIMIT(std::chrono::nanoseconds{intervalNs}, MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGEL(intervalNs, pattern, ...) QUILL_LOG_ERROR_LIMIT(std::chrono::nanoseconds{intervalNs}, MyLogger::GetConsoleLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))

#define LOGTI(pattern, ...) QUILL_LOG_INFO(MyLogger::GetTickLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))
#define LOGSI(pattern, ...) QUILL_LOG_INFO(MyLogger::GetSysLogger(), CHECK_LOG(pattern, ##__VA_ARGS__))

// 借用日志里的rdtsc，需要打一次日志以后才能用
#define NOW() ((int64_t)QuillWrapper::GetEpochTime() + 28800'000'000'000)

constexpr int32_t TIMEZONE_OFFSET = 8;            // 时区偏移量（东八区）
constexpr int32_t S_PER_DAY = 86400;              // 每天多少秒
constexpr int32_t MS_PER_HOUR = 3600'000;         // 每小时多少毫秒
constexpr int32_t MS_PER_DAY = 86400'000;         // 每天多少毫秒
constexpr int64_t NS_PER_MS = 1'000'000;          // 每毫秒多少纳秒
constexpr int64_t NS_PER_SEC = 1'000'000'000;     // 每秒多少纳秒
constexpr int64_t NS_PER_MIN = 60'000'000'000;    // 每分钟多少纳秒
constexpr int64_t NS_PER_HOUR = 3600'000'000'000; // 每小时多少纳秒
constexpr int64_t NS_PER_DAY = 86400'000'000'000; // 每天多少纳秒
constexpr int64_t TIMEZONE_OFFSET_NS = 28800'000'000'000;
