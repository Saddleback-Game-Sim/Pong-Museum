# Pong with a video background

Play Pong over `res/test.mp4`, with its original audio looping in the
background. W/S move the left paddle; Up/Down move the right paddle. Escape
closes the game. A 35% black overlay keeps the ball, paddles, and score readable.

## Build and run

Windows dependencies are bundled under `thirdparty/`. Use an x64 toolchain.
From a Visual Studio x64 Developer Command Prompt with CMake and Ninja:

```sh
cmake -S . -B build/ninja-msvc -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/ninja-msvc
build\ninja-msvc\RaylibTemplate.exe
```

For MinGW, run the same configuration with GCC/G++ on PATH and a separate
build directory. On Windows, keep the project and executable in ASCII-only
paths for compatibility with the bundled raylib and MinGW toolchain.
On macOS/Linux, install raylib, pkg-config, and FFmpeg
development packages (avformat, avcodec, avutil, swresample, swscale), then
configure and build with CMake as above in a separate directory.

The build copies `res/test.mp4` and the Windows runtime DLLs beside the
executable. Keep that directory together when moving the program. Video
paths are resolved from the executable using raylib's `GetApplicationDirectory()`,
so launching from another working directory is supported.

MinGW builds also copy the compiler's C++ and thread runtime DLLs beside the
executable. MSVC builds require the Visual C++ x64 runtime installed by Visual
Studio or the Visual C++ Redistributable.

Missing or undecodable video produces a warning and a playable black-background
game. An unavailable audio device falls back to silent video. Media resources
are released before shutting down audio and the window.

See [third-party notices](THIRD_PARTY_NOTICES.md) and the
[FFmpeg dependency record](thirdparty/ffmpeg/README.md).
