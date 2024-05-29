#include "page_base.hpp"

void Page::OnStart()
{

}

void Page::OnStop()
{

}

namespace {
    constexpr int   max_page_factory_cnt = 1024;
    int             page_cnt = 0;
    pFN_CreatePage  page_factories[max_page_factory_cnt];
}

PageRegistry::PageRegistry(pFN_CreatePage factory)
{
    page_factories[page_cnt++] = factory;
}

int PageRegistry::PageCount() {
    return page_cnt;
}

Page* PageRegistry::Get(int i) {
    return (page_factories[i])();
}
