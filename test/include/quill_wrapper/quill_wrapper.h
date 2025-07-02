#pragma once

#include <cstdint>

class QuillWrapper
{
public:
  static int backendCpuId;

  static void SetupConsoleLogger(const char* logPattern);

  static void CreateFileLogger(const char* loggerName, const char* logFile, int fileLogMaxMB, int fileLogMaxCount);

  static uint64_t GetEpochTime();
};