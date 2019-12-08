#include "lights.h"
#include <fstream>
#include <string>
#include <time.h>
#include <cstring>
#include <unistd.h>

int main(int argc, const char** argv)
{	
	//make sure we have at least one argument, the file name counts as one
	if(argc < 2)
	{
		std::cout << "WARNING: Need at least 1 argument. Running default." << std::endl;
        argv[1] = "run";
	}

	////////// SETUP ///////////
	Lights<8> show(25);
    
    show.parseFile("/home/showrunner/Carol_of_the_bells_300_500_100.csv");


	bool isLightsOn = false;
	const int OFF_TIME = 8;
	const int ON_TIME = 16;
	const int NUM_SHOWS = 3;
	const int SHOW_TIMES[NUM_SHOWS] = {};
    const std::string FILE_PATHS [2] = {"/var/www/villardlight.show/public/variables.txt", "/var/www/villardlight.show/public/showStart.txt"};

	//Setup the time variables
	time_t unxTime = time(NULL);
	struct tm *theTime = localtime(&unxTime);

    /////////FILE VARIABLE RESET///////
    std::fstream file;
    std::string resetData = "000000000000000";

    for(int i = 0; i !=2; i++)
    {
	    file.open(FILE_PATHS[i]);

	    //Return error if cant open file
	    if(!file.is_open())
    	{
	    	std::cout << "ERROR: Cant open file: " << FILE_PATHS[i] << std::endl;
    	}

        file.write(resetData.c_str(),resetData.size());
        file.close();
    }

	
	///////// CODE ////////////	
	
	if(strcmp(argv[1], "show") == 0)
	{
		show.startShow("Carol_of_the_bells_300_500_100.csv", "carolOfTheBells.mp3", 2000, 4510);
	}
	else if(strcmp(argv[1], "run") == 0)
	{
        std::cout << "vixen2Pi Started" << std::endl;
        std::string word = "read";
        std::string fileContents;
        std::ofstream fileWrite;
        std::ifstream fileRead;
        
        show.setState(1);//turn lights on

        while(true)
        {
            //Code for checking the file that tells us to start the show 
            fileRead.open("/var/www/villardlight.show/public/showStart.txt");
            if(!fileRead.is_open())
            {
                std::cout << "ERROR: Cant open file: /var/www/villardlight.show/public/showStart.txt " << std::endl;
            }
            
            std::getline(fileRead, fileContents);
            fileRead.close();

            if(fileContents == "start")
            {
                fileWrite.open("/var/www/villardlight.show/public/showStart.txt");
                if(!fileWrite.is_open())
                {
                    std::cout << "ERROR: Cant open file: /var/www/villardlight.show/public/showStart.txt " << std::endl;
                }
               
                //Let the server know that the show is starting 
                fileWrite.write(word.c_str(), word.length()); 
                fileWrite.close();
		        show.startShow("Carol_of_the_bells_300_500_100.csv", "/home/showrunner/carolOfTheBells.mp3", 0, 4510);
            }
            

            //Code for puting lights on a timer when not in a show

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
			
			unxTime = time(NULL);
			theTime = localtime(&unxTime);

			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
	else if(strcmp(argv[1], "debug") == 0)
	{
		if(argc < 6)
		{
			std::cout << "\nERROR: Need more arguments for command" << argv[1] << std::endl;
			return 0;
		}

		int fileTime = std::stoi(argv[2]);
		int delayAdj = std::stoi(argv[4]);
		int mpvWaitTime = std::stoi(argv[3]);

		std::cout << "fileTiming:  " << fileTime << " | delayAdj: " << delayAdj << " | mpvWait: " << mpvWaitTime << std::endl;

		Lights<8> debugShow(fileTime, delayAdj);
        
        debugShow.parseFile(argv[5]);

		if(fileTime == 25)
		{
			debugShow.startShow(argv[5],"carolOfTheBells.mp3", 2000, mpvWaitTime);
		}
		else
		{
			debugShow.startShow("Carol_of_the_bells.csv", "carolOfTheBells.mp3", 2000, mpvWaitTime);
		}
	}
	else
	{
		std::cout << "ERROR: Invalid argument: '" << argv[1] << "'" << std::endl;
		return 0;
	}
	

//	show.runTest();


	
	

}
