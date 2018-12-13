#include "lights.h"
#include <time.h>
#include <cstring>

int main(int argc, const char** argv)
{	
	//make sure we have at least one argument, the file name counts as one
	if(argc < 2)
	{
		std::cout << "ERROR: Need at least 1 argument" << std::endl;
		return 0;
	}

	////////// SETUP ///////////
	Lights<8> show(50);

	bool isLightsOn = false;
	const int OFF_TIME = 8;
	const int ON_TIME = 16;
	const int NUM_SHOWS = 3;
	const int SHOW_TIMES[NUM_SHOWS] = {};

	//Setup the time variables
	time_t unxTime = time(NULL);
	struct tm *theTime = localtime(&unxTime);

	
	
	
	///////// CODE ////////////	
	
	if(strcmp(argv[1], "show") == 0)
	{
		show.startShow("Carol_of_the_bells.csv",4000);
	}
	else if(strcmp(argv[1], "run") == 0)
	{
		show.setState(1);//turn lights on	

		while(true)
		{
			//Turn off lights if we are in the OFF time period, and the lights are on
			if((theTime->tm_hour >= OFF_TIME && theTime->tm_hour < ON_TIME) && show.lightsState())
			{
				std::cout << "Turning OFF: " << theTime->tm_hour << ":" << theTime->tm_min << std::endl;//DEBUG
				show.setState(0);
			}	
			//Turn on lights if we are in the ON time period, and the lights are off
			else if((theTime->tm_hour >= ON_TIME || theTime->tm_hour < OFF_TIME) && !show.lightsState())
			{
				std::cout << "Turning ON: " << theTime->tm_hour << ":" << theTime->tm_min << std::endl;//DEBUG
				show.setState(1);
			}
		/*	else if()
			{

			}*/
			
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			
			unxTime = time(NULL);
			theTime = localtime(&unxTime);
		}
	}
	else
	{
		std::cout << "ERROR: Invalid argument: '" << argv[1] << "'" << std::endl;
		return 0;
	}
	

//	show.runTest();


	
	

}
