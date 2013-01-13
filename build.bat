if not exist .bin mkdir .bin
cp ../glew/bin/glew32.dll .bin
cp ../freeglut/lib/x86/freeglut.dll .bin

if not exist .build mkdir .build
cd .build

cmake ..
