## Setup:
There are two ways to setup and compile. The first, and default way, is to cross compile on an Arch system and then copy the executable to the pi. This way assumes a machine running Arch and the `arm-linux-gnueabihf-gcc` cross compiler installed. The second way is to compile directly on the pi. However I don't recommend doing this if you plan on making changes to the code, as compile time is significantly longer on the pi.
><i>You could use a diffent version of Linux, however I have only tested on Arch</i>

##### Cross compiling on a secondary system:

1. Clone this repo `git clone https://github.com/HailStorm32/vixen2Pi.git`
2. In the cloned directory run `git submodule init` then `git submodule update`
3. Make a dir called `obj` in the `src` directory. Your resolting path should look like this `vixen2Pi/src/obj`
4. Open the Makefile and change the `RPI_LIB_DIR` variable to where the cross compiler stores its libraries. For me its `/usr/arm-linux-gnueabihf/lib` Make sure the directory includes the following lib files `libm.so, libpthread.so, librt.so, libcrypt.so`
5. After the above has been done, run `make` in the root directory of the project to compile and build the project
6. Transfer the executable to the pi by running `scp vixen2Pi PI_IP_ADDR:`
7. On the Pi, install wiringPi by following the instructions <a href="http://wiringpi.com/download-and-install/">here</a>
8. Once wiringPi is installed, you can remove the wiringPi repo that was cloned
9. See <i>Running and using vixen2Pi</i> for more information on running the executable

##### Compiling on the Pi:

1. Clone this repo `git clone https://github.com/HailStorm32/vixen2Pi.git`
2. In the cloned directory run `git submodule init` then `git submodule update`
3. Make a directory called `obj` in the `src` directory. Your resulting path should look like this `vixen2Pi/src/obj`
4. Open the Makefile, and change the `TARGET` variable by commenting out its value like so `TARGET=#arm-linux-gnueabinf-`
5. Install wiringPi by navigating to `contrib/wiringPi/wiringPi` and running `sudo ./build`
6. After the above has been done, run `make` in the root directory of the project to compile and build the project
7. See <i>Running and using vixen2Pi</i> for more information on running the executable

<br>

## Running and using vixen2Pi
Coming soon
