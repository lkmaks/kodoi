export PATH=~/qt_projects/mxe/usr/bin:$PATH
rm -rf tmp
mkdir tmp
cd tmp
~/qt_projects/mxe/usr/i686-w64-mingw32.static/qt5/bin/qmake ../
make
mv release/kodoi.exe ../
cd ..

