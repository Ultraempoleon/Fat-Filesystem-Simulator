#ifndef SDISK_H
#define SDISK_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Sdisk
{
    public:
    Sdisk(string disk_name, int number_of_blocks, int block_size);
    Sdisk(); //default constructor
    int getblock(int blocknumber, string& buffer);
    int putblock(int blocknumber, string buffer);
    int getnumberofblocks(); //accessor function
    int getblocksize(); //accessor function
    string getdiskname(); //accessor function
    void characterprinter(ofstream &file, int numberofblocks, int blocksize); //prints characters onto text file
    void file_check(string file_name, ofstream &file, int numberofblocks, int blocksize); //checks if file exists

    private:
    string diskname;    //file name of software-disk
    int numberofblocks; //number of blocks on disk
    int blocksize;  //block size in bytes

};

//write readblock
class Filesys: public Sdisk
{
    public:
    Filesys(string disk_name, int number_of_blocks, int block_size); // check
    Filesys(); //default constructor // check
    int fsclose(); //Writes FAT and ROOT to the disk (and closes it?)
    int newfile(string file); //Adds an entry for the string file in ROOT //check
    int rmfile(string file); //Check
    int getfirstblock(string file); //check
    int addblock(string file, string buffer); //Check
    int delblock(string file, int blocknumber); //Check
    int readblock(string file, int blocknumber, string &buffer); //check
    int writeblock(string file, int blocknumber, string buffer); //check
    int nextblock(string file, int blocknumber); //check
    void setpoundamount(); //checks how many pounds are in block 1; //check
    void tester(); //using to test stuff // check
    bool blockchecker(int filepos, int searchblock); //checks if file has the blocks being looked for
    bool filechecker(string file); //checks if file exists
    vector<string> ls();
    vector<string> getfilename();
    vector<int> getfirstblock();
    vector<int> getfat();

    private:
    int rootsize; //max number of entries in ROOT
    int fatsize; //number of blocks occupied by FAT
    int poundamount; //number of pound signs left in block 1
    vector <string> filename; //filename in ROOT
    vector <int> firstblock; //firstblocks in ROOT
    vector <int> fat; // FAT
    int buildfs(); //builds the file system  // check
    int readfs(); //synchs // check
    int fssynch(); //writes FAT and ROOT to the sdisk // check
};

class Shell: public Filesys {
    public:
    Shell(string filename, int blocksize, int numberofblocks);
    int dir(); //lists all files
    int add(string file, string buffer); //add a new file using buffer as data
    int del(string file); //deletes the file
    int type(string file); //lists the contents of file
    int copy(string file1, string file2); //copies file1 to file2
};

class Table: public Filesys {
    public:
    Table(string disk_name, int block_size, int number_of_blocks, string flatfile, string indexfile);
    int Build_Table(string input_file);
    int Search(string value);

    private:
    string flatfile;
    string indexfile;
    int IndexSearch(string value);
};

void read_single_block(string block_content, int blocksize);
vector<string> block(string buffer, int b);
string combine(string date, string h_s);
int roundten(int bs);
#endif