#! /bin/sh
# Add cxxtest path to bin...
PATH=~/Projects/cxxtest/bin:$PATH

if [[ ! -d .build ]]; then
    mkdir .build
fi
cd .build
cmake -G "Xcode" ..
xcodebuild -project CrashCourseInOGL.xcodeproj -alltargets

#cmake -G "Unix Makefiles" ..
#make