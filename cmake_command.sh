rm -r build -y
mkdir build
cd build
cmake 									\
-DWE2=ON								\
-DFREERTOS=ON							\
-DCMAKE_BUILD_TYPE=Release				\
-DBUILD_SHARED_LIBS=OFF 				\
-DCV_DISABLE_OPTIMIZATION=OFF 			\
-DWITH_IPP=OFF 							\
-DWITH_TBB=OFF 							\
-DWITH_OPENMP=OFF 						\
-DWITH_PTHREADS_PF=OFF 					\
-DWITH_QUIRC=OFF 						\
-DWITH_1394=OFF 						\
-DWITH_CUDA=OFF 						\
-DWITH_OPENCL=OFF 						\
-DWITH_OPENCLAMDFFT=OFF 				\
-DWITH_OPENCLAMDBLAS=OFF 				\
-DWITH_VA_INTEL=OFF 					\
-DWITH_EIGEN=OFF						\
										\
-DWITH_GSTREAMER=OFF 					\
-DWITH_GTK=OFF 							\
-DWITH_JASPER=OFF 						\
-DWITH_JPEG=OFF 						\
-DWITH_WEBP=OFF 						\
-DBUILD_ZLIB=ON							\
-DBUILD_PNG=ON 							\
-DWITH_TIFF=OFF 						\
-DWITH_V4L=OFF 							\
-DWITH_LAPACK=OFF						\
-DWITH_ITT=OFF 							\
-DWITH_PROTOBUF=OFF 					\
-DWITH_IMGCODEC_HDR=OFF 				\
-DWITH_IMGCODEC_SUNRASTER=OFF 			\
-DWITH_IMGCODEC_PXM=OFF 				\
-DWITH_IMGCODEC_PFM=OFF 				\
										\
-DBUILD_LIST=core,imgproc,imgcodecs		\
-DBUILD_JAVA=OFF						\
-DBUILD_opencv_python=OFF 				\
-DBUILD_opencv_java=OFF 				\
										\
-DBUILD_opencv_apps=OFF					\
-DBUILD_PACKAGE=OFF						\
-DBUILD_PERF_TESTS=OFF					\
-DBUILD_TESTS=OFF						\
-DCV_ENABLE_INTRINSICS=OFF				\
-DCV_TRACE=OFF							\
-DOPENCV_ENABLE_MEMALIGN=OFF			\
										\
-DWITH_OPENEXR=OFF						\
-DCMAKE_TOOLCHAIN_FILE=../platforms/arm-none-eabi.toolchain.cmake \
..
cp ../alloc_fix.cpp ./3rdparty/ade/ade-0.1.1f/sources/ade/source/alloc.cpp
