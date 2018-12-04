## Welcome to TMBSH ##
=============================
## AUTHOR
Mudit Agarwal(20171090)

## INTRODUCTION
Simple shell implemented in C.
## Getting Started
To run the shell, write:
```
make
```
Followed by:
```
./TMBSH
```
## Features
- Comma separated commands can be given as input. For e.g. echo hello ; ls -al will produce hello then the contents of the current directory.
 - If background process exits then it displays the appropriate message to the user
 - Builtins:
	1. cd
	2. pinfo
	3. exit
	4. pwd
	5. ls
- Modularized code
  - Uses makefile for compilation process
  - All header files and declarations in header.h
- Proper error handling whenever required
- Uniform coding style (Variable names, brackets styling)


## Bonus
- Clock and Remind me
- Can give multiple directory in ls
- Vim & working
- Irrespective of starting location of shell the home will remain same.
- Run in Test Mode for easy debugging  

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
