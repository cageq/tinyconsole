#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include <regex>

 using CommandHandler = std::function<void()>;

struct CmdNode
{

	std::string key;
	CmdNode(const std::string &k = "") ; 
	~CmdNode() ; 

	CmdNode *has_child(const std::string &k); 
	CmdNode *add_child(const std::string &k); 
	CommandHandler handler;

	std::vector<struct CmdNode *> children;
};

class CmdTree
{
public:
	void add_command(const std::string &cmd, CommandHandler handler); 	 

	std::string  get_completion(const std::string &input) ; 

	char * get_hint(const std::string &input); 

	bool compare_key(const std::string & input, const std::string & key ); 

	std::vector<std::string> split(const std::string &in, const std::string &delim = " ") ; 
	void execute(const std::string &cmd); 

	CmdNode root_node;
};
