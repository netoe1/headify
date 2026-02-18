# headify
CLI Tool that generates .h, only with .c files.


# Introduction and Motivation
I'm a guy that likes C/C++, but usually, I don't like do makefiles and this kind of stuff. I Think it's boring.
So, to fix these problems, I've invented headify.
Basically, everything you need to do, at this point, is use keywords to generates .h file, while you programming .c file.
After, just use CLI to generate .h.

# Keywords:
It's easier to implement keywords instead of parser; parser is a complex project, and for this propouse, doesn't have to be complex or complete to use, not at this point.
Keywords are used to things that you want to show at .h file, here are the tables.

| Keyword      | Type            | Description                                                | Behavior in `.h`                        | Behavior in `.c`                            |
|--------------|-----------------|------------------------------------------------------------|----------------------------------------|---------------------------------------------|
| `@var`  | Global variable | Marks a global variable to be exported to the header file | Adds the variable declaration          | Entire line removed from `.c`               |
| `@function`  | Function        | Marks a function to automatically generate a prototype    | Generates function prototype (`;`)     | Removes only `@function`, keeps the code    |
| `@comment`   | Comment         | Comment that should appear in the header file             | Copies comment to `.h`                 | Entire line removed from `.c`               |
| `@struct`    | Struct          | Marks a struct definition for export                      | Copies full struct to `.h`             | Entire struct removed from `.c`             |
              

# Keywords-usage
- **@var**: To variables in general. If you want to show at .h, make sure that **@var** is before the variable declaration.

```c
// At .c original that you wrote file.
@var int a = 0;
```

```c
// At .h file
// Expected behavior for .h:
#ifndef FILE_H
#define FILE_H
int a = 0;
#endif
```


```c
// At .c generated file
// Expected behavior for .c generated:
#include "file.h"
// You put int a = 0 at .h file. It was deleted from .c.
```
---

- **@function**: Only shows the prototip for function in .h, in .c, implements.


```c
// .c original file.

#include <stdio.h>
@function void helloWorld(){
    printf("Welcome to the jungle, we got fun in games!\n");
}
```

```c
// .h generated file.

#ifndef FILE_H
#define FILE_H

void helloWorld();
#endif
```

```c
// .c generated file.

#include <stdio.h>
#include "file.h"

void helloWorld(){
    printf("Welcome to the jungle, we got fun in games!\n");
}
```

---

- **@comment**: A commentary if you want to put in .h file.

```c
// .c original file.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

@comment // This is a function that prints random numbers
@function void get_random(int limit){
    srand(time(NULL));
    return rand() % limit;
}
```

```c
// .h generated file.
#ifndef FILE_H
#define FILE_H

// This is a function that prints random numbers
void get_random(int limit);

#endif
```

```c
// .c generated file.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file.h"


void get_random(int limit){
    srand(time(NULL));
    return rand() % limit;
}
```

---

- **@struct**: struct to put on .h file.

```c
// .c original file.

@struct struct Person{
    char name[64];
    int age;
};

```

```c
// .h generated file.
#ifndef FILE_H
#define FILE_H

struct Person{
    char name[64];
    int age;
};
#endif
```

```c
// .c generated file.
#include "file.h"
// Struct was removed.
```



# Headify CLI Options

The Headify CLI provides commands and options to generate header files automatically from annotated C source files.

---

## Usage

```bash
headify [options] <command> [arguments]
```

## Commands

`generate`
Generates a header file from a C source file using parser keywords.

```bash
headify generate <input.c> <output.h>
```

**Description**

- Parses the input `.c` file.

- Detects special keywords (`@var`, `@function`, `@comment`, `@struct`).

- Generates a header file containing extracted declarations.

- Optionally cleans the original `.c` file depending on parser configuration. (will be implemented)


|Argument | Description |
| --------|-------------|
| `input.c`|  Source C file to be parsed
| `output.h`| Header file to be generated

**Options**

`-h` or `--help`

Display Usage instructions and available commands.
```bash
headify --help or headify -h
```

`--LICENSE`
Displays the software license information.

```bash
headify --LICENSE
```



# Final Considerations:
- Keywords must appear at the beginning of the line.

- The parser reads the file sequentially (single-pass).

- Generated header guards are based on the source filename.

- The tool may generate cleaned versions of source files depending on configuration.

# To fork:

Just fork, but don't broke the LICENSE requisitions.
Remember to put my name as the creator and the autors (people that work on project), in LICENSE!