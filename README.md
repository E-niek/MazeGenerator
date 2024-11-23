# MazeGenerator
Maze generator, written in C

### Options:
- -r: number of rows
- -c: number of columns
- -s: seed for random generation
- -o: output (file, stdout)
- -n: specify output file name (if output is set to file)
- -f: input file to generate the maze from
- -h: show this help

### Compile and run:
Run the following command to compile: ```gcc -I include/ src/*.c -o output```

Then, run the program with: ```./output [OPTIONS]```

> [!IMPORTANT]
> If you want to create mazes with row/column numbers higher then a few hundred, you might need to set your stack limit.
> This can e.g. be done with ulimit: ```ulimit -s unlimited```, or any size in bytes after -s.
