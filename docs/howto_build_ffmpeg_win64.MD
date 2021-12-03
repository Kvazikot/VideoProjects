1. Download MSYS
2. Download VS 2017 with Spectre libs
3. Run cmd
4. Make VS environment. Execute one of the following commands 
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
%comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
%comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

5. Run msys bash in vs environment
"C:\msys64\usr\bin\bash.exe"

6. Fix line endings in git global config. Clone ffmpeg from git
git config --global core.autocrlf false
git clone https://github.com/FFmpeg/FFmpeg.git

7. Cd to cloned ffmpeg directory
cd "D:\projects\VideoProjects\VideoCube\VideoCubeCV\libs\FFmpeg"

8. Install this packages.
pacman -S diffutils make pkg-config yasm

9. Run configure script
./configure --enable-asm --disable-yasm --target-os=win64 --arch=x86_64 --disable-avdevice --disable-swscale --disable-doc --disable-ffplay --disable-ffprobe --disable-ffmpeg --enable-shared --disable-static --disable-bzlib --disable-libopenjpeg --disable-iconv --disable-zlib --prefix=/c/ffmpeg --toolchain=msvc

10. make


