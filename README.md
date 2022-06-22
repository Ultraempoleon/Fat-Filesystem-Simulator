# Fat Filesystem Simulator
 Simulates a fat filesystem using a text file

- Project created in CSUSB CSE 4610 Advanced Operating Systems
- Running program simulates a terminal where user can create, copy
 and delete files
- the content of the files are saved onto a text file filled with the pound character equal to size of the system (created in main.cpp)
- pound characters are divided into the size of a block and file contents are saved onto these blocks
- the first 2 blocks save important information such as the file names and address, and a list of the fat listing off which blocks are blank
- Simulates a small database containing a log of historical events with an associated index
- The historical event's information can be found by searching the index through using the search in the program's terminal