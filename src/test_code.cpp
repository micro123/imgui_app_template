static int ReadPointer(int *ptr) {
    return *ptr;
}

static void MakeMeCrash() {
    int *p = nullptr;
    int y = ReadPointer(p);
}

static void NestedFunction() {
    MakeMeCrash();
}

void SomeCrashFunction() {
    NestedFunction();
}
