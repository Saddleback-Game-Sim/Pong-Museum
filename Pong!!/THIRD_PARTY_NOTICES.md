# Third-Party Notices

## raylib

raylib is licensed under the zlib/libpng license. Its copyright and license
notice are retained in each bundled `include/raylib.h` file. Those files and
libraries are not covered by this repository's MIT license; they remain
available under raylib's own terms.

## raylib-media

The bundled `thirdparty/raylib-media/` sources provide video and audio media
playback support for raylib. raylib-media is licensed under the zlib license:
Copyright (c) 2024 Claudio Z. (@cloudofoz). Its complete license notice is
retained at the top of `raymedia.h` and `rmedia.c`.

Upstream: [cloudofoz/raylib-media](https://github.com/cloudofoz/raylib-media).

## FFmpeg

raylib-media links dynamically to FFmpeg's `libavcodec`, `libavformat`,
`libavutil`, `libswresample`, and `libswscale` libraries. The Windows x64
development files and runtime DLLs are bundled in `thirdparty/ffmpeg/`.
macOS and Linux builds use system development packages.

The bundled BtbN build enables `--enable-version3` and is distributed under
LGPLv3; its original [license](thirdparty/ffmpeg/LICENSE.txt), accompanying
[GPLv3 text](thirdparty/ffmpeg/COPYING.GPLv3), and upstream
[licensing notes](thirdparty/ffmpeg/LICENSE.md) are retained. See the
[dependency record](thirdparty/ffmpeg/README.md) for the exact version,
download, checksum, source, and build configuration references.

raylib-media, FFmpeg, and their dependencies remain under their respective
licenses and are not covered by this project's MIT license.
