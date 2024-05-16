//
// Created by tang on 23-2-5.
//

#include "core/log.hpp"
#include <atomic>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <string>
#include <utils/path_tools.hpp>
#include <core/log_store.hpp>
#include "fmt/format.h"
#include "fmt/chrono.h"

static constexpr const int MAX_THREAD_ID_LEN = 10;

static void                        log_vaprintf (s32 level, const char *fmt, va_list args);
// return thread_id
static u32                         format_thread_id (char (&buff)[MAX_THREAD_ID_LEN]);
static constexpr const char *const level_text[] = {"T", "D", "I", "W", "E", "F"};

static_assert (std::size (level_text) == LOG_LEVEL_FATAL + 1);

static std::string GetLogFileName();

#if defined(_WIN32)
#include <Windows.h>
#include <fcntl.h>
#include "utils/win32_utility.hpp"

// TRACE,DEBUG,INFO,WARN,ERR,CRITICAL
static constexpr WORD levels[LOG_LEVEL_COUNT] = {
    /* TRACE    */ FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // 白色前景色
    /* DEBUG    */ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // 白色前景色，加深
    /* INFO     */ FOREGROUND_INTENSITY | FOREGROUND_GREEN, // 绿色前景色
    /* WARN     */ FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED, // 黄色前景色
    /* ERROR    */ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED, // 红色前景色
    /* FATAL    */ FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // 红色背景色,加深
};

static constexpr WORD RESET_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

void log_vaprintf (s32 level, const char *fmt, va_list args)
{
    // make this function thread-safe
    static struct guard_tool {
        CRITICAL_SECTION cs;
        HANDLE           hLogFile;

        guard_tool()
        {
            InitializeCriticalSection(&cs);
            // time_t rawtime;
            // time(&rawtime);
            // const auto timeinfo = localtime(&rawtime);
            // char       log_name[64];
            // strftime(log_name, sizeof(log_name), "PiInfoApp_%Y_%m_%d.log.txt", timeinfo);
            std::string log_file_name = GetLogFileName();
            std::wstring full_path_wide = fromUtf8(GetDataPath(log_file_name.c_str()));
            hLogFile = CreateFileW(full_path_wide.c_str(),
                GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
            assert(hLogFile != INVALID_HANDLE_VALUE);
            LARGE_INTEGER set{.QuadPart = 0};
            SetFilePointerEx(hLogFile, set, NULL, FILE_END);
            SetConsoleOutputCP(CP_UTF8);
        }
        ~guard_tool()
        {
            CloseHandle(hLogFile);
            DeleteCriticalSection(&cs);
        }
    } guard;
    struct lock {
        lock()
        {
            EnterCriticalSection(&guard.cs);
        }
        ~lock()
        {
            LeaveCriticalSection(&guard.cs);
        }
    };

    lock lck{};

    HANDLE hConsole = level >= LOG_LEVEL_ERROR ? GetStdHandle(STD_ERROR_HANDLE) : GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, levels[level]);
    // const char* colour_strings[] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;30" };
    char        buffer[4096]{ 0 };
    auto const length = vsnprintf(buffer, sizeof(buffer), fmt, args);
    WriteConsole(hConsole, buffer, length, NULL, NULL);
    SetConsoleTextAttribute(hConsole, RESET_COLOR);
    WriteFile(guard.hLogFile, buffer, length, NULL, NULL);
}

u32 format_thread_id(char (&buff)[MAX_THREAD_ID_LEN])
{
    memset(buff, ' ', MAX_THREAD_ID_LEN);
    auto id = GetCurrentThreadId();
    int length = (int)log10(id-1);
    int offset = ((MAX_THREAD_ID_LEN - length)>>1)-1;
    offset += snprintf (buff + offset, MAX_THREAD_ID_LEN - offset, "%d", id);
    buff[offset] = ' ';
    buff[MAX_THREAD_ID_LEN-1] = 0;
    return (u32)id;
}

#else

#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <cmath>
#include <sys/types.h>

