#pragma once

#include "quill/UserClockSource.h"
#include "quill/backend/BackendUtilities.h"
#include "tscns.h"

/**
 * This class needs to be thread-safe, unless only a single thread in the application calling
 * LOG_ macros from the same logger
 */
class MyClock : public quill::UserClockSource
{
private:
  TSCNS _tscns;
  int32_t _backendCpuId;
  std::thread _workerThread;
  std::atomic<bool> _stopFlag = false;

public:
  MyClock(int32_t backendCpuId) : _backendCpuId(backendCpuId)
  {
    _tscns.init();
    _workerThread = std::thread(&MyClock::workerLoop, this);
  }

  /**
   * Required by TimestampClock
   * @return current time now, in nanoseconds since epoch
   */
  uint64_t now() const override { return _tscns.rdns(); }

  void workerLoop()
  {
    std::string name = "tscns-worker";
    quill::detail::set_thread_name(name.c_str());

    if (_backendCpuId >= 0)
    {
      quill::detail::set_cpu_affinity(_backendCpuId);
    }

    while (!_stopFlag.load())
    {
      _tscns.calibrate();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  ~MyClock()
  {
    _stopFlag = true;
    _workerThread.join();
  }

  MyClock(const MyClock&) = delete;
  MyClock& operator=(const MyClock&) = delete;
  MyClock(MyClock&&) = delete;
  MyClock& operator=(MyClock&&) = delete;
};