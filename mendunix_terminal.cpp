#include<iostream>
#include<unistd.h>
#include<vector>
#include<string>
#include<sstream>
#include <cstring>

char** getCommandArgs(std::string command_line)
{   
    int ARGS_COUNTER = 0;
    std::vector<std::string> args_vector;

    std::stringstream ss; 
    ss.str(command_line);

    //I'll break the command here, separating its arguments
    std::string arg;
    while(std::getline(ss, arg, ' '))
    {
        //I'm using args_vector to store each argument.
        args_vector.push_back(arg);
        ARGS_COUNTER++;
    }   

    //To use execvp() we need to pass an const char* args
    //It contains the arguments and the final position is NULL
    char** arr = new char*[++ARGS_COUNTER];

    for(int i=0; i<ARGS_COUNTER - 1; i++)
    {
        arr[i] = new char[args_vector[i].length() + 1];
        strcpy(arr[i], args_vector[i].c_str());
    }

    arr[ARGS_COUNTER - 1] = NULL;

    return &arr[0]; // return the first addr 
}

int main()
{
    //getCommandsAvaiables();

    //getCommandsArgs()

    std::string command_line;
    std::getline(std::cin, command_line);

    char** args = getCommandArgs(command_line);

    execvp("ls", args);

    return 0;
}

