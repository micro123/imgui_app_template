//
// Created by tang on 2022/6/30.
//

#include "utils/proc_utils.hpp"
#include <core/assert.hpp>

#if _WIN32

#include <Windows.h>
#include <atomic>
#include "utils/win32_utility.hpp"

static PROCESS_INFORMATION start_process (const std::string &cmd, const std::vector<std::string> &args, bool detached)
{
    std::wstring wprog = fromUtf8 (cmd);
    std::wstring cmdl  = L'"' + wprog + L'"';
    for (const auto &arg: args)
    {
        cmdl.push_back (L' ');
        cmdl += fromUtf8 (arg);
    }
    cmdl.push_back (L'\0');
    STARTUPINFOW startupinfow {};
    ZeroMemory (&startupinfow, sizeof (startupinfow));
    startupinfow.cb          = sizeof (startupinfow);
    startupinfow.wShowWindow = TRUE;
    PROCESS_INFORMATION information {};
    BOOL r = CreateProcessW (wprog.c_str (), cmdl.data (), nullptr, nullptr, detached ? FALSE : TRUE, detached ? DETACHED_PROCESS : 0, nullptr,
                             nullptr, &startupinfow, &information);
    if (!r)
        fprintf (stderr, "LastError() = %lu", GetLastError ());
    return information;
}

#else

#include <sys/stat.h>
#include <unistd.h>
#include <atomic>
#include <algorithm>
#include <csignal>
#include <cstring>

static int find_command (const char *cmd_short, char *out_path, size_t path_length)
{
    char  *envPath;
    char  *part;
    size_t length;
    size_t cmd_shortLength;
    size_t needTrailingSlash;
    //    struct stat st{};

    if (access (cmd_short, X_OK) == 0)
    {
        char *ret = realpath (cmd_short, out_path);
        (void) ret;
        return 1;
    }

    if (!cmd_short || !cmd_short[0])
    {
        return 0;
    }

    envPath = getenv ("PATH");
    if (!envPath)
    {
        return 0;
    }

    cmd_shortLength = strlen (cmd_short);
    while (*envPath)
    {
        part = strchr (envPath, ':');
        if (part)
        {
            length = part - envPath;
        }
        else
        {
            length = strlen (envPath);
        }

        if (length > 0)
        {
            needTrailingSlash = (envPath[length - 1] == '/') ? 0 : 1;
            if (length + cmd_shortLength + needTrailingSlash < path_length)
            {
                strncpy (out_path, envPath, length);
                strncpy (out_path + length, "/", needTrailingSlash);
                strncpy (out_path + length + needTrailingSlash, cmd_short, cmd_shortLength);
                out_path[length + needTrailingSlash + cmd_shortLength] = '\0';
                if (access (out_path, X_OK) == 0)
                {
                    return 1;
                }
            }
        }

        envPath += length;
        if (*envPath == ':')
        {
            envPath++;
        }
    }
    return 0;
}

static pid_t create_process (const char *cmd_full, char *const *args)
{
    pid_t pid = fork ();
    if (pid == 0)
    {
        // child, execute command with args
        int r = execv (cmd_full, args);
        // mostly not execute here
        if (r)
        {
            perror (cmd_full);
        }
        return 0;
    }
    else if (pid < 0)
    {
        return 0;
    }
    else
    {
        return pid;
    }
}

#endif

uint32_t GetProcessId ()
{
#if _WIN32
    return ::GetProcessId (nullptr);
#else
    return getpid ();
#endif
}

