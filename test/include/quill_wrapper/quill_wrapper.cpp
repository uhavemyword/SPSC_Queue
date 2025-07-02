#include <iostream>

#include "quill_wrapper.h"

#include "quill/Backend.h"
#include "quill/BackendTscClock.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/ConsoleSink.h"
#include "quill/sinks/RotatingFileSink.h"

int QuillWrapper::backendCpuId;

quill::BackendOptions BuildBackendOptions()
{
  quill::BackendOptions backendOptions;
  backendOptions.check_printable_char = {};
  if (QuillWrapper::backendCpuId >= 0)
  {
    backendOptions.backend_cpu_affinity = QuillWrapper::backendCpuId;
    std::cout << "QuillWrapper - set log backend CPU affinity: " << QuillWrapper::backendCpuId << std::endl;
  }
  return backendOptions;
}

void QuillWrapper::SetupConsoleLogger(const char* logPattern)
{
  // Start the backend thread
  quill::BackendOptions backendOptions = BuildBackendOptions();
  quill::Backend::start(backendOptions);

  // Frontend
  auto consoleSink = quill::Frontend::create_or_get_sink<quill::ConsoleSink>("console_sink");

  quill::Frontend::create_or_get_logger("console_logger", std::move(consoleSink), logPattern,
                                        "%H:%M:%S.%Qns", quill::Timezone::LocalTime);
}

void QuillWrapper::CreateFileLogger(const char* loggerName, const char* logFile, int fileLogMaxMB, int fileLogMaxCount)
{
  // Start the backend thread
  quill::BackendOptions backendOptions = BuildBackendOptions();
  quill::Backend::start(backendOptions);

  // Frontend
  auto rotatingFileSink = quill::Frontend::create_or_get_sink<quill::RotatingFileSink>(
    logFile,
    [fileLogMaxMB, fileLogMaxCount]()
    {
      // See RotatingFileSinkConfig for more options
      quill::RotatingFileSinkConfig cfg;
      cfg.set_open_mode('a');
      cfg.set_filename_append_option(quill::FilenameAppendOption::StartDate);
      cfg.set_rotation_time_daily("00:00");
      size_t mb = 1024 * 1024;
      cfg.set_rotation_max_file_size(fileLogMaxMB * mb);
      cfg.set_max_backup_files(fileLogMaxCount);
      return cfg;
    }());

  quill::Frontend::create_or_get_logger(loggerName, std::move(rotatingFileSink),
                                        "%(time),%(message)", "%H:%M:%S.%Qns", quill::Timezone::LocalTime);
}

uint64_t QuillWrapper::GetEpochTime()
{
  return quill::BackendTscClock::now().time_since_epoch().count();
  // return quill::detail::BackendManager::instance().convert_rdtsc_to_epoch_time(quill::detail::rdtsc());
}