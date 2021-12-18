source config.cfg

# Log text colors
PURPLE='\033[0;35m'
YELLOW='\033[1;33m'
NC='\033[0m'

build_directory="build"
build_path=$PWD/$build_directory

# Function to add some nice colors to log text
message() 
{
    if test "$2" = "warning"
    then
        echo "$YELLOW$1$NC"
    else
        echo "$PURPLE$1$NC"
    fi
    
}

# message "Path: ${build_path}"

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