void log_vaprintf (s32 level, const char *fmt, va_list args)
{
    // make this function thread-safe
    static struct guard_tool {
        pthread_mutex_t lck;
        int             log_fd;

        guard_tool ()
        {
            pthread_mutex_init (&lck, nullptr);
            // time_t rawtime;
            // time (&rawtime);
            // const auto timeinfo = localtime (&rawtime);
            // char       log_name[64];
            // char       log_name_pattern[64];
            // snprintf(log_name_pattern, sizeof(log_name_pattern), "%s_%%Y_%%m_%%d.log", GetAppExecutablePath())
            // strftime (log_name, sizeof (log_name), "PiInfoApp_%Y_%m_%d.log.txt", timeinfo);
            std::string log_file_name = GetLogFileName();
            std::string full_path = GetDataPath (log_file_name.c_str());
            log_fd                = open (full_path.c_str (), O_WRONLY | O_CREAT | O_APPEND, 0666);
        }
        ~guard_tool ()
        {
            close (log_fd);
            pthread_mutex_destroy (&lck);
        }
    } guard;
    struct lock {
        lock ()
        {
            pthread_mutex_lock (&guard.lck);
        }
        ~lock ()
        {
            pthread_mutex_unlock (&guard.lck);
        }
    };

    lock lck{};

    const char *colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    char        buffer[4096] {0};
    vsnprintf (buffer, sizeof (buffer), fmt, args);
    fprintf (stdout, "\033[%sm%s\033[0m", colour_strings[LOG_LEVEL_FATAL - level], buffer);
    dprintf (guard.log_fd, "%s", buffer);
}

u32 format_thread_id(char (&buff)[MAX_THREAD_ID_LEN])
{
    memset(buff, ' ', MAX_THREAD_ID_LEN);
    pid_t id = gettid();
    int length = (int)log10(id-1);
    int offset = ((MAX_THREAD_ID_LEN - length)>>1)-1;
    offset += snprintf (buff + offset, MAX_THREAD_ID_LEN - offset, "%d", id);
    buff[offset] = ' ';
    buff[MAX_THREAD_ID_LEN-1] = 0;
    return (u32)id;
}

#endif

void log_printf (const char *file, s32 line, s32 level, const char *msg, ...)
{
    // initial log store
    static struct log_store_init_helper {
        log_store_init_helper() {
            log_store::instance()->init();
        }
        ~log_store_init_helper() {
            log_store::instance()->clean();
        }
    } helper_;

    assert (level <= LOG_LEVEL_FATAL);

    auto next = log_store::instance()->next();

    // char   time_buffer[64] {0};
    time_t rawtime;
    time (&rawtime);
    const auto timeinfo = localtime (&rawtime);
    strftime (next->datetime, sizeof (next->datetime), "%Y/%m/%d %H:%M:%S", timeinfo);
    
    char msg_buffer[1024] {0};
    char thr_id[MAX_THREAD_ID_LEN] {0};
    
    next->thread_id = format_thread_id (thr_id);
    next->level = level & 0xFF;

    if (level <= LOG_LEVEL_WARN)
    {
        snprintf (msg_buffer, sizeof (msg_buffer), "(%s)[%s][%s]: %s\n", next->datetime, thr_id, level_text[level], msg);
    }
    else
    {
        snprintf (msg_buffer, sizeof (msg_buffer), "(%s)[%s][%s]: {%s:%d}: %s\n", next->datetime, thr_id, level_text[level], file, line, msg);
    }

    va_list args;
    va_start (args, msg);
    next->payload_size = vsnprintf(next->payload, next->max_payload_size-1, msg, args);
    next->payload[next->payload_size] = 0;
    va_end (args);

    log_store::instance()->push();

    va_start (args, msg);
    log_vaprintf (level, msg_buffer, args);
    va_end (args);
}

std::string GetLogFileName()
{
    std::string prefix = GetAppExecutableName().data();
    return fmt::format("{}_{:%Y_%m_%d}.log.txt", prefix, fmt::localtime(std::time(nullptr)));
}
