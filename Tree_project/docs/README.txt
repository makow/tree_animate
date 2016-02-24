*******************************************************************************
RANK-BALANCED BINARY TREE PROGRAM
*******************************************************************************

*******************************************************************************
* HOW THE PROGRAM WORKS AT A FILE LEVEL


** INPUT & OUTPUT FILES

The executable will generate a tree output text file called proto_tree.txt
which contains the output tree in bracket form from the input provided in
data.txt. Therefore, you must first generate an input text file. The
instructions that can be used in the input file are 'i' for insert, 'd' for
delete and 's' for search followed by an integer. For example entering 'i20'
will instruct the program to insert 20 into the tree. The input text
file can contain instructions separated by most delimeters and should be parsed
correctly. The output text file is then used as input to the texitree program
which draws plain text tree diagrams. Texitree was obtained from here and is
licensed to the Free Software Foundation:

http://fossies.org/linux/avl/texitree.c

Finally, the texitree output text file can be viewed in a text editor to
examine whether the tree has performed correctly.

The compilation flag -std=gnu11 is used to replace an equivalent c11
feature of safe string tokenisation (strtok_s) which did not work with my
setup.

The helvetica font is provided for systems which don't already have it
installed as it in required by texitree.

** FILE & DIRECTORY STRUCTURE

The directories and files are structured as follows:

a) src/input: An input.h and input.c file handle the input and are 'user'
provided.

b) src/compare: A compare.h and compare.c file handle the comparisons of the
keys with respect to their types and are also user provided.

c) src/core:A tree.h and tree.c file handle the core functions such as search,
insert and delete and then hand off the balancing to the specialised functions
residing in separate files. The functions in the 'tree' files accept generic
data types provided by the user by way of void pointers and function pointers.
The functions return pointers to the data item inserted or deleted from the tree
or NULL upon failure to locate the item. It is the user's responsibility to
check the return values.

d) src/balancing: Tree specific balancing functions for insert and delete
re-balancing located in 'h' and 'c' files called avl, wavl, two-three and
two-four.

e) bin: The executables of texitree and tree_exe are compiled here as are the
text files, proto_tree.txt and tree.txt.

f) data: This directory contains the file data.txt with the input instructions.

The instructions on how to compile and use the program at detailed below. I have
make a shell script that automates the whole process once a tree program
executable has been compiled and an instruction list composed. The program has
been tested on OS X (with Homebrew), Windows (with CygWin) and Ubuntu Linux with
GCC 4.9.3 and 5.3.0.


*******************************************************************************
* INSTRUCTIONS FOR COMPILING AND RUNNING THE TREE PROGRAM


** OPTION 1 - QUICKER

0) ./run_tree.sh
==>> The tree.txt file will need to be opened manually on Windows unless the
nano text editor has been installed and is accessible from the shell where the
commands are being run from. Viewing the tree may require widening the text
editor's window depending on how many items (and thus the width) have been
inserted into the tree.


** OPTION 2 - EXPOSES ALL STAGES

From within the parent directory (code):

0) Compile texitree with:
   gcc src/draw/texitree.c -o bin/texitree

1) Compile the tree program with:
   gcc -std=gnu11 -pedantic -Wall -Wextra -Werror src/input/input.c
   src/core/tree.c src/compare/compare.c src/balancing/avl.c
   src/balancing/wavl.c src/balancing/two-three.c src/balancing/two-four.c -o
   bin/tree_exe

3) bin/tree_exe 'tree_type' data/data.txt
==>> replace 'tree_type' with avl, wavl, 2-3 or 2-3-4 without the single quotes

4) bin/texitree text < bin/proto_tree.txt > bin/tree.txt

5) nano bin/tree.txt
==>> replace nano with your text editor of choice

NB - For MS Windows:
Append .exe to the executable names when invoking them in stages 3 & 4 and then
manually open the bin/tree.txt output file in a text editor if the nano editor
is not installed:

3)  bin/tree_exe.exe 'tree_type' data/data.txt
4)  bin/texitree.exe text < proto_tree.txt > bin/tree.txt
5)  open tree.txt with a text editor

*******************************************************************************
END
*******************************************************************************
