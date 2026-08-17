The basic commands are as follows:

ulc -dll -x86 *.c *.h library.dll
ulc -dll -x86 *.cpp *.hpp library.dll
ulc -dll -x86_64 *.c *.h library.dll
ulc -dll -x86_64 *.cpp *.hpp library.dll
ulc -so -arm64 *.c *.h library.so
ulc -so -arm64 *.cpp *.hpp library.so
ulc -so -arm32 *.c *.h library.so
ulc -so -arm32 *.cpp *.hpp library.so
ulc -dlib -arm64 *.c *.h library.dylib
ulc -dlib -arm64 *.cpp *.hpp library.dylib
ulc -dlib -arm32 *.c *.h library.dylib
ulc -dlib -arm32 *.cpp *.hpp library.dylib

Supports C | C++ source code and headers.