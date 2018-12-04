#include "lights.h"
//#include <wiringPi.h>

int main(int argc, const char** argv)
{	wiringPiSetup();

	pinMode(4, OUTPUT);

	std::cout << "Test" << std::endl;

	while(true)
	{

	digitalWrite(4, HIGH);

	delay(500);

	digitalWrite(4, LOW);

	delay(500);

	}
	
}
