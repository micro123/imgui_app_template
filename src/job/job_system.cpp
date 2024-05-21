#include "job_system.hpp"
#include "job_system_p.hpp"

#include <chrono>
#include <memory>
#include <thread>
#include <algorithm>


void js_init(int thread_count) {
    const int max_thread = std::min<int>(std::max<int>(2, std::thread::hardware_concurrency() - 1), THREAD_LIMITS);
    if (thread_count <= 0) {
        thread_count = max_thread;
    } else {
        thread_count = std::min<int>(thread_count, max_thread);
    }

    // create threads
    init_work_threads(thread_count);
}

void js_terminate() {
    stop_threads();
}

job_ptr create_empty_job() {
    auto j = std::make_shared<job>();
    j->unfinished_count.store(1);
    return j;
}

job_ptr create_job(job_proc proc) {
    auto job = create_empty_job();
    job->proc = proc;
    return job;
}

job_ptr create_job_as_child(job_proc proc, job_ptr parent) {
    auto job = create_job(proc);
    job_set_parent(job, parent);
    return job;
}

void job_add_dependency(job_ptr job, job_ptr dependency)
{
    job->dependencies.emplace_back(dependency);
}

void submit_job(job_ptr job) {
    job_push(job);
}

void wait_job(job_ptr job) {
    while(!job_is_done(job)) {
        job_ptr next = get_job();
        if (next) {
            job_execute(next);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
}
