#! /bin/sh
if [[ ! -d .build ]]; then
    mkdir .build
fi
cd .build
cmake -G "Xcode" ..
xcodebuild -project HelloOpenGL.xcodeproj -alltargets

#cmake -G "Unix Makefiles" ..
#make