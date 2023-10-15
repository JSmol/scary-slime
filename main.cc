#include <bits/stdc++.h>
using namespace std;

#include <raylib.h>

#include "ffmpeg.h"

int width = 1920;
int height = 1080;
const int fps = 60;

int main() {
    
    InitWindow(width, height, "WINDOWTITLE");
    ToggleFullscreen();
    SetTargetFPS(fps);

    FFMPEG ffmpeg;
    RenderTexture2D canvas = LoadRenderTexture(width, height);

    for (int i = 0; i < 300 && !WindowShouldClose(); i++) {
        BeginDrawing();
            BeginTextureMode(canvas);
                ClearBackground(GetColor(0x111111FF));
                DrawText("HELLO!", 0, 0, 20, GetColor(0xFFFFFFFF));
            EndTextureMode();
            DrawTexture(canvas.texture, 0, 0, WHITE);
        EndDrawing();
        Image frame = LoadImageFromTexture(canvas.texture);
        ffmpeg.send(frame.data, width, height);
        UnloadImage(frame);
    }

    UnloadRenderTexture(canvas);
    CloseWindow();
    return 0;
}
