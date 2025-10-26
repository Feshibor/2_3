#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "functions.h"

void test_roman() {
    char buf[128];
    oversprintf(buf, "%Ro", 1987);
    assert(strcmp(buf, "MCMLXXXVII") == 0);
}



void test_base() {
    char buf[128];
    oversprintf(buf, "%Cv %CV", 255, 16, 255, 16);
    assert(strcmp(buf, "ff FF") == 0);
}

void test_memory_dump() {
    char buf[512];
    int x = -1048502157;
    unsigned int y = 12345;
    float f = 3.14f;
    double d = -2.71828;

    oversprintf(buf, "%mi %mu %mf %md", x, y, f, d);
    printf("Memory dump test output:\n%s\n", buf);
}

void test_to_TO() {
    char buf[128];
    oversprintf(buf, "%to %TO", "ff", 16, "FF", 16);
    assert(strcmp(buf, "255 255") == 0);
}

void test_standard() {
    char buf[128];
    oversprintf(buf, "%d %s %c", 123, "hello", 'A');
    assert(strcmp(buf, "123 hello A") == 0);
}

int main() {
    test_roman();
    test_base();
    test_memory_dump();
    test_to_TO();
    test_standard();

    printf("All tests passed successfully!\n");
    return 0;
}
