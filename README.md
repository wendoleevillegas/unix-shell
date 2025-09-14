In my CSE3320 course, I implemented a command line interpreter (CLI) or, as it is more commonly known, a shell. The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.

Building the Shell <br>
You can build the code by typing: <br>
make

Testing the Shell <br>
You can run the provided tests by typing: <br>
./test-msh.sh

Basic Shell: msh <br>
Your basic shell, called msh is basically an interactive loop: it repeatedly prints a prompt msh>  (note the space after the greater-than sign), parses the input, executes the command specified on that line of input, and waits for the command to finish. This is repeated until the user types exit. The name of your final executable should be msh.

Interactive Mode <br>
The shell can be invoked with either no arguments or a single argument; anything else is an error. Here is the no-argument way:

prompt> ./msh <br>
msh> <br>
At this point, msh is running, and ready to accept commands. Type away! <br>

The mode above is called interactive mode, and allows the user to type commands directly.

Batch Mode <br>
The shell also supports a batch mode, which instead reads input from a file and executes commands from therein. Here is how you run the shell with a batch file named batch.txt:

prompt> ./msh batch.txt <br>
One difference between batch and interactive modes: in interactive mode, a prompt is printed (msh> ). In batch mode, no prompt should be printed.
<br> <br>
Structure <br>
Basic Shell <br>
The shell is very simple (conceptually): it runs in a while loop, repeatedly asking for input to tell it what command to execute. It then executes that command. The loop continues indefinitely, until the user types the built-in command exit, at which point it exits. That's it!

For reading lines of input, you should use getline() or fgets(). Generally, the shell will be run in interactive mode, where the user types a command (one at a time) and the shell acts on it. However, your shell will also support batch mode, in which the shell is given an input file of commands; in this case, the shell should not read user input (from stdin) but rather from this file to get the commands to execute.

In either mode, if you hit the end-of-file marker (EOF), you should call exit(0) and exit gracefully.

To parse the input line into constituent pieces, you might want to use strsep(). Read the man page (carefully) for more details.

To execute commands, look into fork(), exec(), and wait()/waitpid(). See the man pages for these functions, and also read the relevant book chapter for a brief overview.

You will note that there are a variety of commands in the exec family; for this project, you must use execv. You should not use the system() library function call to run a command. Any use of system will result in a grade of 0. Remember that if execv() is successful, it will not return; if it does return, there was an error (e.g.,the command does not exist). The most challenging part is getting the arguments correctly specified.

Paths <br>
Important: Note that the shell itself does not implement ls or other commands (except built-ins).

To check if a particular file exists in a directory and is executable, consider the access() system call. For example, when the user types ls, and path is set to include both /bin and /usr/bin, try access("/bin/ls", X_OK). If that fails, try "/usr/bin/ls". If that fails too, it is an error.

Your shell path should search four directories: /bin /usr/bin /usr/local/bin and ./

Built-in Commands <br>
Whenever your shell accepts a command, it should check whether the command is a built-in command or not. If it is, it should not be executed like other programs. Instead, your shell will invoke your implementation of the built-in command. For example, to implement the exit built-in command, you simply call exit(0); in your msh source code, which then will exit the shell.

In this project, you should implement exit, and cd as built-in commands.

exit: When the user types exit, your shell should simply call the exit system call with 0 as a parameter. It is an error to pass any arguments to exit.

cd: cd always take one argument (0 or >1 args should be signaled as an error). To change directories, use the chdir() system call with the argument supplied by the user; if chdir fails, that is also an error.

quit: When the user types quit, your shell should simply call the exit system call with 0 as a parameter. It is an error to pass any arguments to quit.

Redirection <br>
Many times, a shell user prefers to send the output of a program to a file rather than to the screen. Usually, a shell provides this nice feature with the > character. Formally this is named as redirection of standard output. To make your shell users happy, your shell should also include this feature, but with a slight twist (explained below).

For example, if a user types ls -la /tmp > output, nothing should be printed on the screen. Instead, the standard output of the ls program should be rerouted to the file output. In addition, the standard error output of the program should be rerouted to the file output (the twist is that this is a little different than standard redirection).

If the output file exists before you run your program, you should simple overwrite it (after truncating it).

The exact format of redirection is a command (and possibly some arguments) followed by the redirection symbol followed by a filename. Multiple redirection operators or multiple files to the right of the redirection sign are errors.


Additional Requirements <br>
After each command completes, your program shall print the msh> prompt and accept another line of input.
Your shell will exit with status zero if the command is “quit” or “exit”.
If the user types a blank line, your shell will, quietly and with no other output, print another prompt and accept a new line of input.
Your version of Mav shell shall support up to 10 command line parameters in addition to the command.
Your shell shall support and execute any command entered. Any of the 1006 commands in /bin, /usr/bin/, /usr/local/bin/ and the current working directory is to be considered valid for testing.
Mav shell shall be implemented using fork(), wait() and one of the exec family of functions. Your Mav shell shall not use system(). Use of system() will result in a grade of 0.
Your shell shall support the cd command to change directories. Your shell must handle cd ..
Tabs or spaces shall be used to indent the code. Your code must use one or the other. All indentation must be consistent.
No line of code or comment shall exceed 100 characters.
All code must be well commented. This means descriptive comments that tell the intent of the code, not just what the code is executing.
Keep your curly brace placement consistent. If you place curly braces on a new line , always place curly braces on a new end. Don’t mix end line brace placement with new line brace placement.
Remove all extraneous debug output before submission. The only output shall be the output of the commands entered, the error message, or the shell prompt.
