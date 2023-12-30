
# Building Redtangle

Start by cloning the repo:

    git clone https://github.com/sarrettb/dev.git

Step into the repo directory:

    cd dev

Update all of the git submodules to obtain the dependencies:

    git submodule update --init --recursive

If building on Windows, gRPC must be installed locally on the system. See the following for instructions:

    https://sanoj.in/20 20/05/07/working-with-grpc-in-windows.html

Verify cmake is installed and the version is at least 3.5:

    cmake --version 

Set the current directory to the redtangle folder:

    cd redtangle

Create the build folder:

    mkdir -p cmake/build 

Push the build directory:

    pushd cmake/build 

Create the build spec:
    
    cmake ../.. -DSDL_STATIC=ON -DBUILD_SHARED_LIBS=OFF

If building on Windows, -DCMAKE_TOOLCHAIN_FILE=<`path`> needs to also be specified. 

Build the executable:

    cmake --build . 

Run the executable (path is for windows build):

    ./Debug/redtangle.exe

Pop the directory to go back to redtangle folder:

    popd

Note: In order to see the redtangle icon, you must manually copy the file into the executable directory. This functionality is handled when using the installer.  