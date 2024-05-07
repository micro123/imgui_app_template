#include "crash_handler.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "core/log.hpp"
#include "utils/path_tools.hpp"
#include "utils/subprocess.hpp"

#if __unix__
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include <linux/limits.h>
#include <cstdlib>
#include <vector>
#include "pmparser.h"
#elif _WIN32

#else
#error "Unknown Platform not supported!!"
#endif

#if __unix__

using handler_t = sighandler_t;

// we catch some signals
static handler_t handlers[_NSIG] {};

struct map_info_item{
    long start_addr;
    long end_addr;
    long offset;
    char file_path[PATH_MAX];

    long operator[](long addr) const {
        return addr - start_addr + offset;
    }
};

static std::vector<map_info_item> analysis_maps() {
    std::vector<map_info_item> result;

/*
    char cmd[256]{};

    snprintf(cmd, sizeof(cmd), "cat /proc/%d/maps | grep %x:%x | awk '{print $1\" \"$6}'", getpid(), dev.first, dev.second);

    L_INFO("%s", cmd);

    system(cmd);

    FILE *cat = popen(cmd, "r");

    // parse
    if (cat) {
        char *line = nullptr;
        size_t len;
        long   read;
        
        while((read=getline(&line, &len, cat)) != EOF)
        {
            line[read-1] = 0;
            bool need_start = false;
            char *ptr = strchr(line, ' ');
            *ptr = 0;
            if (result.empty() || 0 != strcmp(result.back().file_path, ptr+1)) {
                result.emplace_back();
                need_start = true;
                strcpy(result.back().file_path, ptr+1);
            }
            
            map_info_item &item = result.back();
            ptr=strchr(line, '-');
            *ptr=0;
            if (need_start)
                item.start_addr = strtol(line, nullptr, 16);
            item.end_addr = strtol(ptr+1, nullptr, 16);
            assert(item.start_addr < item.end_addr);
            // free(line);
            // line = nullptr;
        }
        if (line)
            free(line);

        pclose(cat);
    }
*/
    auto iter = pmparser_parse(-1);
    if (iter != nullptr) {
        procmaps_struct *ps = nullptr;
        while((ps = pmparser_next(iter)) != nullptr) {
            if (ps->length > 0 && ps->is_x && ps->pathname[0] == '/') {
                auto &back = result.emplace_back();
                back.start_addr = (intptr_t)ps->addr_start;
                back.end_addr = (intptr_t)ps->addr_end;
                back.offset = ps->offset;
                strncpy(back.file_path, ps->pathname, sizeof(map_info_item::file_path) - 1);
            }
        }
        pmparser_free(iter);
    }

    return result;
}

static void print_sym(int frame, const std::vector<map_info_item> &infos, long addr) {
    auto it = std::find_if(begin(infos), end(infos), [addr](const map_info_item& info){
        return info.start_addr <= addr && addr <= info.end_addr;
    });
    if (it == end(infos)) {
        L_WARN("#%d UNKNOWN 0x%lX", frame, addr);
    } else {
        // call addr2line
        char cmd[8192]{0};
        // L_INFO("file: %s, addr: %lx, start: %lx, end: %lx", it->file_path, addr, it->start_addr, it->end_addr);
        snprintf(cmd, sizeof(cmd), "addr2line -Cfp -e %s 0x%lx", it->file_path, (*it)[addr]);
        // puts(cmd);
        FILE *addr2line = popen(cmd, "r");
        if (addr2line) {
            char *line = nullptr;
            size_t len;
            long l = getdelim(&line, &len, '\n', addr2line);
            line[l-1] = 0;
            L_WARN("#%d %s", frame, line);
            free(line);
            pclose(addr2line);
        }
    }
}

static void print_stack_trace()
{
    L_WARN("!!App Crashed!!");
#define MAX_TRACE 64
    void *frames[MAX_TRACE] {};
    char **strings;

    int size, i;

    size = backtrace(frames, MAX_TRACE);
    strings = backtrace_symbols(frames, size);
    // backtrace_symbols_fd(frames, size, 2);

    auto maps = analysis_maps();

    if (strings != nullptr) {
        i = 3; // skip this function and handler and builtin __sigaction
        L_WARN("------ CALL STACKS ------");
        for(;i<size;++i) {
            print_sym(i-3, maps, (intptr_t)frames[i]);
        }
        L_WARN("-------------------------");
        free(strings);
    }
}

static void my_sig_handler(int sig) {
    print_stack_trace();
    if (handlers[sig]) {
        handlers[sig](sig);
    }
    exit(EXIT_FAILURE);
}

void system_install_handlers()
{
    static const int signals[] {
        SIGINT,
        SIGILL,
        SIGABRT,
        SIGFPE,
        SIGSEGV,
        SIGTERM,
    };

    for(auto const &sig: signals) {
        handlers[sig] = signal(sig, &my_sig_handler);
    }
}
#elif _WIN32
void system_install_handlers()
{
    // #error "To be implement!!"
}
#endif
