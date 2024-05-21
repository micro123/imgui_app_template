#ifndef JS_PRIVATE_HPP
#define JS_PRIVATE_HPP

#include "job_system.hpp"

#define THREAD_LIMITS 15

void js_init(int thread_count = -1);

void js_terminate();

void init_work_threads(int count);

void stop_threads();

job_ptr get_job();

void job_push(job_ptr job);

void job_set_parent(job_ptr job, job_ptr parent);

void job_check_dependencies(job_ptr job);

void job_execute(job_ptr job);

bool job_is_done(job_ptr job);

#endif /* JS_PRIVATE_HPP */
