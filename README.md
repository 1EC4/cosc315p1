# COSC 315: Operating Systems - Project 1

### Table of Contents
- [Description](#description)
- [Core Features](#core-features)
- [Design Choices](#design-choices)
- [Sample Output](#sample-output)
- [Contributors](#contributors)

## Description
A simple clone shell in C which uses system calls directly to implement a command-line shell that can run multiple copies of a program at once.

## Core Features
**Takes as input and executes the program accordingly:**
- [x] The name of the program to run
- [x] The number of copies (processes) of the program to run (1 to 9)
- [x] Whether a process should execute concurrently or sequentially (p or s)
- [x] A timeout specifying the maximum duration of each process in seconds (0 to 9)

## Design Choices
###### When executing the processes concurrently
  1. Creates an array with the size equal to the number of copies chosen by the user
  2. In a for loop, uses **fork()** to create a child and assign a PID to each index in the array
      - Notifies user of each child created along with their respective PID numbers
      - Uses **execvp()** to start each of the copied programs
  3. Uses **fork()** to create a timer child that keeps track of the timeout
      - Notifies user of the timer child created along with its PID number
  4. ses **wait(NULL)** to finish process child uses **kill()** to stop it once finished
  5. Stores in local variable the most recently finished process and keeps count of them
  6. Uses a while loop to execute the processes until the all children are finished

###### When executing the processes sequentially
  1. Uses **fork()** to create a process child that keeps track of the timeout
  2. Notifies user of the process child created along with its PID number
  3. Uses **fork()** to create a timer child that keeps track of the timeout
  4. Notifies user of the timer child created along with its PID number
  5. Uses **wait(NULL)** to finish process child uses **kill()** to stop it once finished
      - Checks timer and uses **kill()** to stop it if the timeout has been reached
  6. Waits for the second process to finish and decrements the counter
  7. Uses a while loop to repeat the algorithm as many times as chosen by the user

## Sample Output
Check out [this example file](sample_output.txt) for sample output.

## Contributors
- **Anthony Boyko**
  - Scheduled team meetings and reserved study space
  - Identified and communicated necessary features to be included in the program
  - Planned the project and assigned tasks to team members
  - Based out framework for the required program functions to write
  - Implemented initial fork join operations
  - Modified the code to ignore the timeout feature when the user chooses a timeout of zero
- **Nick Rieske**
  - Wrote function to concurrently execute the amount of processes chosen by the user
  - Wrote an alternate function to sequentially execute the amount of processes chosen by the user
  - Implemented timer to exit the program when the timeout chosen by the user is reached
  - Created the logic to keep track of processes to execute and ones that are finished
  - Clearly commented the code to explain how the program is intended to work
  - Reviewed README file and adjusted as the design choices as needed
- **Jonathan Gresl**
  - Managed deliverables according to the assignment guidelines
  - In-depth testing of program to verify all requirements are met
  - Modified code to specify child type and associated PID number
  - Minor code changes to improve readability and finalize required functionality
  - Created the README file to include description, core features, design choices, sample output, and contributions
  - Copied examples from the program output into a separate file and linked to the README - _sample_output.txt_

[Back to The Top](#cosc-315-operating-systems---project-1)
