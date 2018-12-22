## Setup:
There are two ways to setup and compile. The first, and default way, is to cross compile on an Arch system and then copy the executable to the pi. This way assumes a machine running Arch and the `arm-linux-gnueabihf-gcc` cross compiler installed. The second way is to compile directly on the pi. However I don't recommend doing this if you plan on making changes to the code, as compile time is significantly longer on the pi.
><i>You could use a diffent version of Linux, however I have only tested on Arch</i>

##### Cross compiling on a secondary system:

1. Clone this repo `git clone https://github.com/HailStorm32/vixen2Pi.git`
2. In the cloned dir run `git submodule init` then `git submodule update`
3. Open the Makefile and change the `RPI_LIB_DIR` variable to where the cross compiler stores its libraries. For me its `/usr/arm-linux-gnueabihf/lib` Make sure the dir includes the following lib files `libm.so, libpthread.so, librt.so, libcrypt.so`
4. After the above has been done, run `make` in the root dir of the project to compile and build the project
5. Transfer the executable to the pi by running `scp vixen2Pi PI_IP_ADDR:`
6. On the Pi, install wiringPi by following the instructions <a href="http://wiringpi.com/download-and-install/">here</a>
7. Once wiringPi is installed, you can remove the wiringPi repo that was cloned
6. See <i>Running and using vixen2Pi</i> for more information on running the executable

##### Compiling on the Pi:
Coming soon

<br>

## Running and using vixen2Pi
Coming soon
