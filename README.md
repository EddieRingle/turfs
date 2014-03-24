turfs
=====
*"Get off mah lawn, platform gnostics."*

### Building ###

Simple enough. Like any other CMake project, run the `cmake` command with your
favorite arguments. Then, depending on the build system that was generated,
build turfs from there.

##### ...for Android #####

Generating an Android build system requires at least the following arguments
to the `cmake` command (assuming we're building in a directory two levels down
from the source tree):

* `-DCMAKE_TOOLCHAIN_FILE=../../scripts/cmake/toolchains/toolchain.android.cmake`
* `-DTURFS_TARGET_PLATFORM=TURFS_TARGET_PLATFORM_ANDROID`
* `-DANDROID_API_LEVEL=14`

