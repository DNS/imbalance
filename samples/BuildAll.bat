del ms3d-loader.exe
cd ..\libms3d\src
del libms3d.a
cls
mingw32-make

cd ..\..\samples
mingw32-make

ms3d-loader.exe
