#include <wiringPi.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <string>

template <unsigned int NUM_CHANNELS>
class Lights
{

public:
	Lights(int delayTime, std::string vixenFile);

	bool startShow(int startDelay = 0);


private:
	//Varibles
	int delayTime;
	const int MAX_CHAR_LIMIT = ((NUM_CHANNELS * 3) + (NUM_CHANNELS - 1)+4);
	std::fstream csv;

	std::string vixenFile;

	std::vector<int[NUM_CHANNELS]> parsedFile;
	
	
	
	bool parseFile();


};
