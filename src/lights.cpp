#include "../include/lights.h"

template <unsigned int NUM_CHANNELS>
Lights<NUM_CHANNELS>::Lights(int delayTime, std::string vixenFile)
{
	this->delayTime = delayTime;
	this->vixenFile = vixenFile;
}


template <unsigned int NUM_CHANNELS>
bool Lights<NUM_CHANNELS>::startShow(int startDelay)
{
	//If there is a delay, run it
	if(startDelay > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(startDelay)); 
	}	
	return true;
}


template <unsigned int NUM_CHANNELS>
bool Lights<NUM_CHANNELS>::parseFile()
{
	int lineValues[NUM_CHANNELS] = {};
	char lineData[MAX_CHAR_LIMIT];
	
	csv.open(vixenFile);

	//Return error if cant open file
	if(!csv.is_open())
	{
		std::cout << "ERROR: Cant open file: " << vixenFile << std::endl;
		return false;
	}

	while(!csv.eof())
	{
		csv.getline(lineData, MAX_CHAR_LIMIT);

		//Make sure we didnt reach the char limit
		if(csv.fail())
		{
			std::cout << "ERROR: Expected less characters when reading line in:"<< vixenFile << std::endl;
		}

		//Go through the line we just got and take out the data
		for(int indx=0; indx < lineData.length(); indx++)
		{
			
		}
			
	}
}
