turfs
=====
*"Get off mah lawn, platform gnostics."*

turfs is a virtual file system. (I guess that's what you'd call it, anyway.) At the moment it's completely geared for a particular use case (only two static virtual path roots, /assets and /userdata), but I plan to eventually extend it to support not only a variable number of virtual roots, but physical path merging and zlib compression and all sorts of other goodies.

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

