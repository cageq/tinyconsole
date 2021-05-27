


#include "tiny_console.h"


int main(int argc, char * argv[]){

	TinyConsole tinyConsole; 


	tinyConsole.add_command("add item" , [](){

			printf("add command is \n"); 

			}); 


	tinyConsole.add_command("add npc" , [](){


			}); 


	tinyConsole.process(argc, argv); 



}; 
