#include "application.hpp"
#include "utils/backward.hpp"

#if defined(_WIN32)
#include <Windows.h>
#endif

ENTRY_DECL();

int main_proc() {
    ENTRY_INIT();
    std::unique_ptr<Application> app {CreateApplication()};
    app->Init();
    return app->Exec();
}

int FW_API main() {
    backward::SignalHandling sh;
    return main_proc();
}
