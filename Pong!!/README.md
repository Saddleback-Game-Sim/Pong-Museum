# Pong...?

A simple Pong game made with raylib.

It starts as Pong.

That's about all you need to know.

## Controls

| Player | Up | Down |
| --- | --- | --- |
| Left Paddle | `W` | `S` |
| Right Paddle | `↑` | `↓` |

Try hitting the ball a few times

## Features

- Two-player Pong
- Paddle physics of questionable legality
- Video playback using `raymedia`
- FFmpeg-based media decoding
- Something fun happens eventually

## Build and Run

### Windows

The required Windows dependencies are bundled under `thirdparty/`.

Use an x64 toolchain.

#### Visual Studio / MSVC

From a Visual Studio x64 Developer Command Prompt with CMake and Ninja:

```sh
cmake -S . -B build/ninja-msvc -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/ninja-msvc
build\ninja-msvc\RaylibTemplate.exe
```

#### MinGW

Make sure GCC/G++ are available on `PATH`, then configure the project using a
separate build directory.

For example:

```sh
cmake -S . -B build/mingw -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/mingw
build\mingw\RaylibTemplate.exe
```

On Windows, keep the project and executable in ASCII-only paths for
compatibility with the bundled raylib and MinGW toolchain.

### macOS / Linux

Install:

- raylib
- pkg-config
- FFmpeg development libraries:
  - `avformat`
  - `avcodec`
  - `avutil`
  - `swresample`
  - `swscale`

Then configure and build with CMake using a separate build directory.

```sh
cmake -S . -B build
cmake --build build
```

## Media

Video playback is provided through `raymedia`, using FFmpeg for decoding.

The build copies the required media files and Windows runtime DLLs beside the
executable.

Media paths are resolved relative to the executable using
`GetApplicationDirectory()`, so the game can be launched from a different
working directory.

If the video cannot be loaded or decoded, the game will continue with a black
background.

If no audio device is available, video playback will continue without audio.

## Runtime Dependencies

MinGW builds copy the required C++ and threading runtime DLLs beside the
executable.

MSVC builds require the Visual C++ x64 Runtime, normally installed with Visual
Studio or the Microsoft Visual C++ Redistributable.

Media resources are released before shutting down the audio device and window.

## Third-Party Libraries

See:

- [Third-Party Notices](THIRD_PARTY_NOTICES.md)
- [FFmpeg Dependency Record](thirdparty/ffmpeg/README.md)

## Why?

We were supposed to make Pong.
