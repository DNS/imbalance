
rm ms3d-loader.o ms3d-loader.exe

echo
echo "> Building libms3d.a"
echo

cd ../libms3d/src
rm libms3d.a
rm *.o

make static

echo
echo "> Building ms3d-loader.exe"
echo

cd ../../samples
make

./ms3d-loader.exe
