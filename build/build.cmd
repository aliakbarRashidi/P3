set Configuration=Debug
set PROFILE=MSVC2017
set Platform=x64
set Toolset=v141
set CMAKE_GENERATOR=Visual Studio 15 2017 Win64
set CMAKE_OPTIONS=-DUSE_RUNTIME_DLL=ON

set CMAKE_BUILD_TYPE=%Configuration%
set BUILD_TRIPLE=%PROFILE%-%Platform%-%CMAKE_BUILD_TYPE%
set INSTALL_DIR=../%BUILD_TRIPLE%
set BUILD_DIR=%BUILD_TRIPLE%-build

mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
cmake -G "%CMAKE_GENERATOR%" -T "%Toolset%" ../.. "-DCMAKE_BUILD_TYPE=%CMAKE_BUILD_TYPE%" %CMAKE_OPTIONS%
cmake --build . --config "%CMAKE_BUILD_TYPE%"
cmake "-DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%" "-DBUILD_TYPE=%CMAKE_BUILD_TYPE%" -P cmake_install.cmake

cd ..
