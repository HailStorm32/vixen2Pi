See the vixenPi setup in action: <a href="https://youtu.be/kdkh6msmvUg">https://youtu.be/kdkh6msmvUg</a>

## Description:
Takes an exported <a href="http://www.vixenlights.com/downloads/vixen-3-downloads/">Vixen Lights</a> file and controls lights through a Pi making use of the (now deprecated) WiringPi library. Also works in conjuction with a  <a href="https://github.com/HailStorm32/lightshow-site">website</a> I created that is served by the Pi, and is used to control when the show is started as well as other things.


## Setup:


### Compile Environment: 
There are two ways to setup and compile. The first, and default way, is to cross compile on an Arch system and then copy the executable to the pi. This way assumes a machine running Arch and the `arm-linux-gnueabihf-gcc` cross compiler installed. The second way is to compile directly on the pi. However I don't recommend doing this if you plan on making changes to the code, as compile time is significantly longer on the pi.
><i>You could use a diffent version of Linux, however I have only tested on Arch</i>

This code has been writen to work with a mysql database, and needs login credentials. Current code looks for credentials in a `config.h` file.

##### Cross compiling on a secondary system:

1. Clone this repo `git clone https://github.com/HailStorm32/vixen2Pi.git`
2. Open the Makefile and change the `RPI_LIB_DIR` variable to where the cross compiler stores its libraries. For me its `/usr/arm-linux-gnueabihf/lib` 
3. Copy `crossCompile/arm-linux-gnueabihf'` to `/usr` and the make sure not to overwrite existing files
>Makes sure the necessary files for mysql cross compiling are included
3. After the above has been done, run `make` in the root directory of the project to compile and build the project
4. Transfer the executable to the pi by running `scp vixen2Pi PI_IP_ADDR:~`
5. Install wiringPi on the Pi by cloning this repo and navigating to `contrib/wiringPi` and running `sudo ./build`
6. Once wiringPi on the Pi is installed, you can remove the wiringPi repo that was cloned on the pi
7. See <a href="https://github.com/HailStorm32/vixen2Pi#running-and-using-vixen2Pi">Running and using vixen2Pi</a> for more information on running the executable

##### Compiling on the Pi:

1. Clone this repo `git clone https://github.com/HailStorm32/vixen2Pi.git`
4. Open the Makefile, and change the `TARGET` variable by commenting out its value like so `TARGET=#arm-linux-gnueabinf-`
5. Install wiringPi by navigating to `contrib/wiringPi` and running `sudo ./build`
6. After the above has been done, run `make` in the root directory of the project to compile and build the project
7. See <a href="https://github.com/HailStorm32/vixen2Pi#running-and-using-vixen2Pi">Running and using vixen2Pi</a> for more information on running the executable

<br>

### Webserver Setup:
Once you have setup your compile environent, now you need to setup the webpage that controls this program.
1. Follow the steps <a href="https://github.com/HailStorm32/lightshow-site">here</a>


<br>


## Running and using vixen2Pi

Make sure the executable, CSV file, and mp3 are in the same directory before running vixen2Pi.

