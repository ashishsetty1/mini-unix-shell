# Mini Unix Shell (C)

A Unix-like shell implemented in C that supports command execution, background processes, piping, and command history.

---

## 🚀 Features

- Execute system commands (`ls`, `pwd`, `date`)
- Built-in commands: `cd`, `exit`, `history`
- Background processes using `&`
- Piping with `|`
- Command history tracking

---

## 🧠 How It Works

This shell uses core Unix system calls:

- `fork()` → create child processes  
- `execvp()` → execute commands  
- `wait()` / `waitpid()` → manage processes  
- `pipe()` + `dup2()` → enable piping between commands  

---

## 🛠️ Build & Run

```bash
make
./mysh