#pragma once

#include <bits/stdc++.h>
#include <cstddef>

struct FFMPEG {
    FFMPEG();
    ~FFMPEG();
    int fd[2];
    int child;
    void send(void *data, size_t width, size_t height);
};

