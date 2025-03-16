#CD into the current directory
cd "$(dirname "$0")"

#If the build directory does not exist, terminate
if [ ! -d "build" ]; then
  echo "Build directory does not exist"
  exit 1
fi

#CD into the build directory
cd build

#Run the program
./pathplan