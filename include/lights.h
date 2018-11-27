#include <wiringPi>
#include <fstream>

class Lights
{

public:
	Lights(const int numOfChannels, const int delayTime, const string vixenFile);

	bool startShow(const int startDelay = 0);

private:
	int numOfChannels;
	int delayTime;
	
	ifstream csv;

	string vixenFile;
	

}
