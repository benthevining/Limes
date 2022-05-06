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

### `absolute`: Convert an input path to an absolute path
```
FileUtil absolute <path> [<basePath>]
```
If `<path>` is an absolute path, prints `<path>`; otherwise, prints `<basePath>/<path>`. If `<basePath>` is not specified, it defaults to the current working directory.

### `append`: Append content to a given file
```
FileUtil append <filename> <content> [--strict]
```
Note that the content is given as a single argument. If the file did not already exist, then an error will be raised if the `--strict` option was given; otherwise, the content will be written to the file as if the `write` command were called.

### `cat`: Concatenate and display contents of the given files
```
FileUtil cat <filenames...> [--output <outputFile>]
```
If `<outputFile>` is specified, the concatenated output will be written to that file; otherwise, output is printed to the standard output.

### `cd`: Change the current working directory
```
FileUtil cd <directory>
```

### `copy`: Copy files and directories to a new location
```
FileUtil copy <filesOrDirs...> <destination>
```
In this command, the last argument is always the destination. If one input file is given, the destination may be a filename. If multiple inputs are given, the destination must be a directory. If the destination is a directory that doesn't exist, it will be created.

### `equiv`: Check if two paths are equivalent
```
FileUtil equiv <path1> <path2> [--error]
```
Prints 'yes' if the two paths refer to the same filesystem object; otherwise, prints 'no'.
If the `--error` option is given, the result of the comparison is indicated by an exit code of 0 or 1 instead of printing to standard output.

### `exists`: Check if files and/or directories exist
```
FileUtil exists <filesOrDirs...> [--error]
```
Prints `yes` if every item in the passed list exists; otherwise prints `no`.
If the `--error` option is given, the result of the comparison is indicated by an exit code of 0 or 1 instead of printing to standard output.

### `follow_symlink`: Print the target of a symbolic link
```
FileUtil follow_symlink <symlink> [--no-recurse]
```
This command follows the given symlink (and, if it points to another symlink, follows *that* symlink, etc) and prints the absolute path of the target to the standard output. If the optional `--no-recurse` argument is present, then the recursive behavior is disabled, and the path of the passed symlink is printed, even if its target is another symlink.

### `glob`: Find files/directories matching a pattern
```
FileUtil glob <globbingExpression> [--dir <baseDir>] [--recurse] [--error]
```
Prints a list of absolute paths found that match the given globbing expression.
If the `<baseDir>` option is given, files will be searched for in that directory. Otherwise, it defaults to the current working directory. If the --error flag is given, the program will exit with an error code if no files are found. Otherwise, 'No files found' will be printed.

### `hash`: Compute a hash of a file's contents
```
FileUtil hash <hashType> <filename>
```
Available hash types are: `md5`, `sha1`, `sha224`, `sha256`, `sha384`, `sha512`

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

### `native`: Convert a path to the current platform's preferred directory separators
```
FileUtil native <path>
```
Converts any directory separators in `<path>` to the preferred directory separator for the current platform. Unlike most other commands, this one does not make the passed path absolute if a relative path was given.

### `path`: Prints the contents of the PATH environment variable
```
FileUtil path
```
Prints the contents of the PATH environment variable as a list of absolute paths to directories, one per line of output.

### `prepend`: Prepend content to a file
```
FileUtil prepend <filename> <content> [--strict]
```
Note that the content is given as a single argument. If the file did not already exist, then an error will be raised if the `--strict` option was given; otherwise, the content will be written to the file as if the `write` command were called.

### `pwd`: Print the absolute path of the current working directory
```
FileUtil pwd
```

### `relative`: Get a path relative to another location
```
FileUtil relative <path> [<basePath>]
```
If `<basePath>` is not specified, it defaults to the current working directory.

### `rename`: Rename a file or directory
```
FileUtil rename <oldName> <newName>
```

### `rm`: Remove files and/or directories
```
FileUtil rm <filesOrDirs...>
```

### `sep`: Prints the current platform's preferred directory separator

### `size`: Prints the size of a file or directory, in bytes
```
FileUtil size [<fileOrDir>]
```
The size is printed to standard output and is represented as bytes. If a directory is passed, this command reports the cumulative size of all the directory's contents. If no file or directory is specified, this command prints the capacity of the entire filesystem.

### `space`: Prints the amount of remaining space on the filesystem, in bytes
```
FileUtil space
```

### `touch`: Update modification time of files or directories
```
FileUtil touch <filesOrDirs...> [--no-create]
```
If the `--no-create` option is specified, files/directories in the input list that don't already exist will simply be ignored. Otherwise, they will be created.

### `type`: Print the type of a filesystem entry
```
FileUtil type <path>
```
If nothing exists at `<path>`, raise an error. Otherwise, prints one of `file`, `directory`, or `symlink` describing what exists at the given path.

### `which`: Locate an executable
```
FileUtil which [<programName>]
```
Searches for an executable file named `<programName>` in each of the directories in the PATH environment variable and prints the absolute path of the first match found. If the `<programName>` is ommitted, this command searches for the FileUtil executable itself.

### `write`: Write content to a file
```
FileUtil write <filename> <content> [--no-overwrite]
```
Note that the content is given as a single argument. If the `--no-overwrite` option is given, then an error is raised if the file already existed; otherwise, it will be overwritten.


## Dependencies

* [limes_core](../../libs/limes_core/README.md)

## Targets

* Limes::FileUtil

## Install components

* limes_file_util_dev
* limes_file_util_runtime
* limes_file_util (group)
