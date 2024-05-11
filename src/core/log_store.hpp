#ifndef LOG_STORE_HPP
#define LOG_STORE_HPP

#include "config.h"
#include "utils/singleton.hpp"
#include "core/log.hpp"

struct FW_API log_item {
    log_item();

    char datetime[32];
    u64  thread_id : 56;
    u64  level     : 8;
    u16  max_payload_size;
    u16  payload_size;
    char payload[0];
};

class FW_API log_store final: public PublicSingleton<log_store> {
public:
    class const_iter final {
    public:
        const_iter(u64 beg, u64 end, u8 filter);
        ~const_iter();

        u64 count() const;
        const log_item *operator[](u64 index);
    private:
        u64 beg;
        u64 end;
        u64 real;
        u64 logic;
        u8  filter;
    };

    void init();
    void clean();

    void set_filter(u8 level, bool active);
    void reset_filter();
    void set_filter_value(u8 mask);
    u8   filter_value() const;

    void push();
    log_item* next() const;
    void clear();
    u32  count() const;

    const_iter logs() const;
};

#endif /* LOG_STORE_HPP */
