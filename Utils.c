#pragma once
#ifndef UTILS
#define UTILS

#include <sys/timeb.h>
#include <stdlib.h>

static long long get_random_number(unsigned int seed, long long min, long long max)
{
    srand(seed);
    return min + rand() % (max + 1 - min);
}

static long long get_time_milliseconds()
{
    struct timeb timeBuffer = { 0 };
    ftime(&timeBuffer);
    return timeBuffer.time + timeBuffer.millitm * 1000;
}

#endif
