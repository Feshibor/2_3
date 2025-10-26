#include <stdio.h>
#include "functions.h"

int main() {
    char buf[2048];
    int si = -12345;
    unsigned int ui = 12345;
    float f = 3.14f;
    double d = -2.71828;

    oversprintf(buf,
        "Рим: %Ro\n"
        "Цек: %Zr\n"
        "База: %Cv %CV\n"
        "mi: %mi\n"
        "mu: %mu\n"
        "mf: %mf\n"
        "md: %md\n"
        "to: %to %to %to\n"
        "TO: %TO %TO %TO\n",
        1987,
        17u,
        255, 16,
        255, 16,
        si,
        ui,
        f,
        d,
        "ff", 16,
        "1010", 2,
        "1a", 16,
        "FF", 16,
        "1010", 2,
        "1A", 16
    );

    printf("%s", buf);
    return 0;
}
