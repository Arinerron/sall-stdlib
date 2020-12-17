void exit() {
    // TODO
}

void panic(const char *msg) {
    puts("PANIC: ");
    puts(msg);
    putc('\n');
    exit();
}

#define ASSERT(statement, msg) if (!(statement)) { panic((msg)); }
