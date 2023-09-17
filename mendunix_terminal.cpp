#include<iostream>
#include<unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include<vector>
#include<string>
#include<sstream>
#include <cstring>
#include <time.h>
#include <unordered_map>

char** getCommandArgs(std::string command_line);
void execute(char** args);

typedef struct 
{
    std::string name;
    std::string description; 
    std::string usage;
} Command;

static Command commands_description[] = {
    {
        "mendunix-help",
        "Command to help you to know all the things Mendunix-Terminal can do for you.",
        "Use help <command name> to know more about a specific command."},

    {
        "mendunix-exit",
        "Command to exit Mendunix-Terminal.",
        "Use exit to exit Mendunix-Terminal."
    }
};

std::unordered_map<std::string, Command> src = {
    {"mendunix-help", commands_description[0]}, 
    {"mendunix-exit", commands_description[1]}
};

int main(int argv, char* argc[])
{
    std::cout << "====== Welcome to Mendunix-Terminal v1.0 ======" << std::endl;
    std::cout << "* Documentation: https://github.com/ImNotMenduina/Mendunix-Terminal-OS *" << std::endl;   

    while(1)
    {
        std::string command_line;
        std::cout << "/> ";
        std::getline(std::cin, command_line);
        char** args = getCommandArgs(command_line);

        execute(args);
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
    pid_t id, w;
    int status;

    if(strcmp(args[0], "mendunix-exit") == 0)
    {   
        std::cout << "====== BYE BYE ======" << std::endl;
        exit(EXIT_SUCCESS);
    }
    else if(strcmp(args[0], "mendunix-help") == 0)
    {
        for(auto& p: src)
        {
            std::cout << "====== COMMAND //> " << p.second.name << " ======" << std::endl;
            std::cout << "Description: " << p.second.description << std::endl;
            std::cout << "Usage: " << p.second.usage << std::endl;
            std::cout << std::endl;
        }
    }
    else 
    {
        //Creates a new process. 
        //The new process (the child process) is an exact duplicate of the process that calls fork().
        id = fork();
        if(id < 0)
        {
            perror("Error creating process");
            exit(EXIT_FAILURE);
        }
        if(id == 0)
        {
            execvp(args[0], args);
            perror(args[0]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            waitpid(id, &status, WUNTRACED | WCONTINUED); 
            //Suspends the calling process until a child process ends or is stopped. 
            //More precisely, waitpid() suspends the calling process until the system gets status information on the child
        }
    }
}
