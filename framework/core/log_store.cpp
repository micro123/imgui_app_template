#include <core/log_store.hpp>
#include <cstdlib>
#include <cassert>
#include "core/log.hpp"

log_item::log_item() {}

union log_index {
    u8 counts[LOG_LEVEL_COUNT];
    struct {
        u8 trace;
        u8 debug;
        u8 info;
        u8 warning;
        u8 error;
        u8 fatal;
    };
};

#define MAX_STORE_ITEMS  (4096)
#define STORE_MASK       (MAX_STORE_ITEMS-1)
#define LOG_ITEM_SIZE    (4096)
#define MAX_PAYLOAD_SIZE (LOG_ITEM_SIZE - sizeof(log_item))
#define INDEX_STEP_N     (100)
#define MAX_INDEX_ITEMS  (64)
#define INDEX_MASK       (MAX_INDEX_ITEMS - 1)
#define ALL_FILTER_EN    ((1u << LOG_LEVEL_COUNT) - 1)

static_assert((MAX_STORE_ITEMS & STORE_MASK) == 0, "CHECK MAX_STORE_ITEMS, Must be power of 2!!");
static_assert((MAX_INDEX_ITEMS & INDEX_MASK) == 0, "CHECK MAX_INDEX_ITEMS, Must be power of 2!!");

static log_item     *log_items[MAX_STORE_ITEMS] {};
static log_index    *log_indexes = nullptr;
static void         *memory_store = nullptr;
static void         *memory_index = nullptr;
static u64           store_head, store_tail;
static u64           index_head, index_tail;
static u8            filter_bits = ALL_FILTER_EN;
static u8            log_count_down = INDEX_STEP_N; // index step

void log_store::init() {
    char *ptr = (char*)malloc(MAX_STORE_ITEMS * LOG_ITEM_SIZE);
    memset(ptr, 0, MAX_STORE_ITEMS * LOG_ITEM_SIZE);
    memory_store = ptr;
    for (auto &item: log_items) {
        item = (log_item*)ptr;
        item->max_payload_size = MAX_PAYLOAD_SIZE;
        ptr += LOG_ITEM_SIZE;
    }
    log_indexes = (log_index*)malloc(MAX_INDEX_ITEMS*sizeof(log_index));
    memset(log_indexes, 0, MAX_INDEX_ITEMS*sizeof(log_index));
    memory_index = log_indexes;

    store_head = 0;
    store_tail = 0;
    index_head = 0;
    index_tail = 0;
}

void log_store::clean()
{
    free(memory_store);
    free(memory_index);
}

void log_store::clear() {
    store_head = 0;
    store_tail = 0;
    index_head = 0;
    index_tail = 0;

    log_count_down = INDEX_STEP_N;
    memset(&log_indexes[index_head%INDEX_MASK], 0, sizeof(log_index));
}

void log_store::push() {
    // increase count
    log_item *item = next();
    ++log_indexes[index_head%INDEX_MASK].counts[item->level];   // index
    ++store_head;   // store (item count)

    if (--log_count_down == 0) {
        log_count_down = INDEX_STEP_N;
        ++index_head; // next index
        memset(&log_indexes[index_head%INDEX_MASK], 0, sizeof(log_index)); // reset to zeros
        if (index_head - index_tail > 30) { // log count are more than 30 * 100, drop front 100
            ++index_tail; // move front
            store_tail+=100; // drop items
        }
    }
}

log_item *log_store::next() const {
    return log_items[store_head % STORE_MASK];
}

u32 log_store::count() const {
    if ((filter_bits & ALL_FILTER_EN) == ALL_FILTER_EN) {
        return store_head - store_tail;
    } else {
        u32 count = 0;
        for (u64 i=index_tail; i<=index_head; ++i) {
            for (u8 j=0; j < LOG_LEVEL_COUNT; ++j) {
                if ((1u<<j) & filter_bits)
                    count += log_indexes[i % INDEX_MASK].counts[j];
            }
        }
        return count;
    }
}

void log_store::set_filter(u8 level, bool active) {
    if (level < LOG_LEVEL_COUNT) {
        if (active) {
            // set bit
            filter_bits |= (1u << level);
        } else {
            // clear bit
            filter_bits &= ~(1u << level);
        }
    }
}

void log_store::reset_filter() {
    filter_bits = ALL_FILTER_EN;
}

void log_store::set_filter_value(u8 mask) {
    filter_bits = mask & ALL_FILTER_EN;
}

u8   log_store::filter_value() const {
    return filter_bits;
}

log_store::const_iter log_store::logs () const
{
    return {store_tail, store_head, filter_bits};
}

log_store::const_iter::const_iter (u64 beg, u64 end, u8 filter): beg(beg), end(end), filter(filter) {
    logic = 0;
    real = beg;
    while (0 == ((1u << log_items[real % STORE_MASK]->level) & filter) && real < end)
        ++real;
}
log_store::const_iter::~const_iter () {

}
u64 log_store::const_iter::count () const
{
    return log_store::instance()->count();
}
const log_item *log_store::const_iter::operator[] (u64 index)
{
    assert(index >= logic);

    while(logic < index) {
        do {
            ++real;
        } while (0 == ((1u << log_items[real % STORE_MASK]->level) & filter) && real < end);
        ++logic;
    }

    if (real < end)
        return log_items[real % STORE_MASK];
    return nullptr;
}
