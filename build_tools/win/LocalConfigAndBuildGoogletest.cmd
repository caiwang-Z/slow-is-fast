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

cmake -B "%CUR_PROJECT%\build" -S "%CUR_PROJECT%" -G "Visual Studio 17 2022" -A x64 -DBUILD_CONCEPTS_APP=OFF -DBUILD_DUMMY_CMAKE_APP=OFF -DBUILD_ImageProcessSolution=OFF -DBUILD_CV_APP=OFF -DBUILD_GOOGLETEST=ON

pause

rem echo ##################### build 3rd party with build system #####################

rem cmake --build . --target opencv_core opencv_imgproc opencv_imgcodecs --config %BUILD_CONFIGURATION% -j 4

rem pause

echo ##################### build modules with build system #####################

cmake --build . -j 4 --config %BUILD_CONFIGURATION%

rem echo ##################### change to bin output #####################

rem cd %CUR_PROJECT%\bin\%BUILD_CONFIGURATION%

rem echo ##################### copy runtime required data #####################

rem xcopy  src target  /e /i /h /y

rem pause 


rem echo ##################### run tests #####################


cd ../../build

ctest --verbose -j 6 -C %BUILD_CONFIGURATION%


rem echo ####################Installing the selected headers now#################


rem echo ####################Installing headers for BasicTools###################




