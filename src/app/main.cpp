#include "application.hpp"
#include "utils/backward.hpp"

int main_proc() {
    auto app = CreateApplication();
    app->Init();
    return app->Exec();
}

int FW_API main() {
    backward::SignalHandling sh;
    return main_proc();
}
