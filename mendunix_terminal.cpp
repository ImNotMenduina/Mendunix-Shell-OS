#include<iostream>
#include<unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include<vector>
#include<string>
#include<sstream>
#include <cstring>
#include <time.h>

char** getCommandArgs(std::string command_line);
void execute(char** args);

int main()
{
    std::cout << "***************** Welcome to Mendunix-Terminal v1.0 *****************" << std::endl;
    std::cout << "Documentation: https://github.com/ImNotMenduina/Mendunix-Terminal-OS" << std::endl;   

    while(1)
    {
        std::string command_line;
        std::getline(std::cin, command_line);
        char** args = getCommandArgs(command_line);

        execute(args);
        //execvp(args[0], args);
    }

    return 0;
}

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

void execute(char** args)
{
    pid_t id;
    int status;

    if(strcmp(args[0], "q") == 0)
        exit(0);

    if((id = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    if(id == 0)
    {
        execvp(args[0], args);
        perror(args[0]);
        exit(1);
    }
    
    //work in progress....
    //
    //
    while(wait(&status) != id)
        printf("\n");
}