#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

#include <raylib.h>

#include "ffmpeg.h"

const int width = 1200;
const int height = 1200;
const Vector2 resolution = { width, height };

const int fps = 60;

int main() {
    
    InitWindow(width, height, "WINDOWTITLE");
    SetWindowPosition(300, 1200);
    SetTargetFPS(fps);


    // ToggleFullscreen();

    FFMPEG ffmpeg;
    // Image start = GenImagePerlinNoise(width, height, 0, 0, 1);
    Image start = GenImageWhiteNoise(width, height, 0.4);
    int corner = 80;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (i < corner and j < corner) ImageDrawPixel(&start, i, j, ColorFromHSV(rand() % 360, 1, 1));
            else ImageDrawPixel(&start, i, j, WHITE);
        }
    }

    RenderTexture2D state[2];
    state[0] = LoadRenderTexture(width, height);
    state[1] = LoadRenderTexture(width, height);
    UpdateTexture(state[0].texture, start.data);
    UpdateTexture(state[1].texture, start.data);
    SetTextureWrap(state[0].texture, TEXTURE_WRAP_REPEAT);
    SetTextureWrap(state[1].texture, TEXTURE_WRAP_REPEAT);
    SetTextureFilter(state[0].texture, TEXTURE_FILTER_ANISOTROPIC_16X);
    SetTextureFilter(state[1].texture, TEXTURE_FILTER_ANISOTROPIC_16X);

    Shader fs = LoadShader(NULL, "life.fs");
    int resolution_loc = GetShaderLocation(fs, "resolution");
    SetShaderValue(fs, resolution_loc, &resolution, SHADER_UNIFORM_VEC2);

    for (int i = 0; i < 1200 && !WindowShouldClose(); i++) {

        // update texture
        BeginTextureMode(state[i & 1]);
            ClearBackground(BLACK);
            BeginShaderMode(fs);
                DrawTexture(state[~i & 1].texture, 0, 0, WHITE);
            EndShaderMode();
        EndTextureMode();

        // draw to window
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(state[i & 1].texture, 0, 0, WHITE);
        EndDrawing();

        // send to ffmpeg
        Image frame = LoadImageFromTexture(state[i & 1].texture);
        ffmpeg.send(frame.data, width, height);
        UnloadImage(frame);
    }

    UnloadRenderTexture(state[0]);
    UnloadRenderTexture(state[1]);
    CloseWindow();
    return 0;
}
