# MazeGenerator
Maze generator, written in C

### Options:
- -r: number of rows
- -c: number of columns
- -s: seed for random generation
- -o: output; stdout, file, img
- -n: if the output is a file or img, specify the name here
- -f: input file to generate the maze from
- -h: show this help

### Image
The image is saved as a .tga file. This is unfortunately not a widely supported image format, so if you want, you can convert it here (e.g. to png or jpg): https://convert.karos.dev

### Compile and run:
Run the following command to compile: ```gcc -I include/ src/*.c -o output```

Then, run the program with: ```./output [OPTIONS]```

> [!IMPORTANT]
> If you want to create mazes with row/column numbers higher then a few hundred, you might need to set your stack limit.
> This can e.g. be done with ulimit: ```ulimit -s unlimited```, or any size in bytes after -s.
