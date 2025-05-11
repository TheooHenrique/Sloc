# Introduction

This project was constructed to analyze code files in c/c++ language and resume some informations to the user: 
- The number of total lines of the code
- The quantity and percentage of code lines
- The quantity and percentage of comment lines
- The quantity and percentage of doxygen comment lines
- The quantity and percentage of blank lines

This project was made to verify more than one file per run, and it also has the option of verify all c/c++ files of a directory, recursive or not.

The main reason of doing this project is to verify the quality of code of our next projects, searching for decrease the quantity of code lines to do something. It is also important understand the reading process of a compiler, something programmers use a lot, but not always knows exactly how it works.

# Author(s)

### Theo Henrique da Silva Borges: <theo123b@gmail.com>
### Haniel Lucas Machado: <hani.lucas70@gmail.com>


# Problems found or limitations

## Problems found
Here we are listing some problems we have found doind the project

### Work With States
Work with states was certainly the worst part of the project, not just because statement is something new, but also because a StateMachine is something with a very difficult logic in this case. Thats because of the way this program works, there are very tiny details that if the programmer do not pay attention, he will make mistakes, like:

```c++
#include <iostream>

/**
the function says hello to the world
*/ int main(){ ///Says Hello to the World
    std::cout << "//Hello World!";
}
```
Note that the `*/` determine the end of the doc-comment block, so this line will count as a line of doc-comment line. But also, this line has a code after the `*/`, so it will be also count as a code line. That as such a hard do implement because i dit the function recursively.

### First Time Using Classes
That was not so hard comparing to statement, but it was a bit difficult to understand where my input was going. Hopefully, this knowledge will bring us some smoothness in next projects.

## Limitations
Here we are listing some things that our project can't do.

### Work With Compiling Error Codes
In this project, the analysis of codes with compiling error are no 100% accuratly, but it works.

# Grading

| Item                                                | Valor máximo | Valor esperado |
| --------------------------------------------------- | :----------: | :------------: |
| Read one or multiple source file                    |      10      |        10      |
| Read a list of files from a directory               |      10      |        10      |
| Read a list of files recursively from a directory   |      8       |         8      |
| Show help as requested `-h`                         |      4       |         4      |
| Treat error situations                              |      10      |        10      |
| Show correct number of blank lines                  |      5       |         5      |
| Show correct number of comment lines                |      10      |        10      |
| Show correct number of doc comment lines            |      10      |        10      |
| Show correct number of code lines                   |      10      |        10      |
| Show correct number of total lines                  |      5       |         5      |
| Show the type of source file                        |      5       |                |
| Show percentages as requested                       |      10      |                |
| Show consolidation at the bottom of a table         |      5       |                |
| Table is formatted as requested                     |      8       |                |
| The project contains at least one struct or classes |      5       |         5      |
| Overall evaluation of the program                   |      5       |                |

# Compiling and Running

To compile this code, we are using `g++` compiler. To compile, use this code in root directory of the project:

```shell
g++ ./src/main.cpp -o sloc
```
To run the `sloc` executable created, run this code

```shell
./sloc dir file.cpp
```
Where:

```./sloc: Call the executable created```

```dir: The directory you want to analyse the c/c++ files inside```

```file: The c/c++ files you want to analyse```

Parameters: 
- `-h` to show help
- `-s` to sort it ascending
- `-S` to sort it descending

The `-s` and `-S` parameters are:
- `f` to sort by filename
- `t` to sort by filetype
- `c` to sort by comments
- `d` to sort by doc comments
- `b` to sort by blank lines
- `s` to sort by line of codes
- `a` to sort by all