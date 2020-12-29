# CS 360
## Zach Nett, Fall 2020

Systems Programming with KC Wang at Washington State University. Course topics include Linux/Unix operating systems, I/O operations, processes, concurrent programming, networking, and file systems.

Please do not plagiarize or use this code dishonestly.

## Project Explanations
The file structure of this repository is as follows:
```
.
├── final
├── lab1
├── lab1-prelab
├── lab2
├── lab3
├── lab4
├── lab5
└── lab6
```
Each directory corresponds to a major assignment in the class as detailed below.

### Lab 1 Pre-Lab
A small pre-lab project covering the execution image of a C program and the content of a binary executable, as well as runtime stack contents (stack frame linked list, function parameters, local variables and etc. in stack memory).

### Lab 1
Replicates `fdisk p` output of a virtual disk with regards to hard disk partition sectors. Reads Master Boot Record of virtual disk and prints out relative information. Part 2 includes implementation of printf() using variable arguments.

### Lab 2
Implementation of a basic Linux file tree. Includes basic command implementation for file tree navigation. Repository also includes various examples of file trees that can be loaded using the provided code.

### Lab 3
A shell simulator (znsh) project which manipulates the Linux environment PATH variable to execute Linux command line programs. Include process forking and code for handling pipes and I/O redirection. Personal extension (for fun!) included using aliases to enable terminal color.

(Possible extension of this project could be to use `.bashrc` to process aliases rather than hard-coding them within the program).

### Lab 4
A multi-threaded program to solve a system of linear equations by LU-decomposition. Utilizes pthreads, hashing, and barriers for concurrent programming.

### Lab 5
Basic networking between a client and server with bytestream data communication and client and server-specific Linux commands.

### Lab 6
The foundations of a EXT2 filesystem (see Final Project): specifically mount_root, pwd, cd, and ls. Note the `lab6` directory contains most likely incomplete or unmodified code, as Lab 6 was foundational for the final project (and thus the final project was constructed on top of the working Lab 6 code).

### Final Project: EXT2 Filesystem
Implementation of the Extended 2 (EXT2) Filesystem in C. The project was broken down into three levels with corresponding related commands: all levels were completed except for the modification of pwd for mounting (part of level 3), which was not completed due to time constraints.

## Credits
Most code in this repository is mine, although Lab 5, Lab 6 and the final project were done collaboratively with Connor Easton ([Eastonco](https://github.com/Eastonco)). If applicable, any base code and example code is from Professor KC Wang.