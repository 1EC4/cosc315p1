// closh.c - COSC 315, Winter 2020
// #47462361 Anthony Boyko
// #22092845 Nick Rieske
// #29164977 Jonathan Gresl

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

// Tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
    cmd[strlen(cmd) - 1] = '\0';        // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// Read one character of input, then discard up to the newline - do not modify
char readChar() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// Main method - program entry point
int main() {
    char cmd[81];           // Array of chars (a string)
    char* cmdTokens[20];    // Array of strings
    int count;              // Number of times to execute command
    int parallel;           // Whether to run in parallel or sequentially
    int timeout;            // Max seconds to run set of commands (parallel) or each command (sequentially)
    
    // Main shell input loop - Return to the prompt once all processes have either completed or timed out
    while (TRUE) {          
        
        /*** Parse input from user ***/          
        
            // Prompt user for the name of the program to run
            printf("closh> ");
            fgets(cmd, sizeof(cmd), stdin);
            if (cmd[0] == '\n') continue;
            readCmdTokens(cmd, cmdTokens);

            // Prompt user for the number of copies of the program to run
            do {
                printf("  count> ");
                count = readChar() - '0';
            } while (count <= 0 || count > 9);
            
            // Prompt user whethere a process should execute concurrently or sequentially
            printf("  [p]arallel or [s]equential> ");
            parallel = (readChar() == 'p') ? TRUE : FALSE;
            do {
                printf("  timeout> ");
                timeout = readChar() - '0';
            } while (timeout < 0 || timeout > 9);

        /*** Execute processes concurrently ***/

        if (parallel) {
            // Create an array of PIDs to keep track of each child
            pid_t child_pid[count];

            // Create each of the children and assign its PID to the array
            for(int i = 0; i < count; i++){
                if ((child_pid[i] = fork()) == 0){
                    printf("Process child created. PID = %d\n", getpid());

                    // Start the specified program or print an error and exit
                    execvp(cmdTokens[0], cmdTokens);
                    printf("Can't execute %s\n", cmdTokens[0]);
                    exit(1);
                }
            }

            // Variable to keep track of Timer PID
            pid_t timer_pid;

            // Create the timer process
            if(timeout > 0 && (timer_pid = fork()) == 0){
                printf("Timer child created.   PID = %d\n", getpid());
                sleep(timeout);
                exit(0);
            } else {
                timer_pid = -1;
            }

            // Variable to keep track of each process as it exits
            pid_t finished_process;

            // Counter to keep track of how many children exit before the timer does
            int finished_children = 0;

            // The loop doesn't terminate until either:
            //   - the number of children that have exited is equal to the number of children created (ie: all children have finished)
            //   - the timer finishes (so the inside of the while loop only executes if a child exits)
            while(finished_children < count && (finished_process = wait(NULL)) != timer_pid){
                finished_children++;
                // Remove the child's PID from the PID array
                for(int i = 0; i < count; i++){
                    if (child_pid[i] == finished_process){
                        child_pid[i] = -1;
                    }
                }
            }

            if (finished_children == count && timeout > 0){
                // Since all the children finished executing, the timer is killed
                kill(timer_pid, SIGKILL);
                wait(NULL);
            } else if (finished_children < count){
                // The timer finished before all the children did, so an error is printed
                printf("%s exceeded the specified timeout\n", cmdTokens[0]);
                // and all children that are still running are killed
                for(int i = 0; i < count; i++){
                    if(child_pid[i] > 0){ 
                        // Since we set the PID of the children that exited to -1,
                        // this will only execute for children that haven't exited yet.
                        kill(child_pid[i], SIGKILL);
                        wait(NULL);
                    }
                }
            }

        /*** Execute processes sequentially ***/

        } else {
            // Variables to keep track of 2 child processes
            pid_t child_pid, timer_pid;

            // Iterates count number of times
            while ( count > 0 ) {

                // Creates a child process and stores its PID in child_pid
                // The child executes the code inside of the if block
                if ((child_pid = fork()) == 0){
                    printf("Process child created. PID = %d\n", getpid());
                    // Tries replacing itself with the user specified program
                    execvp(cmdTokens[0], cmdTokens);
                    // If the above fails, the child prints an error and exits
                    printf("Can't execute %s\n", cmdTokens[0]);
                    exit(1);
                }

                // Creates a child to keep track of the timeout
                if (timeout > 0 && (timer_pid = fork()) == 0){
                    printf("Timer child created.   PID = %d\n", getpid());
                    sleep(timeout);
                    exit(0);
                } else {
                    timer_pid = -1;
                }

                // Parent program waits for either child to finish
                pid_t finished_process = wait(NULL);
                if (finished_process == child_pid && timeout > 0){
                    // If the child executing the user command finishes first,
                    // the timer process is killed. This is the intended outcome.
                    kill(timer_pid, SIGKILL);
                    // Waiting for the second process to exit
                    wait(NULL);
                } else if (finished_process == timer_pid){
                    // If the timer finishes before the user command child, we
                    // kill that child as it has exceeded the timeout.
                    kill(child_pid, SIGKILL);
                    // After killing the child, the parent prints an error message.
                    printf("%s exceeded the specified timeout\n", cmdTokens[0]);
                    // Waiting for the second process to exit
                    wait(NULL);
                }

                // Reducing counter
                count--;
            }
        }     
    }
}
