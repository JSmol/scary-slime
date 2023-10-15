#include "ffmpeg.h"

#include <bits/stdc++.h>
#include <cstdint>
#include <unistd.h>
#include <wait.h>
using namespace std;

const int READ = 0;
const int WRITE = 1;

FFMPEG::FFMPEG() {
    if (pipe(fd) < 0) throw std::runtime_error("error creating pipe");

    pid_t id = fork();
    if (id < 0) throw std::runtime_error("error forking");

    if (id == 0) { // child proc
        if (dup2(fd[READ], STDIN_FILENO) < 0) throw runtime_error("error opening read end of pipe as stdin");
        close(fd[WRITE]);
        int res = execlp("ffmpeg",
            "ffmpeg",
            "-loglevel", "verbose",
            "-y",
            "-f", "rawvideo",
            "-pix_fmt", "rgba",
            "-s", "1920x1080",
            "-r", "60",
            "-i", "-",
            "-c:v", "libx264",
            "-vb", "2500k",
            "-c:a", "aac",
            "-ab", "200k",
            "-pix_fmt", "yuv420p",
            "output.mp4",
            NULL
        );
        if (res < 0) throw runtime_error("error could not run ffmpeg");
        assert(0);
    } else { // main proc
        close(fd[READ]);
        child = id;
    }
}

FFMPEG::~FFMPEG() {
    close(fd[WRITE]);
    waitpid(child, NULL, 0);
}

void FFMPEG::send(void *data, size_t width, size_t height) {
    for (size_t y = 0; y < height; y++) {
        write(fd[WRITE], (uint32_t*)data + (height - y - 1) * width, sizeof(uint32_t)*width);
    }
}


