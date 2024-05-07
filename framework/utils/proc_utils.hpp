#ifndef PROC_UTILS_HPP
#define PROC_UTILS_HPP

#include <cstdint>
#include <functional>
#include <string>

uint32_t GetProcessId ();


uint32_t GetParentProcessId ();

class  Process final
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
