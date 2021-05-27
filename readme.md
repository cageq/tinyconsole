

tiny embed console lib for fun . 


simple to use , with basic auto complete . 



```cpp 


#include "tiny_console.h"

int main(int argc, char * argv[]){

	TinyConsole tinyConsole; 

	tinyConsole.add_command("add item" , [](){

			printf("execute add item command  \n"); 

			}); 

	tinyConsole.add_command("add npc" , [](){

			printf("execute add npc command  \n"); 

			}); 

	tinyConsole.process(argc, argv); 

}; 

```
