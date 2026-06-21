# minishell

A POSIX-compatible shell implemented from scratch in C with tokenizer, parser, execution engine, custom libft... 
Created by mokutucu and afoth as a 42 school project.

## Overview

Most shell projects stop at parsing. This one goes further: a full five-stage pipeline
(lex → syntax-check → expand → parse → execute) built entirely in C with no helper
frameworks. The goal was to understand what a real shell does under the hood — process
management, signal propagation, file descriptor wiring, and memory ownership — by
implementing all of it by hand.

## Highlights

- **Custom garbage collector** — all heap allocations go through `ft_gc_malloc`, which
  registers each pointer in a linked list. One call to `ft_gc_free` at teardown cleans
  everything, preventing leaks without manual tracking across the call graph.
- **Multi-stage pipeline architecture** — input flows through six numbered source
  directories (`0.garbage_collector` → `1.lexer` → `1.syntax_check` → `2.expansion` →
  `3.cmd_parser` → `5.execution`), each with a clean interface. The tokenizer produces a
  doubly-linked `t_arg` list; the command parser consumes it and produces a `t_command`
  linked list for the executor.
- **Heredoc with variable expansion** — `<<` forks a child process that reads via
  `readline("heredoc>")`, expands `$VAR` / `$?` inline, and writes into a pipe whose
  read-end becomes the next command's stdin.
- **Context-aware signal handling** — SIGINT and SIGQUIT behave differently depending on
  whether the shell is in interactive mode, running a heredoc, or executing a child process.
  A single global `g_sig` int propagates signal state across fork boundaries.
- **Full pipe support** — `execute_commands_with_pipes` allocates an array of `2 * n`
  file descriptors, forks one child per command, wires them with `dup2`, and waits only
  on the last child for the exit status (reaping the rest with `wait(NULL)`).
- **All required builtins** — `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`,
  plus `||` operator support.

## Tech stack

| Layer | Technology |
|---|---|
| Language | C (C99, `-Wall -Wextra -Werror`) |
| Input / history | GNU Readline |
| String utilities | Custom libft (bundled in `libs/libft/`) |
| System APIs | POSIX — `fork`, `execve`, `pipe`, `dup2`, `waitpid`, `signal` |
| Build | GNU Make |

## Getting started

**Prerequisites**

- `gcc` or `cc`
- GNU Readline (`libreadline-dev` on Debian/Ubuntu, `readline` via Homebrew on macOS)

**Build**

```sh
make
```

This compiles `libs/libft` first, then links the main binary.

**Run**

```sh
./minishell
```

**Uninstall**

```sh
make fclean
```

## Architecture

```
stdin
  │
  ▼
readline  ──►  expand_string ($VAR, $?)
                    │
                    ▼
               tokenizer  →  t_arg doubly-linked list
                    │
                    ▼
             syntax_checker  (pipe / redir / || rules)
                    │
                    ▼
     create_and_populate_commands  →  t_command linked list
                    │
          ┌─────────┴──────────┐
          │ piped?             │
          ▼                    ▼
  execute_commands_    execute_command_
  with_pipes           without_pipes
  (fork × N, dup2,     (fork or direct
   wait last child)     builtin call)
```

Memory for all intermediate structures (token list, command list, expanded strings)
is owned by the shell's `t_gc` garbage collector and freed at the end of each REPL
iteration.

## Status & limitations

This is a 42 school project built to the project specification.
It was created as a learning exercise and when our understanding of C was still developing, so the code is not full production quality. Now I would refactor it and add tests, correct git history...
The repo reflects the state of the project at submission.

