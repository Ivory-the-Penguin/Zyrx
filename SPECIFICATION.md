# Zyrx specification
The specification for the Zyrx programming language, and serves as a sort of tutorial as well. 

***For major version*** `BetaBeta`

## BetaBeta Limitations
Here is a list of some limitations for BetaBeta:
* Stack only
* Not full planned feature set (Like a macro system)
* No standard library
* Semicolon is mandatory

## File Type
All Zyrx applications will be in a single file, with the extension of `.zrx`. The transpiled C code will also be in one file. By default it compiles straight to an executable, but you have an option to keep the C file with the `--keep-c` flag.

## Basic program
Heres a simple bare bones programming.
```
main <- proc() int {
  return 0;
}
```

The `main` is the name of the procedure, while the `<-` is a const declaration. You use it for mostly declaring the following:
* Constants
* Functions
* Procedures
* Structs
* Enums
* Imports

In this case, it's used to declare a **procedure**, the difference between the two will be specified later. 

You would put the parameters inside the parentheses as you would with most languages. It's followed by the return type (if there is none, that means its a void function).

You wrap code inside functions and procedures using curly braces (similar to C and multiple other languages).

the `return` keyword is for returning values from a function or a procedure, followed by the value. 

You use semicolons to end a statement (**likely to change**). 

## Normal operators
This is the same as it is with all languages:
* Addition: `10 + 5`
* Subtraction: `10 - 5`
* Multiplication: `10 * 5`
* Division: `10 / 5`
* Mod: `10 % 5`

The language respects the order of operations, so `5 + 5 * 10` would be 55, and not 100.

The order of operations goes as following:
* Parentheses
* Mod
* Multiplication and Division
* Addition and Subtraction

If they are in the same tier, it's determined by the order instead, so `10 * 5 / 2` would be 25. You can also change the order of operations with parentheses

## Functions and Procedures
Unlike in other languages, Zyrx strictly distinguishes procedures (`proc` keyword) and functions (`func` keyword).

Procedures have the following rules:
* You can use pointers
* You can call other procedures and functions
While functions have the exact opposite:
* You aren't allowed to use pointers
* You can only call functions
* You *can* use mutable variables inside the function
* Must have a return type

# TODO: FINISH SPEC