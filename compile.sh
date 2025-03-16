#CD into the current directory
cd "$(dirname "$0")"

#If the build directory does not exist, create it
if [ ! -d "build" ]; then
  mkdir build
fi

#Check if CMakeLists.txt exists
if [ ! -f "CMakeLists.txt" ]; then
  echo "CMakeLists.txt not found"
  exit 1
fi

#CD into the build directory
cd build

#Run CMake
cmake ..

make -j$(nproc)