# Redirection and Heredocs

## Redirection
Redirection is a feature in Unix-like operating systems that allows you to change the input or output of a command. It allows you to redirect the standard input, standard output, and standard error streams to different sources or destinations.

There are three basic types of redirection operators:
- `>`: Redirects standard output to a file, overwriting the file if it already exists.
- `>>`: Redirects standard output to a file, appending the output to the end of the file if it already exists.
- `<`: Redirects standard input from a file.

Examples:
- `command > output.txt`: Redirects the output of `command` to `output.txt`, overwriting the file if it exists.
- `command >> output.txt`: Redirects the output of `command` to `output.txt`, appending the output to the end of the file if it exists.
- `command < input.txt`: Redirects the input of `command` from `input.txt`.

## Heredocs
Heredocs, short for "here documents," are a way to include multiple lines of input within a command or script. They are useful when you need to provide input to a command that expects it interactively.

Heredocs are defined using the `<<` operator followed by a delimiter. The delimiter can be any word or string, and it marks the end of the heredoc.


!/bin/bash

 Using heredocs to provide input to a command

Here's a simple script that reads user input using a heredoc
and then echoes it back to the user

read -r -d '' message <<EOF
Hello, user!
Please enter your name:
EOF

echo "You entered: $message"

## dup2()

Syntax:

int dup2(int oldfd, int newfd);
 oldfd: alter Dateideskriptor.
 newfd : neuer Dateideskriptor, der von dup2() zum Erstellen einer Kopie verwendet wird.

Wichtige Punkte:

Fügen Sie die Header-Datei unistd.h ein, um die Systemaufrufe dup() und dup2() zu verwenden.
Wenn der Deskriptor newfd zuvor geöffnet war, wird er vor der Wiederverwendung stillschweigend geschlossen.
Wenn oldfd kein gültiger Dateideskriptor ist, schlägt der Aufruf fehl und newfd wird nicht geschlossen.
Wenn oldfd ein gültiger Dateideskriptor ist und newfd denselben Wert wie oldfd hat, tut dup2()
nichts und gibt newfd zurück.

