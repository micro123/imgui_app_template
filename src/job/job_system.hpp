#ifndef JS_H
#define JS_H

#include "config.h"

#include <memory>
#include <atomic>
#include <functional>
#include <list>
#include <cstdint>

using job_ptr  = std::shared_ptr<struct job>;
using job_proc = std::function<void()>;

struct job {
    job_ptr               parent{};
    std::atomic<uint32_t> unfinished_count{};
    std::list<job_ptr>    dependencies;

    job_proc              proc;
};

FW_API
job_ptr create_empty_job();

FW_API
job_ptr create_job(job_proc proc);

FW_API
job_ptr create_job_as_child(job_proc proc, job_ptr parent);

FW_API
void job_add_dependency(job_ptr job, job_ptr dependency);

inline void job_add_dependencies(job_ptr job) {}

template <typename F, typename ... T>
inline void job_add_dependencies(job_ptr job, F first, T ... more) {
    job_add_dependency(job, first);
    job_add_dependencies(job, more...);
}

FW_API
void submit_job(job_ptr job);

FW_API
void wait_job(job_ptr job);

#endif /* JS_H */
