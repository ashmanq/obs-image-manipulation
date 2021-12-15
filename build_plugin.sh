source config.cfg

build_directory="build"
build_path=$PWD/$build_directory

echo "Path: ${build_path}"

echo "Checking for build directory"

if [ -d $build_path ]
then
    echo "Removing existing build folder..."
    rm -r $build_directory
fi

echo "Creating build folder..."
mkdir $build_directory
cd "build"
echo "Libobs directory set to: $DLIBOBS_INCLUDE_DIR"
cmake -DLIBOBS_INCLUDE_DIR=$DLIBOBS_INCLUDE_DIR .. # Variable taken from config.cfg
make