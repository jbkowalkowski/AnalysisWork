
setup uboonecode v05_11_01 -q e9:prof
setup cetbuildtools v5_04_00
# setup mrb v1_05_03

if [ -d build ]
then
echo "build directory already exists"
else
mkdir build
echo "made build directory"
fi

if [ -d install ]
then
echo "install directory already exists"
else
mkdir install
echo "made install directory"
fi

INSTALL_PATH=`pwd`/install
LIB_PATH=${INSTALL_PATH}/lib
INC_PATH=${INSTALL_PATH}/include
BIN_PATH=${INSTALL_PATH}/bin

if [ -z "$LD_LIBRARY_PATH" ]
then
 # is zero length
 export LD_LIBRARY_PATH=$LIB_PATH
else
 export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$LIB_PATH
fi

PATH=${PATH}:${BIN_PATH}

cd build
source ../src/ups/setup_for_development -p
