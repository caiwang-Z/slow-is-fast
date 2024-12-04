echo ##################### setup build configuration #####################

set BUILD_CONFIGURATION_DBGSTR=Debug
set BUILD_CONFIGURATION_RELSTR=Release

set BUILD_CONFIGURATION=%BUILD_CONFIGURATION_DBGSTR%

echo build config %BUILD_CONFIGURATION%

pause

echo ##################### setup build system environment #####################

call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64"

call set

echo ##################### change working dir to repository #####################

pushd %~dp0

cd ../..

set CUR_PROJECT=%cd%

echo %CUR_PROJECT%

rem echo ##################### setup 3rd party distributions #####################

rem TODO
rem pause

rem echo ##################### setup env var for configuration #####################

rem TODO
rem pause

echo ##################### create and switch to build dir #####################

mkdir %CUR_PROJECT%\build
cd %CUR_PROJECT%\build

echo ##################### configure for system #####################

cmake -B "%CUR_PROJECT%\build" -S "%CUR_PROJECT%" -G "Visual Studio 17 2022" -A x64 -DBUILD_CONCEPTS_APP=OFF -DBUILD_DUMMY_CMAKE_APP=OFF -DBUILD_ImageProcessSolution=OFF -DBUILD_CV_APP=ON -DCMAKE_BUILD_TYPE=%BUILD_CONFIGURATION% DCMAKE_RUNTIME_OUTPUT_DIRECTORY=%CMAKE_RUNTIME_OUTPUT_DIRECTORY% -DBUILD_SHARED_LIBS=ON -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_DOCS=OFF -DWITH_IPP=OFF -DWITH_CUDA=OFF -DWITH_OPENCL=OFF -DWITH_MSMF=OFF -DWITH_MSMF_DXVA=OFF -DWITH_FFMPEG=OFF -DINSTALL_PYTHON_EXAMPLES=OFF -DINSTALL_C_EXAMPLES=OFF -DBUILD_opencv_world=OFF -DWITH_AVIF=OFF -DINSTALL_CMAKE_CONFIG_MODULE=ON -DWITH_TIFF=ON -DWITH_VIDEOIO=ON -DBUILD_opencv_core=ON -DBUILD_opencv_imgproc=ON -DBUILD_opencv_imgcodecs=ON 

pause

rem echo ##################### build 3rd party with build system #####################

cmake --build . --target opencv_core opencv_imgproc opencv_imgcodecs --config %BUILD_CONFIGURATION% -j 4

rem pause

echo ##################### build modules with build system #####################

cmake --build . -j 4 --config %BUILD_CONFIGURATION%

rem echo ##################### change to bin output #####################

rem cd %CUR_PROJECT%\bin\%BUILD_CONFIGURATION%

rem echo ##################### copy runtime required data #####################

rem xcopy  src target  /e /i /h /y

rem pause
