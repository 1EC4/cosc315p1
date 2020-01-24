// closh.c - COSC 315, Winter 2020
// #47462361 Anthony Boyko

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

// tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// main method - program entry point
int main() {
    char cmd[81]; // array of chars (a string)
    char* cmdTokens[20]; // array of strings
    int count; // number of times to execute command
    int parallel; // whether to run in parallel or sequentially
    int timeout; // max seconds to run set of commands (parallel) or each command (sequentially)
    
    while (TRUE) { // main shell input loop
        
        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n') continue;
        readCmdTokens(cmd, cmdTokens);
        do {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);
        
        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);
        // end parsing code
        
        
        ////////////////////////////////////////////////////////
        //                                                    //
        // TODO: use cmdTokens, count, parallel, and timeout  //
        // to implement the rest of closh                     //
        //                                                    //
        // /////////////////////////////////////////////////////

	//using parallel 
	if (parallel) {
	
	    //creates a new process while count is non negative
	    while ( count > 0 ) {
	      //count--; //has to be at the start otherwise the subract won't be reached by execvp
	      //exec(); //create a new process
	      if(fork() == 0){
		    execvp(cmdTokens[0], cmdTokens);
	      } else {
		    count--;
	      }
	       
	    }

    } else {
        // Sequential Execution
        // Variables to keep track of 2 child processes
        pid_t child_pid, timer_pid;
        // Iterates count number of times
        while ( count > 0 ) {

            // Creates a child process and stores its PID in child_pid
            // The child executes the code inside of the if block
            if ((child_pid = fork()) == 0){
                // Tries replacing itself with the user specified program
                execvp(cmdTokens[0], cmdTokens);
                // If the above fails, the child prints an error and exits
                printf("Can't execute %s\n", cmdTokens[0]);
                exit(1);
            }

            // Creates a child to keep track of the timeout
            if ((timer_pid = fork()) == 0){
                sleep(timeout);
                exit(0);
            }

            // Parent program waits for either child to finish
            pid_t finished_process = wait(NULL);
            if (finished_process == child_pid){
                // If the child executing the user command finishes first,
                // the timer process is killed. This is the intended outcome.
                kill(timer_pid, SIGKILL);
            } else {
                // If the timer finishes before the user command child, we
                // kill that child as it has exceeded the timeout.
                kill(child_pid, SIGKILL);
                // After killing the child, the parent prints an error message.
                printf("%s exceeded timeout\n", cmdTokens[0]);
            }
            // Waiting for the second process to exit
            wait(NULL);

            // Reducing counter
            count--;
        }
    }

	//note for parallel try using fork if the parent process and use exec for children

	//execvp(cmdTokens[0], cmdTokens); // replaces the current process with the given program
	// doesn't return unless the calling failed
        //printf("Can't execute %s\n", cmdTokens[0]); // only reached if running the program failed
        //exit(1);        
    }
}

