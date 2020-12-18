#include "lights.h"
#include "config.h"
#include <fstream>
#include <string>
#include <time.h>
#include <cstring>
#include <unistd.h>
#include <mysql/mysql.h>


//TODO make this work for other tabels
bool updateDB(MYSQL *sqlConn, int state, std::string channel)
{
    MYSQL_RES *sqlResult;
    MYSQL_ROW sqlRow;
    std::string query;
    std::string stateStr;
    std::string errorMsg;

    if(!sqlConn)
    {
        std::cout << "ERROR: updateDB cant connect to SQL database!" << std::endl;
        mysql_close(sqlConn);
        return 1;
    }
    
    if(state <= 0)
    {
        stateStr = "off";
    }
    else
    {
        stateStr = "on";
    }

    if(channel == "all")
    {
        for(int indx = 1; indx < 9; indx++)
        {
            channel = "ch" + std::to_string(indx);

            query = "UPDATE channels SET state = '" + stateStr + 
                "' WHERE channel = '" + channel + "'";

            //std::cout << query << std::endl;//DEBUG

            if(mysql_query(sqlConn, query.c_str()))
            {
                 std::cout << "ERROR: sql query failed in updateDB on pass " 
                     << indx << " !" << std::endl;
                 errorMsg = mysql_error(sqlConn);
                 std::cout << errorMsg << std::endl;
                 mysql_close(sqlConn);
                 return false;
            }
        }
    }
    else
    {
        query = "UPDATE channels SET state = '" + stateStr + 
            "' WHERE channel = '" + channel + "'";

        if(mysql_query(sqlConn, query.c_str()))
        {
             std::cout << "ERROR: sql query failed in updateDB!" << std::endl;
             errorMsg = mysql_error(sqlConn);
             std::cout << errorMsg << std::endl;
             mysql_close(sqlConn);
             return false;
        }

    }
    return true;
}

