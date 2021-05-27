#include <string.h>
#include "linenoise.h"
#include "tiny_console.h"
#include "cmd_tree.h"
static CmdTree kCmdTree;

std::string TinyConsole::default_prompt = "=>"; 
TinyConsole::TinyConsole(const std::string & prompt )
{
	
	default_prompt  = prompt; 
	add_command("exit", []() {
			exit(0);
			});
}

void TinyConsole::add_command(const std::string &cmd, CommandHandler handler)
{

	command_map[cmd] = handler;
	kCmdTree.add_command(cmd, handler);
}

static void terminal_completion(const char *buf, linenoiseCompletions *lc)
{

	std::string completion = kCmdTree.get_completion(buf);
	if (!completion.empty())
	{
		linenoiseAddCompletion(lc, completion.c_str());
	}
}

static char *terminal_hints(const char *buf, int *color, int *bold)
{
	// if (!strcasecmp(buf,"hello")) {
	//     *color = 35;
	//     *bold = 0;
	//     return " World";
	// }
	auto hints = kCmdTree.get_hint(buf);
	if (hints != nullptr)
	{

		*color = 35;
		*bold = 0;
		return hints;
	}

	return nullptr;
}

int TinyConsole::process(int argc, char **argv)
{
	char *line;
	char *progName = argv[0];

	/* Parse options, with --multiline we enable multi line editing. */
	while (argc > 1)
	{
		argc--;
		argv++;
		if (!strcmp(*argv, "--multiline"))
		{
			linenoiseSetMultiLine(1);
			printf("Multi-line mode enabled.\n");
		}
		else if (!strcmp(*argv, "--keycodes"))
		{
			linenoisePrintKeyCodes();
			exit(0);
		}
		else
		{
			fprintf(stderr, "Usage: %s [--multiline] [--keycodes]\n", progName);
			exit(1);
		}
	}

	/* Set the completion callback. This will be called every time the
	 * user uses the <tab> key. */
	linenoiseSetCompletionCallback(&terminal_completion);
	linenoiseSetHintsCallback(&terminal_hints);

	/* Load history from file. The history file is just a plain text file
	 * where entries are separated by newlines. */
	linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

	/* Now this is the main loop of the typical linenoise-based application.
	 * The call to linenoise() will block as long as the user types something
	 * and presses enter.
	 *
	 * The typed string is returned as a malloc() allocated string by
	 * linenoise, so the user needs to free() it. */

	while ((line = linenoise(default_prompt.c_str() )) != NULL)
	{
		/* Do something with the string. */
		if (line[0] != '\0' && line[0] != '/')
		{
			// printf("execute command : '%s'\n", line);

			// auto itr = command_map.find(line);
			// if (itr != command_map.end()){
			//     printf("found command \n");
			//     itr->second();
			// } else {

			// }
			kCmdTree.execute(line);

			linenoiseHistoryAdd(line);           /* Add to the history. */
			linenoiseHistorySave("history.txt"); /* Save the history on disk. */
		}
		else if (!strncmp(line, "/historylen", 11))
		{
			/* The "/historylen" command will change the history len. */
			int len = atoi(line + 11);
			linenoiseHistorySetMaxLen(len);
		}
		else if (!strncmp(line, "/mask", 5))
		{
			linenoiseMaskModeEnable();
		}
		else if (!strncmp(line, "/unmask", 7))
		{
			linenoiseMaskModeDisable();
		}
		else if (line[0] == '/')
		{
			printf("Unreconized command: %s\n", line);
		}

		free(line);
	}
	return 0;
}
