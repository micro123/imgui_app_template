#ifndef PAGE_BASE_HPP
#define PAGE_BASE_HPP

class Page {
public:
    virtual ~Page() = default;

    virtual void OnStart();
    virtual void OnFrame(double delta, double total) = 0;
    virtual void OnStop();
    virtual const char *Name() const = 0;
};

using pFN_CreatePage = Page*(*)();

class PageRegistry final {
public:
    PageRegistry(pFN_CreatePage factory);
    ~PageRegistry() = default;

    static int   PageCount();
    static Page* Get(int i);
};

#define REGISTER_PAGE(T,...) \
static Page* T##_Factory() { return new T{__VA_ARGS__}; } \
static PageRegistry T##_Page(T##_Factory)

#endif /* PAGE_BASE_HPP */
