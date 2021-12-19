source config.cfg

# Log text colors
PURPLE="\033[0;35m"
YELLOW="\033[1;33m"
RED="\x1b[31m"
NC='\033[0m'

plugin_name="image-manipulation"
build_directory="build"


build_path=$PWD/$build_directory
data_directory_path=$PWD/data
output_directory=$build_path/$plugin_name

# Function to add some nice colors to log text
message() 
{
    case "$2" in
        "warning")  echo "$YELLOW$1$NC";;
        "error") echo "$RED$1$NC";;
        *) echo "$PURPLE$1$NC";;
    esac
}


message "${YELLOW}Checking for build directory${NC}"

if [ -d $build_path ]
then
    message "Removing existing build folder..." warning
    rm -r $build_directory
fi

message "Creating build folder..."
mkdir $build_directory

cd "build"
message "Libobs directory set to: $OBS_PATH"
if cmake -DobsPath=$OBS_PATH .. # Variable taken from config.cfg
then   
    message "Running make command in ${build_directory} folder..."
    make
fi

message "Creating output directory..."
mkdir -p $output_directory/bin

# Copy the required files into the output directory which will be copied into the OBS plugins folder

echo "Looking for build plugin .so file: ${build_path}/${plugin_name}.so"
# Copy the built plugin .so file to the output plugin folder
if [ -f "${build_path}/${plugin_name}.so" ]
then
    
    cp "${build_path}/${plugin_name}.so" "$output_directory/bin/${plugin_name}.so"

    echo "Looking for data folder at" $data_directory_path

    # Copy the data to the output plugin folder
    if [ -d $data_directory_path ]
    then
        cp -R $data_directory_path $output_directory
    else
        message "Could not find the required 'data' folder in the root directory!" error
    fi

else
    message "Could not find plugin file: ${build_path}/${plugin_name}.so" error
fi



