#include "lights.h"
//#include <wiringPi.h>

int main(int argc, const char** argv)
{	wiringPiSetup();

	pinMode(4, OUTPUT);

	std::cout << "Test" << std::endl;

	digitalWrite(4, HIGH);

	delay(5000);
	
}
