#include <cstdint>
static int ReadPointer(int *ptr) {
    return (*ptr)++;
}

static void MakeMeCrash() {
    int *p = (int*)(intptr_t) &ReadPointer;
    [[maybe_unused]] int y = ReadPointer(p);
}

static void NestedFunction() {
    MakeMeCrash();
}

void SomeCrashFunction() {
    NestedFunction();
}
