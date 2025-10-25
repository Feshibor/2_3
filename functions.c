#include "functions.h"
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static void int_to_roman(int value, char *buffer) {
    struct { int val; const char *rom; } table[] = {
        {1000,"M"},{900,"CM"},{500,"D"},{400,"CD"},
        {100,"C"},{90,"XC"},{50,"L"},{40,"XL"},
        {10,"X"},{9,"IX"},{5,"V"},{4,"IV"},{1,"I"}
    };
    buffer[0] = '\0';
    for (int i = 0; i < 13; i++) {
        while (value >= table[i].val) {
            strcat(buffer, table[i].rom);
            value -= table[i].val;
        }
    }
}

static void int_to_base(int value, int base, char *buffer, int uppercase) {
    if (base < 2 || base > 36) base = 10;
    char tmp[65];
    int i = 0, neg = 0;
    if (value < 0 && base == 10) { neg = 1; value = -value; }
    do {
        int d = value % base;
        tmp[i++] = (d < 10) ? ('0' + d) : (uppercase ? 'A' + d - 10 : 'a' + d - 10);
        value /= base;
    } while (value);
    int k = 0;
    if (neg) buffer[k++] = '-';
    while (i--) buffer[k++] = tmp[i];
    buffer[k] = '\0';
}

static void int_to_zeckendorf(unsigned int n, char *buffer) {
    unsigned int fib[64] = {1, 2};
    int count = 2;
    while (fib[count - 1] <= n) {
        fib[count] = fib[count - 1] + fib[count - 2];
        count++;
    }

    buffer[0] = '\0';
    for (int i = count - 2; i >= 0; i--) {
        if (fib[i] <= n) { strcat(buffer, "1"); n -= fib[i]; }
        else strcat(buffer, "0");
    }
    strcat(buffer, "1");
}

static void byte_to_bin(unsigned char byte, char *out) {
    for (int i = 7; i >= 0; i--) *out++ = (byte & (1 << i)) ? '1' : '0';
    *out = '\0';
}

static void memory_dump(const void *data, size_t size, char *out) {
    const unsigned char *p = (const unsigned char*)data;
    out[0] = '\0';
    char b[9];
    for (size_t i = 0; i < size; i++) {
        byte_to_bin(p[i], b);
        strcat(out, b);
        if (i != size - 1) strcat(out, " ");
    }
}

static int str_to_int_base(const char *s, int base, int uppercase) {
    if (base < 2 || base > 36) base = 10;
    char tmp[65];
    int i = 0;
    while (*s) tmp[i++] = uppercase ? toupper(*s) : tolower(*s), s++;
    tmp[i] = '\0';
    return (int)strtol(tmp, NULL, base);
}

static int voversprintf(char *str, const char *format, va_list args) {
    int written = 0;
    for (const char *p = format; *p; p++) {
        if (*p == '%' && *(p + 1)) {
            if (*(p + 1) == '%') { str[written++] = '%'; p++; continue; }
            p++;
            if (*p == 'R' && *(p + 1) == 'o') {
                p++;
                int v = va_arg(args, int);
                char buf[64];
                int_to_roman(v, buf);
                written += sprintf(str + written, "%s", buf);
            } else if (*p == 'Z' && *(p + 1) == 'r') {
                p++;
                unsigned v = va_arg(args, unsigned);
                char buf[128];
                int_to_zeckendorf(v, buf);
                written += sprintf(str + written, "%s", buf);
            } else if (*p == 'C' && (*(p + 1) == 'v' || *(p + 1) == 'V')) {
                int uppercase = (*(p + 1) == 'V');
                p++;
                int val = va_arg(args, int);
                int base = va_arg(args, int);
                char buf[65];
                int_to_base(val, base, buf, uppercase);
                written += sprintf(str + written, "%s", buf);
            } else if (*p == 'm' && *(p + 1)) {
                p++;
                char buf[256];
                if (*p == 'i') { int v = va_arg(args, int); memory_dump(&v, sizeof(int), buf); written += sprintf(str + written, "%s", buf);}
                else if (*p == 'u') { unsigned v = va_arg(args, unsigned); memory_dump(&v, sizeof(unsigned), buf); written += sprintf(str + written, "%s", buf);}
                else if (*p == 'f') { float v = (float)va_arg(args, double); memory_dump(&v, sizeof(float), buf); written += sprintf(str + written, "%s", buf);}
                else if (*p == 'd') { double v = va_arg(args, double); memory_dump(&v, sizeof(double), buf); written += sprintf(str + written, "%s", buf);}
                else { str[written++] = '%'; str[written++] = 'm'; str[written++] = *p; }
            } else if (*p == 't' && *(p + 1) == 'o') {
                p++;
                const char *s = va_arg(args, const char*);
                int base = va_arg(args, int);
                int val = str_to_int_base(s, base, 0);
                written += sprintf(str + written, "%d", val);
            } else if (*p == 'T' && *(p + 1) == 'O') {
                p++;
                const char *s = va_arg(args, const char*);
                int base = va_arg(args, int);
                int val = str_to_int_base(s, base, 1);
                written += sprintf(str + written, "%d", val);
            } else if (*p == 'd') {
                int v = va_arg(args, int);
                written += sprintf(str + written, "%d", v);
            } else if (*p == 's') {
                char *v = va_arg(args, char*);
                written += sprintf(str + written, "%s", v);
            } else if (*p == 'c') {
                int v = va_arg(args, int);
                written += sprintf(str + written, "%c", v);
            } else {
                str[written++] = '%';
                str[written++] = *p;
            }
        } else str[written++] = *p;
    }
    str[written] = '\0';
    return written;
}

int oversprintf(char *str, const char *format, ...) {
    va_list a;
    va_start(a, format);
    int len = voversprintf(str, format, a);
    va_end(a);
    return len;
}

int overfprintf(FILE *f, const char *format, ...) {
    char buf[2048];
    va_list a;
    va_start(a, format);
    voversprintf(buf, format, a);
    va_end(a);
    return fprintf(f, "%s", buf);
}
