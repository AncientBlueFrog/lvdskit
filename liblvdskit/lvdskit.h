#ifndef LVDSKIT_H
#define LVDSKIT_H
#include <lvdsfloat.h>
#include <lvdshtable.h>
#include <lvdsllist.h>
#include <lvdsmath.h>
#include <lvdstrie.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char *get_string(const char *fmt, ...);

char get_char(const char *fmt, ...);

int get_int(const char *fmt, ...);

long get_long(const char *fmt, ...);

float get_float(const char *fmt, ...);

double get_double(const char *fmt, ...);

#endif
