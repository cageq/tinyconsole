#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <functional>
#include <unordered_map>

using CommandHandler = std::function<void()>;

class TinyConsole
{
public:
    TinyConsole(const std::string &prompt = "=>"); 
    int process(int argc, char **argv);

    void add_command(const std::string &cmd, CommandHandler);    
private:
    std::unordered_map<std::string, CommandHandler> command_map;

	static std::string default_prompt ; 
};
