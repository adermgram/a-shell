# ASH - Adam's Shell

Welcome to **ASH**, a simple Unix-like shell implemented in C. This project was created to understand the core mechanics behind how a shell works, including command parsing, process management, and executing built-in as well as external commands.

## Features

- Custom command prompt (`>`)
- Support for built-in commands:
  - `cd` – change directory
  - `help` – show help message
  - `exit` – exit the shell
- Execution of external programs using `fork()` and `execvp()`
- Input parsing using `strtok()`
- Dynamic memory allocation for input buffer and argument parsing

## Getting Started

### Prerequisites

- A C compiler (like `gcc`)
- A Unix-like environment (Linux or macOS)

### Compilation

To compile the shell, run:

```bash
gcc -o ash main.c
```
### Usage
- Once compiled, start the shell with:
```bash
./ash
```
- You’ll be greeted with a simple prompt:
```bash
>
```
  Now you can type any supported command or run external programs like ls, echo, mkdir, etc.
### Example
```bash
> help
Adam Idris's ASH
Type program names and arguments, and hit enter.
The following are built in:
  cd
  help
  exit
Use the man command for information on other programs.

> cd ..
> ls
> exit
```
## Structure

### `main.c`
The entire shell implementation, including:

- Input reading
- Parsing
- Built-in command handling
- Process launching

---

## Acknowledgments

This project was heavily inspired by and references the excellent tutorial by [Stephen Brennan](https://brennan.io/2015/01/16/write-a-shell-in-c/).  
Big thanks to him for making shell development accessible and well-documented.

---

## License

This project is open-source and available under the **MIT License**.

  
