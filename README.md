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
**Takes as input:**
- [x] The name of the program to run
- [x] The number of copies (processes) of the program to run (1 to 9)
- [x] Whether a process should execute concurrently or sequentially (p or s)
- [x] A timeout specifying the maximum duration of each process in seconds (0 to 9)

## Design Choices
###### When running the processes concurrently
  1. Create an array with the size equal to the number of copies chosen by the user
  2. In a loop, use **fork()** to create a child and assign a PID to each index in the array
      - Notify user of each child created along with their respective PID numbers
      - Use **execvp()** to start each of the copied programs
  3.
  4.
  5.
  6.
  7.
  8.
  9.

###### When running the processes sequentially
  1.
  2.
  3.
  4.
  5.
  6.
  7.

## Sample Output
Check out [this example file](examples/output.jpg) for sample output.

## Contributors
- **Anthony Boyko**
  - Scheduled team meetings and reserved study space
  - Identified and communicated necessary features to be included in the program
  - Planned the project and assigned tasks to team members
  - Implemented initial fork join operations
- **Nick Rieske**
  - Implemented...
  -  
  - 
  - 
  - 
  - 
- **Jonathan Gresl**
  - Managed deliverables according to the assignment guidelines
  - In-depth testing of program to verify all requirements are met
  - Minor code changes to improve readability and finalize required functionality
  - Created README file to include description, core features, design choices, sample output, and contributions

[Back to The Top](#cosc-315-operating-systems---project-1)
