#ifndef PROC_UTILS_HPP
#define PROC_UTILS_HPP

#include "config.h"
#include <cstdint>
#include <functional>
#include <string>

FW_API
uint32_t GetProcessId ();

FW_API
uint32_t GetParentProcessId ();

class FW_API Process final
{
    struct ProcData;
    ProcData *const d;

public:
    Process ();
    explicit Process (const std::string &program, std::vector<std::string> args = {});
    ~Process ();

    bool     Start ();
    bool     Start (const std::string &program, std::vector<std::string> args = {});
    bool     Terminate ();
    bool     IsRunning ();
    uint32_t ProcessId ();

    // static functions
    static bool StartDetached (const std::string &program, std::vector<std::string> args = {});
};

#endif  // PROC_UTILS_HPP
