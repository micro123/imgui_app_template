#include "job_system_p.hpp"
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include "core/log.hpp"
#include "job_system.hpp"

#define QUEUE_MAX  (4096)
#define QUEUE_MASK (QUEUE_MAX-1)

class job_queue {
public:
    job_queue(): lo{0}, hi{0} {

    }

    bool push(job_ptr ptr) {
        bool ok = false;
        std::lock_guard lck(lock);
        if (hi - lo < QUEUE_MASK)
        {
            storage[(hi++)%QUEUE_MASK] = ptr;
            ok = true;
        }
        return ok;
    }

    job_ptr pop() {
        job_ptr ret{};
        std::lock_guard lck(lock);
        if (hi - lo != 0) {
            std::swap(storage[(--hi)%QUEUE_MASK], ret);
        }
        return ret;
    }

    job_ptr stealing() {
        job_ptr ret{};
        std::lock_guard lck(lock);
        if (hi - lo != 0) {
            std::swap(storage[(lo++)%QUEUE_MASK], ret);
        }
        return ret;
    }

private:
    std::atomic<uint32_t> lo;
    std::atomic<uint32_t> hi;
    std::mutex            lock;
    job_ptr               storage[QUEUE_MAX];
};

static std::thread*  work_threads[THREAD_LIMITS];
static volatile bool work_running[THREAD_LIMITS];
static job_queue     job_queues[THREAD_LIMITS + 1];
static int           thread_count;

// worker thread data
thread_local int       worker_id = 0; // 0 代表非工作线程


static void worker_func(int id, volatile bool *running_flag);

void init_work_threads(int count) {
    srand(time(NULL));

    thread_count = count;

    for (int i=0; i<thread_count; ++i) {
        work_running[i] = true;
        work_threads[i] = new std::thread(&worker_func, i + 1, work_running + i - 1);
    }
}

void stop_threads() {
    for (int i=0; i<thread_count; ++i) {
        work_running[i] = false;
        if (work_threads[i]->joinable())
            work_threads[i]->join();
        delete work_threads[i];
        work_threads[i] = nullptr;
    }
}

job_ptr get_job() {
    job_ptr job = job_queues[worker_id].pop();
    if (!job) {
        const int steal_id = rand() % (thread_count + 1);
        if (steal_id != worker_id) {
            job = job_queues[steal_id].stealing();
        }
    }
//    if (job) {
//        L_INFO("job %p", job.get());
//    }
    return job;
}

void job_push(job_ptr job) {
    while(!job_queues[worker_id].push(job));
}

void job_set_parent(job_ptr job, job_ptr parent) {
    job->parent = parent;
    if (job->parent) {
        job->parent->unfinished_count.fetch_add(1);
    }
}

void job_check_dependencies(job_ptr job)
{
    for (auto it = begin(job->dependencies); it != end(job->dependencies); ++it) {
        wait_job(*it);
    }
}

void job_execute(job_ptr job)
{
    job_check_dependencies(job);

    if (job->proc)
        job->proc();
    job->unfinished_count.fetch_sub(1);

    if (job->parent)
        job->parent->unfinished_count.fetch_sub(1);
}

bool job_is_done(job_ptr job) {
    return job->unfinished_count.load() == 0;
} 

void worker_func(int id, volatile bool *running_flag)
{
    // init
    worker_id = id;

    while(*running_flag) {
        job_ptr job = get_job();
        if (job) {
            job_execute(job);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
}
