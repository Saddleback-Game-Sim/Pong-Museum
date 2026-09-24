# Bundled FFmpeg (Windows x64)

- Version: `n7.1.5-12-g1fdbca85aa-20260731` (FFmpeg 7.1 branch).
- Distributor: [BtbN/FFmpeg-Builds](https://github.com/BtbN/FFmpeg-Builds).
- Release: [autobuild-2026-07-31-14-10](https://github.com/BtbN/FFmpeg-Builds/releases/tag/autobuild-2026-07-31-14-10).
- Archive: [ffmpeg-n7.1.5-12-g1fdbca85aa-win64-lgpl-shared-7.1.zip](https://github.com/BtbN/FFmpeg-Builds/releases/download/autobuild-2026-07-31-14-10/ffmpeg-n7.1.5-12-g1fdbca85aa-win64-lgpl-shared-7.1.zip).
- SHA-256: `0f376f96fb38554ccefb1b2ae9c7c6a7b351f0e60a372b38262c320e8392c5d0` (verified against the release asset digest).
- FFmpeg source revision: [`1fdbca85aa`](https://github.com/FFmpeg/FFmpeg/tree/1fdbca85aa).
- Build recipes and dependency sources: [BtbN build scripts](https://github.com/BtbN/FFmpeg-Builds/tree/autobuild-2026-07-31-14-10).

The package uses shared libraries, `--enable-version3`, and no `--enable-gpl`.
Its license is LGPLv3; retain `LICENSE.txt`, `COPYING.GPLv3`, and `LICENSE.md`
with redistributed copies. Upstream licensing notes also describe external
components. When distributing binaries, provide corresponding source and
build materials as required by their licenses.

`include/` and `lib/` are copied unmodified from the archive and include both
MSVC `.lib` and MinGW `.dll.a` import libraries. `bin/` contains the five
runtime DLLs used by raylib-media: avcodec, avformat, avutil, swresample,
and swscale. The archive's command-line executables, avdevice/avfilter DLLs,
documentation, and presets are not needed by this application and are omitted.

CMake copies the runtime DLLs beside the executable. The program loads them
dynamically; it does not invoke an installed `ffmpeg.exe`.
