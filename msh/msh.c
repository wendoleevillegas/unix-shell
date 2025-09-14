// The MIT License (MIT)
// 
// Copyright (c) 2024 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>

#define WHITESPACE " \t\n" // used to split up the command line given by user
#define MAX_COMMAND_SIZE 255 // max command size
#define MAX_NUM_ARGUMENTS 10

FILE* fd = NULL;
char *command_string;

int main( int argc, char * argv[] )
{
  command_string = (char*)malloc(MAX_COMMAND_SIZE);
  //  check argc
  if (argc > 2)
  {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
  }

  // batch mode, checks arguments to see if the second is a file
  // if it's a file, it reads it, and does normal tokenization like interactive mode
  else if (argc == 2)
  {
    // where the actual file reading takes place
    char max[MAX_COMMAND_SIZE];
    fd = fopen(argv[1], "r");
    if(fd == NULL)
    {
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
    }

    while(fgets(max, sizeof(max), fd) != NULL)
    {
      char *token[MAX_NUM_ARGUMENTS];
      int token_count = 0;

      char *argument_pointer;

      char *working_string = strdup(max);
     //char *head_ptr = working_string;

      //where actual tokenization takes place
      while(((argument_pointer = strsep(&working_string, WHITESPACE)) != NULL) &&
      (token_count<MAX_NUM_ARGUMENTS))
      {
        token[token_count] = strndup(argument_pointer, MAX_COMMAND_SIZE);
        if(strlen(token[token_count]) == 0)
        {
          token[token_count] = NULL;
        }

        token_count++;
      }

      // command reading
      if(strcmp(token[0], "exit") == 0 || strcmp(token[0], "quit") == 0)
      {
        exit(0);
      }

      if(strcmp(token[0], "cd") == 0)
      {
        if(token[1] == NULL)
        {
          char error_message[30] = "An error has occurred\n";
          write(STDERR_FILENO, error_message, strlen(error_message));
        }

        else if(token[2] != NULL)
        {
          char error_message[30] = "An error has occurred\n";
          write(STDERR_FILENO, error_message, strlen(error_message));
        }

        else if(chdir(token[1]) != 0)
        {
          char error_message[30] = "An error has occurred\n";
          write(STDERR_FILENO, error_message, strlen(error_message));
        }
      }

      //free(head_ptr);
    }

    fclose(fd);
  }
  

  // interactive mode, reads from the command line
  while(1) // constantly keeps bash running
  {
 
    printf("msh> ");

    while(!fgets(command_string, MAX_COMMAND_SIZE, stdin)); // fgets reads user input into command_string

    //parse input
    char *token[MAX_NUM_ARGUMENTS];
    int token_count = 0;

    //pointer to point to the token?
    char *argument_pointer;

    // creating copies of the tokens to store in token[]
    char *working_string = strdup(command_string);

    char *head_ptr = working_string;

    //tokenizing input, using whitespace as delimeter
    while(((argument_pointer = strsep(&working_string, WHITESPACE)) != NULL) &&
    (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup(argument_pointer, MAX_COMMAND_SIZE);
      if(strlen(token[token_count]) == 0)
      {
        token[token_count] = NULL;
      }

      token_count++;
    }

    if(token[0] == NULL)
    {
      continue;
    }

    // exit
    if(strcmp(token[0], "exit") == 0 || strcmp(token[0], "quit") == 0)
    {
      exit(0);
    }

    // cd 
    if(strcmp(token[0], "cd") == 0)
    {
      if(token[1] == NULL)
      {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
      }

      else if(token[2] != NULL)
      {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
      }

      else if(chdir(token[1]) != 0)
      {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
      }
    }

    pid_t pid = fork();
    int status;

    if(pid == -1)
    {
      char error_message[30] = "An error has occurred\n";
      write(STDERR_FILENO, error_message, strlen(error_message));
    }

    else if(pid == 0) // child, where we do redirection in case user wants to throw output to a file
    {
      int i;
      for(i = 1; token[i] != NULL; i++)
      {
        if(strcmp(token[i], ">") == 0)
        {
          if(token[i+1] == NULL)
          {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
          }

          int fd = open(token[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

          if(fd < 0)
          {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
          }

          dup2(fd, 1);
          close(fd);

          token[i] = NULL;
        }
      }

      execvp(token[0], token);
      exit(EXIT_SUCCESS);
    }

    else // parent
    {
      waitpid(pid, &status, 0);
    }

    // loop continues until all tokens are passed OR MAX_COMMAND_SIZE is reached
    free(head_ptr);
  }

  return 0;
}

