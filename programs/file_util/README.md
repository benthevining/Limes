<!-- markdownlint-disable -->
```
 __    ____  __  __  ____  ___
(  )  (_  _)(  \/  )( ___)/ __)
 )(__  _)(_  )    (  )__) \__ \
(____)(____)(_/\/\_)(____)(___/
```

# FileUtil

A command line utility to provide some universal file utility operations.

Run without arguments or with a single argument of `help` or `--help` to display usage.

In every command, if a filename or directory is not given as an absolute path, it will be interpreted as relative to the current working directory.

In most commands, an error will be reported if input files or directories do not exist. See each individual command for exceptions to this.

## Usage:

```
FileUtil <mode> [<additionalArgs...>]
```

Where `mode` is one of:

### `append`: Append content to a given file
```
FileUtil append <filename> <content>
```
Note that the content is given as a single argument. If the file didn't previously exist, the content will be written to the file as if the `write` command was called.

### `cat`: Concatenate and display contents of the given files
```
FileUtil cat <filenames...>
```

### `cd`: Change the current working directory
```
FileUtil cd <directory>
```

### `copy`: Copy files and directories to a new location
```
FileUtil copy <filesOrDirs...> <destination>
```
In this command, the last argument is always the destination. If one input file is given, the destination may be a filename. If multiple inputs are given, the destination must be a directory. If the destination is a directory that doesn't exist, it will be created.

### `exists`: Check if files and/or directories exist
```
FileUtil exists <filesOrDirs...>
```
Prints `yes` if every item in the passed list exists; otherwise prints `no`.

### `follow_symlink`: Print the target of a symbolic link
```
FileUtil follow_symlink <symlink> [--no-recurse]
```
This command follows the given symlink (and, if it points to another symlink, follows *that* symlink, etc) and prints the absolute path of the target to the standard output. If the optional `--no-recurse` argument is present (must be last), then the recursive behavior is disabled, and the path of the passed symlink is printed, even if its target is another symlink.

### `ln`: Create a symbolic link
```
FileUtil ln <symLinkPath> <symLinkTarget>
```

### `ls`: List directory contents
```
FileUtil ls [<directory>]
```
The additional `<directory>` argument is optional; if ommitted, this command lists the contents of the current working directory.

### `mkdir`: Create a directory
```
FileUtil mkdir <directory>
```

### `modtime`: Display the last modification time of a file or directory
```
FileUtil modtime <fileOrDirectory>
```
This command prints the last modification time of the file or directory to standard output in the form `HH:MM:SS Day Month Year`, with the time in 24-hour format and the month as a 3-letter abbreviation; for example, `03:33:52 2 May 2022`.

### `pwd`: Print the absolute path of the current working directory
```
FileUtil pwd
```

### `relative`: Get a path relative to another location
```
FileUtil relative <path> <basePath>
```

### `rename`: Rename a file or directory
```
FileUtil rename <oldName> <newName>
```

### `rm`: Remove files and/or directories
```
FileUtil rm <filesOrDirs...>
```

### `size`: Prints the size of a file or directory, in bytes.
```
FileUtil size <fileOrDir>
```
The size is printed to standard output and is represented as bytes. If a directory is passed, this command reports the cumulative size of all the directory's contents.

### `space`: Prints the amount of remaining space on the filesystem, in bytes
```
FileUtil space
```

### `touch`: Update modification time of files or directories, creating them if they don't exist
```
FileUtil touch <filesOrDirs...>
```

### `touch_nocreate`: Update modification time of files or directories, but do nothing if they don't exist
```
FileUtil touch_nocreate <filesOrDirs...>
```
In this command, no error will be reported if passed files or directories do not exist.

### `type`: Print the type of a filesystem entry
```
FileUtil type <path>
```
If nothing exists at `<path>`, raise an error. Otherwise, prints one of `file`, `directory`, or `symlink` describing what exists at the given path.

### `write`: Write content to a file
```
FileUtil write <filename> <content>
```
Note that the content is given as a single argument. If the file already existed, it will be overwritten.


## Dependencies

* [limes_core](../../libs/limes_core/README.md)

## Targets

* Limes::FileUtil

## Install components

* limes_file_util_dev
* limes_file_util_runtime
* limes_file_util (group)