bool isSysOn(MYSQL *sqlConn)
{
    MYSQL_RES *sqlResult;
    MYSQL_ROW sqlRow;
    std::string value;
    std::string name;
    std::string errorMsg;

    if(mysql_query(sqlConn, "SELECT * FROM variables"))
    {
        std::cout << "ERROR: sql query failed in isSysOn!" << std::endl;
        errorMsg = mysql_error(sqlConn);
        std::cout << errorMsg << std::endl;
        mysql_close(sqlConn);
        while(true){}
    }

    sqlResult = mysql_store_result(sqlConn);

    while((sqlRow = mysql_fetch_row(sqlResult)) != NULL)
    {
        name = sqlRow[0];
        if(name == "systemState")
        {
            value = sqlRow[1];
            if(value == "on")
            {
                mysql_free_result(sqlResult);
                return true;
            }
            else if(value == "off")
            {
                mysql_free_result(sqlResult);
                return false;
            }
            else
            {
                std::cout << "ERROR: invalid state in isSysOn!" << std::endl;
                while(true){}
            }

        }
    }
    std::cout << "ERROR: exited loop without return in isSysOn!" << std::endl;
    while(true){}
}

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
    MYSQL *sqlConn;
    MYSQL_RES *sqlResult;
    MYSQL_ROW sqlRow;

    show.parseFile("/home/showrunner/Carol_of_the_bells_300_500_100.csv");//Carol_of_the_bells_300_500_100.csv


	const int OFF_TIME = 8;
	const int ON_TIME = 16;
	const int NUM_SHOWS = 3;
	const int SHOW_TIMES[NUM_SHOWS] = {};

	bool isLightsOn = false;
    bool gotToResult = false;
    std::string query[2];
    std::string value;
    std::string name;
    std::string errorMsg;

    
    //Connet to database
    sqlConn = mysql_init(NULL);
    sqlConn = mysql_real_connect(sqlConn, DB_IP.c_str(), DB_USER.c_str(), 
            DB_PASS.c_str(), DB_NAME.c_str(), DB_PORT, NULL,0);
    if(!sqlConn)
    {
        std::cout << "ERROR: cant connect to SQL database!" << std::endl;
        errorMsg = mysql_error(sqlConn);
        std::cout << errorMsg << std::endl;
        mysql_close(sqlConn);
        return 1;
    }

	//Setup the time variables
	time_t unxTime = time(NULL);
	struct tm *theTime = localtime(&unxTime);

    /////////FILE VARIABLE RESET///////
    query[0] = "UPDATE variables SET value = '0000' WHERE " 
        "variable = 'showStartAck'";
    query[1] = "UPDATE variables SET value = '0000' WHERE " 
        "variable = 'showWaitTime'";
    for(int i = 0; i <= 1; i++)
    {
        if(mysql_query(sqlConn, query[i].c_str()))
        {
             std::cout << "ERROR: sql query failed to reset variables on try #:" 
                 << i+1 << "In main" << std::endl;
             errorMsg = mysql_error(sqlConn);
             std::cout << errorMsg << std::endl;
             mysql_close(sqlConn);
             return 1;
        }
    }
	
	///////// CODE ////////////	
	
	if(strcmp(argv[1], "show") == 0)
	{
		show.startShow("Carol_of_the_bells_300_500_100.csv", 
                "carolOfTheBells.mp3", 2000, 4510); //Carol_of_the_bells_300_500_100.csv
	}
	else if(strcmp(argv[1], "run") == 0)
	{
        std::cout << "vixen2Pi Started" << std::endl;
        std::string channel;
        std::string state;

        show.setState(1,"all");//turn lights on
        updateDB(sqlConn, 1, "all");
        

        while(true)
        {
            //Code for checking database to see if the show needs to be started 
            if(mysql_query(sqlConn, "SELECT * FROM variables"))
            {
                std::cout << "ERROR: sql query failed to retrive showStartAck!"
                    << std::endl;
                errorMsg = mysql_error(sqlConn);
                std::cout << errorMsg << std::endl;
                mysql_close(sqlConn);
                return 1;
            }

            sqlResult = mysql_store_result(sqlConn);

            while((sqlRow = mysql_fetch_row(sqlResult)) != NULL)
            {
                name = sqlRow[0];
                if(name == "showStartAck")
                {
                    value = sqlRow[1];
                    if(value == "start")
                    {
                        mysql_free_result(sqlResult);
                        
                        //Update database to let the server know that 
                        //we saw their ack and that the show is starting 
                        query[0] = "UPDATE variables SET value = 'read' " 
                            "WHERE variable = 'showStartAck'";
                        if(mysql_query(sqlConn, query[0].c_str()))
                        {
                             std::cout << "ERROR: sql query failed to " << 
                                 "update showStartAck!" << std::endl;
                             errorMsg = mysql_error(sqlConn);
                             std::cout << errorMsg << std::endl;
                             mysql_close(sqlConn);
                             return 1;
                        }

		                show.startShow("Carol_of_the_bells_300_500_100.csv", 
                                "/home/showrunner/carolOfTheBells.mp3", 0, 4510);

                        gotToResult = true;
                        break; 
                    }
                    else
                    {
                        mysql_free_result(sqlResult);
                        gotToResult = true;
                        break; 
                    }
                }
            }
            //Throw an error if we never got to the showStartAck variable
            if(gotToResult == false)
            {
                mysql_free_result(sqlResult);
                std::cout << "ERROR: exited loop without retreving" <<
                    " showStartAck" << std::endl;
                return 1;
            }
            gotToResult = false;//reset the flag
            

            //Code for puting lights on a timer when not in a show

			//Turn off lights if we are in the OFF time period, and the lights are on
			if((theTime->tm_hour >= OFF_TIME && theTime->tm_hour < ON_TIME) && show.lightsState())
			{
				std::cout << "Turning OFF: " << theTime->tm_hour << ":" << theTime->tm_min << std::endl;//DEBUG
				show.setState(0,"all");
                updateDB(sqlConn, 0, "all");
			}	
			//Turn on lights if we are in the ON time period, and the lights are off
			else if((theTime->tm_hour >= ON_TIME || theTime->tm_hour < OFF_TIME) && !show.lightsState())
			{
				std::cout << "Turning ON: " << theTime->tm_hour << ":" << theTime->tm_min << std::endl;//DEBUG
				show.setState(1,"all");
                updateDB(sqlConn, 1, "all");
			}
			
			unxTime = time(NULL);
			theTime = localtime(&unxTime);

			std::this_thread::sleep_for(std::chrono::milliseconds(50));

            //////////////////////SQL////////////////

            //Check the DB to update the channels        
           if(mysql_query(sqlConn, "SELECT * FROM channels"))
           {
                std::cout << "ERROR: sql query failed!" << std::endl;
                errorMsg = mysql_error(sqlConn);
                std::cout << errorMsg << std::endl;
                mysql_close(sqlConn);
                return 1;
           }

            sqlResult = mysql_store_result(sqlConn);

            while((sqlRow = mysql_fetch_row(sqlResult)) != NULL)
            {
                channel = sqlRow[0];
                for(int i = 1; i < mysql_num_fields(sqlResult); i++)
                {
                    state = sqlRow[i]; 
                    if(state == "on")
                    {
                        show.setState(1,channel);
                    }
                    else if(state == "off")
                    {
                        show.setState(0,channel);
                    }
                    else
                    {
                        std::cout << "ERROR: " << state << " invalid!" << std::endl;   
                        mysql_close(sqlConn);
                        return 1;
                    }
                }
            }
            mysql_free_result(sqlResult);

            //Check to make sure we havnt been told to turn off the 
            //entire light system
            if(isSysOn(sqlConn) == false)
            {
                show.setState(0,"all");
                updateDB(sqlConn, 0, "all");

                //wait until we have been told to turn the system back on
                while(!isSysOn(sqlConn))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                } 
            }
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
	
}


