# Arm CortexM55 OpenCV port

See original OpenCV README [at the bottom of this README.](#opencv-open-source-computer-vision-library).

Builds static libraries for `core`, `imgprocs` and `imgcodecs` using `arm-none-eabi-gcc` for an Arm CortexM55 MCU.

Uses OpenCV version `4.2.0` forked from commit `8808aaccffaec43d5d276af493ff408d81d4593c`

When built with `#define FREERTOS`, uses FreeRTOS headers from [FreeRTOS-Kernel tag V10.5.1](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/V10.5.1) for:
- `<mutex>` threading primitives wrapper
    - see `custom_mutex.hpp`
    - [FreeRTOS semaphores][https://www.freertos.org/Documentation/02-Kernel/04-API-references/10-Semaphore-and-Mutexes/00-Semaphores]
- `<pthread.h>` keys (thread local storage) wrapper
    - see `custom_pthread_keys.hpp`
    - [FreeRTOS Thread Local Storage Pointers](https://freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/16-Thread-local-storage-pointers)

Other manual changes for compilation where the removal of the following files (stubbing should be implemented in future so I don't have to remove these):
```
modules/imgproc/src/color_lab.cpp
modules/imgproc/src/fixedpoint.inl.hpp
modules/imgproc/src/resize.cpp
modules/imgproc/src/smooth.dispatch.cpp
modules/imgproc/src/smooth.simd.hpp
modules/core/src/parallel.cpp
```

And significant changes were made to:
```
modules/core/src/system.cpp
modules/core/src/softfloat.cpp
```

When linking against the generated `libopencv_core.a`, `libopencv_imgprocs.a` and `libopencv_codecs.a`, you will need a stubbed implementation of the OS functions:
```
_sbrk
_exit
_getpid
_kill
_isatty
_fstat
_lseek
_close
_open
_read
_write
```

To build:

```
./cmake_command.sh
cd build
make
```

Static library binary sizes (Bytes) are:
```
493904  libade.a
4989046 libopencv_core.a
275310  libopencv_imgcodecs.a
4499938 libopencv_imgproc.a
```

## OpenCV: Open Source Computer Vision Library

### Resources

* Homepage: <https://opencv.org>
* Docs: <https://docs.opencv.org/master/>
* Q&A forum: <http://answers.opencv.org>
* Issue tracking: <https://github.com/opencv/opencv/issues>

### Contributing

Please read the [contribution guidelines](https://github.com/opencv/opencv/wiki/How_to_contribute) before starting work on a pull request.

#### Summary of the guidelines:

* One pull request per issue;
* Choose the right base branch;
* Include tests and documentation;
* Clean up "oops" commits before submitting;
* Follow the [coding style guide](https://github.com/opencv/opencv/wiki/Coding_Style_Guide).
