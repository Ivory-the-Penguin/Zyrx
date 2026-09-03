# The Zorax programming language
Zorax is a little hobby project I've started to work on. This is the specification for that language. It's designed to have all of the features than I liked from the multiple languages I tried, with a few of its own!

It has the .zrx extension, and the code is transpiled to C. The plan is to ONE DAY move to llvm, but C is better for C interop. The language in general is a Systems and Data Oriented language

Some languages that it's been inspired by:
* Zig
* Odin
* C/C++/C#
* Go

---

## Core idea
Some of the main goals of the engine is DX (Developer Experience) and clean code. The compiler will have some features that enforce clean code, and the language is designed to be well performant, and the C code should read like standard C code.

An example of simple Zorax code to C code

Zorax code
```
libc <- foreign {
  puts <- proc(string s) int
  sqrt <- func(float x) float
}

Vector <- struct {
  float x,
  float y,
}

Vector <- extension {
  print <- proc() {
    libc.puts("Vector has printed!");
  }

  length <- func() {
    return libc.sqrt(self.x * self.x + self.y * self.y)
  }
}
```

And the C code:
```c
int puts(const char *s);
double sqrt(double arg);

typedef struct {
  double x;
  double y;
} Vector;

void Vector_print() { puts("Vector has printed!"); }

double Vector_length(const Vector *self) {
  return sqrt(self->x * self->x + self->y * self->y);
}
```

This code has some syntax I haven't mentioned, but you should just have a look. This functionality is mostly available because of the way Zorax is designed. 

There are no high-level types, since it's a systems language. This means we don't need any messy high level code in here at all, at least for now. And if I do decide to add types later on, it will be meticulously designed to be idiomatic C code.

It's also Data Oriented, which means it works with mostly structs. That means C isn't trying to imitate an OOP language!

There's also seamless C interop, since it's compiled to C. All types in Zorax also only correspond to their 64-bit C counter part, this is subject to change.

---
## The plan
Currently I want to work towards a beta-beta version, which is a beta of the beta version. The beta-beta version has some of the language features, and only stack allocations, but it's extremely capable, and a good POF (Proof of concept)

You can look in the SPECIFICATION.md file to see the language specification