uint32_t GetParentProcessId ()
{
#if _WIN32
    ULONG_PTR pbi[6];
    ULONG     ulSize = 0;
    LONG (WINAPI * NtQueryInformationProcess)
    (HANDLE ProcessHandle, ULONG ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
    *(FARPROC *) &NtQueryInformationProcess = GetProcAddress (LoadLibraryA ("NTDLL.DLL"), "NtQueryInformationProcess");
    if (NtQueryInformationProcess)
    {
        if (NtQueryInformationProcess (GetCurrentProcess (), 0, &pbi, sizeof (pbi), &ulSize) >= 0 && ulSize == sizeof (pbi))
            return (uint32_t) pbi[5];
    }
    return (uint32_t) -1;
#else
    return getppid ();
#endif
}

struct Process::ProcData {
    std::atomic_bool         started {false};
    std::string              program;
    std::vector<std::string> arguments;

    // platform specific data
#if _WIN32
    PROCESS_INFORMATION pi {};
#else
    pid_t child_pid {-1};  // store fork() return value
#endif

    bool                   Start ();
    bool                   Stop ();
    bool                   IsRunning ();
    [[nodiscard]] uint32_t Pid () const;
    void                   Cleanup ();
};

Process::Process () : d {new ProcData}
{
    d->started = false;
}

Process::Process (const std::string &program, std::vector<std::string> args) : Process ()
{
    d->program   = program;
    d->arguments = move (args);
}

Process::~Process ()
{
    d->Cleanup ();
    delete d;
}

bool Process::Start ()
{
    return d->Start ();
}

bool Process::Terminate ()
{
    return d->Stop ();
}

bool Process::StartDetached (const std::string &program, std::vector<std::string> args)
{
#if _WIN32
    auto pi = start_process (program, args, true);
    return pi.dwProcessId > 0;
#else
    char  full_cmd[1024] {0};
    int   found = find_command (program.c_str (), full_cmd, sizeof (full_cmd));
    if (found)
    {
        std::vector<char *> local_args (args.size () + 2);
        std::transform (begin (args), end (args), begin (local_args) + 1, [] (auto &x) { return &x[0]; });
        local_args.front () = full_cmd;
        local_args.back ()  = nullptr;
        int success         = create_process (full_cmd, local_args.data ());
        return success;
    }
    return false;
#endif
}

bool Process::Start (const std::string &program, std::vector<std::string> args)
{
    d->program   = program;
    d->arguments = move (args);
    return d->Start ();
}

bool Process::IsRunning ()
{
    return d->IsRunning ();
}

bool Process::ProcData::Start ()
{
    FW_ASSERT_MSG (started.load () == false, "Process Already Started!");
#if _WIN32
    pi             = start_process (program, arguments, false);
    return started = (pi.dwProcessId > 0);
#else
    char cmd_full[1024] {0};
    if (find_command (program.c_str (), cmd_full, sizeof (cmd_full)))
    {
        std::vector<char *> args {arguments.size () + 2};
        args.front () = cmd_full;
        args.back ()  = nullptr;
        std::transform (begin (arguments), end (arguments), begin (args) + 1, [] (auto &x) { return &x[0]; });
        child_pid = create_process (cmd_full, args.data ());
    }
    return started = (child_pid != 0);
#endif
}

bool Process::ProcData::Stop ()
{
    FW_ASSERT_MSG (started.load () == true, "Process Not Started!");
#if _WIN32
    BOOL r = TerminateProcess (pi.hProcess, 0);
    if (!r)
    {
        fprintf (stderr, "Stop Error: GetLastError() = %lu\n", GetLastError ());
    }
    else
    {
        started = false;
        CloseHandle (pi.hThread);
        CloseHandle (pi.hProcess);
    }
    return r;
#else
    started        = false;
    if (0 == kill (child_pid, SIGKILL))
    {
        child_pid = -1;
        return true;
    }
    else
    {
        perror ("Kill");
        return false;
    }
#endif
}

void Process::ProcData::Cleanup ()
{
    if (started.load ())
    {
        Stop ();
    }
}

bool Process::ProcData::IsRunning ()
{
#if _WIN32
    return started && WaitForSingleObject (pi.hProcess, 0) == WAIT_TIMEOUT;
#else
    return child_pid > 0 && kill (child_pid, 0) == 0;
#endif
}

uint32_t Process::ProcData::Pid () const
{
    if (!started)
        return 0;
#if _WIN32
    return (uint32_t) pi.dwProcessId;
#else
    return child_pid;
#endif
}

uint32_t Process::ProcessId ()
{
    return d->Pid ();
}
