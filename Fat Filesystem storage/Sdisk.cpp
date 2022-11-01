#include "Sdisk.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>


using namespace std;

//Block function
vector<string> block(string buffer, int b){
    //blocks buffer into a list of blocks of size b
    vector<string> blocks;
    int numberofblocks=0;

    if (buffer.length() % b == 0) {
        numberofblocks= buffer.length()/b;
    }
    
    else {
        numberofblocks= buffer.length()/b +1;
    }

    string tempblock;
    
    for (int i=0; i<numberofblocks; i++){
        tempblock=buffer.substr(b*i,b);
        blocks.push_back(tempblock);
    }

    int lastblock=blocks.size()-1;


    for (int i=blocks[lastblock].length(); i<b; i++){ 
        blocks[lastblock]+="#";
    }
    
    return blocks;
}

//constructor
Sdisk::Sdisk(string disk_name, int number_of_blocks, int block_size){
    diskname = disk_name;
    numberofblocks = number_of_blocks;
    blocksize = block_size;
    ofstream file;
    string file_name = diskname;

    file_check(file_name, file, numberofblocks, blocksize
    ); //if file exists open, if not create it
}

//def
Sdisk::Sdisk() {
    //cout << "Uh the Sdisk default constructor was used";
}

//checks if exists bef
void Sdisk::file_check(string file_name, ofstream &file, int numberofblocks, int blocksize) {
    ifstream file_check;
    file_check.open(file_name);

    //file already exists so we open it with ofstream
    if(file_check) {
        file_check.close();
        file.open(file_name, ios::app);
    }

    //if file doesn't exist it is created
    else {
        cout << "Sdisk doesn't exist, creating file";
        file.open(file_name);
        characterprinter(file, numberofblocks, blocksize);
    }
}


//prints characters onto file if file doesn't exist
void Sdisk::characterprinter(ofstream &file, int numberofblocks, int blocksize){
    int blocks = numberofblocks * blocksize;
    for (int i=0; i < blocks; i++) {
        file << "#";
    }
}


//returns the blocknumber called for and passes it by ref in buffer
int Sdisk::getblock(int blocknumber, string &buffer){
    ifstream file(diskname);
    file.clear();
    int i = (blocknumber -1) * blocksize;
    int j = (blocknumber * blocksize) -1;
    int k = 0;
    char b;

    if (j > (blocksize * numberofblocks)) {
        file.close();
        return 0;
    }

    while (file >> noskipws >> b) {
        if ((k > i-1) && (k < j+1)) {
            buffer.push_back(b);
        }
        k++;
    }
    file.close();
    file.clear();
    return 1;
}


//writes onto block
int Sdisk::putblock(int blocknumber, string buffer){
    fstream s(diskname, fstream::binary | fstream::out | fstream::in);
    s.clear();
    if (buffer.size() > blocksize) {
        cout << "buffer too large\n";
        return 0;
    }
    int i = (blocknumber - 1) * blocksize;
    s.seekp(i);
    s << buffer;
    s.close();
    s.clear();
    return 1;
}


//returns size of block //these 2 don't work properly don't use them (might be working now havn't checked)
int Sdisk::getblocksize() {
    return blocksize;
}


int Sdisk::getnumberofblocks() {
    return numberofblocks;
}


string Sdisk::getdiskname() {
    return diskname;
}

//Filesys functions

//Fylsys constructor
Filesys::Filesys(string disk_name, int number_of_blocks, int block_size): Sdisk(disk_name, number_of_blocks, block_size)
{
    string buffer;
    rootsize = getblocksize()/ 10; //maximum number of files that exist in the root every file in root size has size of 10 characters
    fatsize = (getnumberofblocks() * 5)/getblocksize() + 1; //total size of the fat getnum * 5 is going to be the total size of the FAT needed
                                                            //divide by block size, +1
    int x; 
    int code = getblock(0, buffer);

    if (code == 0) {
        cout << "Bad disk";
    }

    code = getblock(1, buffer); //is there a root on the disk

    if (buffer[0] == '#'){ //gonna build the filesystem
        cout << "No filesystem, building filesystem \n";
        x = buildfs();
        setpoundamount();
    }

    else {
        cout << "File system found, reading filesystem\n";
        x = readfs(); //will read the root on disk
        setpoundamount();
    }
}


//Builds filesystem
int Filesys::buildfs() {
    //Root

    for (int i = 0; i < rootsize; i++){
        filename.push_back("xxxx"); //xxxx wil mean open spot
        firstblock.push_back(0); //  
    }

    /**********************************************
    * Fat (block number 1 is going to be the root)
    * Fat is going to start at block 2 and have fatsize many blocks
    * * finish at 2 + fatsize -1
    ****************************************************/
    
    fat.push_back(fatsize+2);
    fat.push_back(1);

    for (int i=0; i < fatsize; i++){
        fat.push_back(1); //indicate that this is a reserved block not used for data
    }

    for (int i = fatsize+2; i < getnumberofblocks(); i++){
        fat.push_back(i+1);
    }

    fat[fat.size()-1] = 0; //last block is 0

    int x = fssynch();
    cout << "\nFilesystem created\n";
    return 1;
}


//Reads the disk
int Filesys::readfs() {
    istringstream istream, istream2;
    string buffer, buffer2, b;
    string s;
    int t, t2, x;

    /*** Root ***/
    getblock(1, buffer);
    istream.str(buffer);

    for (int i = 0; i < rootsize; i++){
        istream >> s;
        istream >> t;

        filename.push_back(s);
        firstblock.push_back(t);

    }

    /*** FAT ***/

    //Error


    for (int i = 0; i < fatsize-2; i++){
        b = "";
        x = getblock(i+2, b);
        buffer2 += b;
    }

    istream2.str(buffer2);
    for (int i = 0; i < getblocksize() * 2; i++){
        istream2 >> t;
        fat.push_back(t);
    }
    //tester();

    return 1;
}


//write root and fat to the disk
int Filesys::fssynch(){
    ostringstream ostream, ostream2;
    string buffer, buffer2;
    vector<string> blocks, blocks2;
    int x;

    //Root
    for (int i = 0; i < rootsize; i++){
        ostream << filename[i] << " " << firstblock[i] << " ";
    }

    buffer = ostream.str();
    blocks = block(buffer, getblocksize());
    x = putblock(1, blocks[0]); //root has been written to Sdisk in block #1


    /* TESTS */
    //Fat vector is correct, not writing correctly?

    //printing Fat

    for (int i =0; i < fat.size(); i++) {
        ostream2 << fat[i] << " ";
    }

    buffer2 = ostream2.str();
    blocks2 = block(buffer2, getblocksize());

    //ERROR?
    for (int i = 0; i < blocks2.size(); i++) {
        x = putblock(2+i, blocks2[i]);
    }

    return 1;
}


//This function adds an entry for the string file in ROOT with an initial first block of 0 (empty). It returns error codes of 1 if successful
// and 0 otherwise (no room or file already exists).
int Filesys::newfile(string file){
    int alocate, openblock;
    int search = 0;
    vector<string> blocks;

    for (string s:filename){
        if (s == file) {
            cout << "\nFile " << file;
            cout << " already exists, file not created";
            return 0;
        }
    }

    //is there enough file space?
    //needs to be at least 4 + 2 spaces available
    //pound amount needs to be greater than 6
    if (poundamount < 6){
        cout << "\nNot enough space for this file";
        cout << "\nFile " << file;
        cout << " not added";
        return 0;
    }

    //looks for xxxx in filenames
    //means that the corresponding file is empty and
    //usable
    for (search; search < firstblock.size(); search++) {
        if (filename[search] == "xxxx") {
            break;
        }
    }

    // no empty files return 0
    if (search == filename.size()) {
        cout << "\nNo available files";
        cout << "\nFile was not created";
        return 0;
    }

    //add clause for not enough space

    //changes corresponding file name to the file
    filename[search] = file;

    int x = fssynch();
    //tester();
    return 1;
}


//Removes the file from the system and frees the block
int Filesys::rmfile(string file){
    //Find file
    if(!filechecker(file)){
        return 0;
    }

    int i = 0;
    for (i; i < filename.size(); i++){
        if (filename[i] == file){
            break;
        }
    }

    vector<int> file_blocks;
    int f_block = firstblock[i];

    if (fat[f_block] == 0){
        delblock(file, f_block);
    }
    else {
    while (fat[f_block] != 0){
        file_blocks.push_back(f_block);
        f_block = fat[f_block];
    }
    file_blocks.push_back(f_block);

    for (int counter = (file_blocks.size() - 1); counter >= 0; counter--){
        delblock(file, file_blocks[counter]);
        tester();
    }

    }

    filename[i] = "xxxx";

    int j = fssynch();

    return 1;


}


//returns corresponding firstblock for file input (doesn't work)
int Filesys::getfirstblock(string file){
    int i = 0;
    for (i; i < filename.size(); i++) {
        if (filename[i] == file) {
            break;
        }
    }

    if (i == filename.size()){
        cout << "\nFile not found";
        return -1;
    }

    return firstblock[i];
}


//adds block
int Filesys::addblock(string file, string buffer){
    if (!(filechecker(file))){
        return 0;
    }

    int i = 0;
    for (i; i < filename.size(); i++){
        if (filename[i] == file){
            break;
        }
    }

    //if New file block is not initialized
    //DO NOT ALLOW BUFFER TO BE BIGGER THAN BLOCKSIZE OR THIS WILL GO HAYWIRE
    if (firstblock[i] == 0){
        int allocate = fat[0];
        int empty_block = fat[allocate];
        fat[0] = empty_block; //next availble block
        fat[allocate] = 0;
        firstblock[i] = allocate;

        vector<string> blocks = block(buffer, getblocksize());
        putblock(allocate, blocks[0]);

        int x = fssynch();
        return 1;
    }

    
    int allocate = fat[0]; //Allocate = next availble block
    int newblock = fat[allocate]; //First block of file
    int oldendblock = firstblock[i]; //Find the next available empty block

    //is there room available?
    if(fat[allocate] == 0){
        cout << "\nFile system is full!";
        cout << "\nBlock was not added";
        return -1;
    }

    int pos = buffer.find("#");
    if (pos != string::npos){
        cout << "\nString cannot contain # sorry";
        cout << "\nBlock was not added";
        cout << "\n" << buffer;
        return -1;
    }

    while (fat[oldendblock] != 0) {
        oldendblock = nextblock(file, oldendblock);
    }
    
    fat[oldendblock] = allocate;
    fat[0] = newblock;
    fat[allocate] = 0;

    vector<string> blocks = block(buffer, getblocksize());

    putblock(allocate, blocks[0]);


    int x = fssynch();
    //tester();
    return 1;
}


//deletes a block
int Filesys::delblock(string file, int blocknumber){
    if(!filechecker(file)){
        return 0;
    }
    int i = 0;
    for (i; i < filename.size(); i++){
        if (filename[i] == file){
            break;
        }
    }

    if (blocknumber == 0 || blocknumber == 1){
        cout << "\nCannot delete blocks 0 or 1";
        cout << "\nBlock cannot be deleted";
        return 0;
    }

    int allocate = fat[0];
    int newfreeblock = fat[blocknumber];
    int f_block = firstblock[i];

    // cout << "\nallocate " << allocate;
    // cout << "\n nfb " << newfreeblock;
    // cout << "\n fblock " << f_block;

    if (f_block != blocknumber){
        while (fat[f_block] != blocknumber){
            f_block = fat[f_block];
        }
    }

    fat[blocknumber] = allocate;
    fat[0] = blocknumber;
    if (f_block == blocknumber){
        firstblock[i] = newfreeblock;
    }
    else {
        fat[f_block] = newfreeblock;
    }

    //delete block contents
    string buffer;
    int y = getblock(blocknumber, buffer);
    buffer = "";
    for (int j = 0; j < getblocksize(); j++){
        buffer += "#";
    }

    putblock(blocknumber, buffer);
    int x = fssynch();

    return 1;


    
}


//returns the contents of a block by removing all the # from it
int Filesys::readblock(string file, int blocknumber, string &buffer){
    int i = 0;
    string block_buffer;

    if (!filechecker(file)){
        cout << "\nFile not found";
        return -1;
    }

    if (buffer.size() > getblocksize()){
        cout << "\nBuffer too large";
        cout << "\nblock not written";
        return 0;
    }

    //returns block but returns it with all the pound signs
    int x = getblock(blocknumber, buffer);

    int pos = buffer.find("#");
    int howmuchtoerase = getblocksize() - pos;
    buffer.erase(pos, howmuchtoerase);

    return 1;
    
}


//writes onto block, '#' cannot be added
int Filesys::writeblock(string file, int blocknumber, string buffer){
    int i = 0;
    string block_buffer;
    // for (i; i < filename.size(); i++){
    //     if (filename[i] == file){
    //         break;
    //     }
    // }

    // if (i == filename.size()){
    //     cout << "\nFile not found";
    //     return 0;
    // }

    if (!filechecker(file)){
        cout << "\nFile not found";
        return 0;
    }

    

    if (buffer.size() > getblocksize()){
        cout << "\nBuffer too large";
        cout << "\nblock not written";
        return 0;
    }

    int pos = buffer.find("x");
    if (pos != string::npos){
        cout << "\nString cannot contain # sorry";
        cout << "\nBlock was not added";
        return 0;
    }

    
    vector<string> blocks = block(buffer, getblocksize());
    putblock(blocknumber, blocks[0]);
    return 0;




}


//goes to the next block in the file's chain
int Filesys::nextblock(string file, int blocknumber){

    if (!filechecker(file)){
        cout << "\nFile not found";
        return -1;
    }

    if (fat[blocknumber] == 0){
        cout << "\nEnd of blocks";
        return 0;
    }

    return fat[blocknumber];
}

//returns string vector of all files
vector<string> Filesys::ls(){
    vector<string> flist;
    for (int i =0; i < filename.size(); i++){
        if (filename[i] != "xxxx"){
            flist.push_back(filename[i]);
        }
    }

    return flist;
}


//Added functions

//checks if the block being looked for belongs to a file
bool Filesys::blockchecker(int filepos, int searchblock){
    int first_block = firstblock[filepos];
    if (first_block == searchblock) {
        return true;
    }

    int end_block = fat[first_block];
    while (end_block != 0){
        if (end_block == searchblock){
            return true;
        }

        end_block = fat[end_block];
    }

    return false;

}


bool Filesys::filechecker(string file){
    int i = 0;
    for (i; i < filename.size(); i++){
        //cout << "\nfile name: " << filename[i];
        if (filename[i] == file){
            return true;
        }
    }

    if (i == filename.size()){
        cout << "\nFile not found";
        return false;
    }

    return false;
}


//Tests to make sure its all working
void Filesys::tester(){

    cout << "\nTesting filesystem contents";

        cout << "\nFilename contents :\n";
        for (string s: filename){
            cout << s;
        }

        cout << "\nFirst block contents :\n";
        for (int i : firstblock){
            cout << i;
        }

        cout << "\nFat contents :\n";
        for (int i : fat) {
            cout << i << " ";
        }

}

//accessor functions
vector<string> Filesys::getfilename(){
    return filename;
}

vector<int> Filesys::getfirstblock(){
    return firstblock;
}

vector<int> Filesys::getfat(){
    return fat;
}


//Gets the amount of pound signs in block 1
//used to check if there is room available in root
void Filesys::setpoundamount(){
    string buffer;
    getblock(1, buffer);
    int n = count(buffer.begin(), buffer.end(), '#');
    poundamount = n;
}


/*****************
 * SHELL FUNCTIONS
 *****************/

Shell::Shell(string disk_name, int number_of_blocks, int block_size): Filesys(disk_name, number_of_blocks, block_size){
    //Not sure if anything goes here
}

//Has too many spaces will figure out why later
int Shell::dir(){
    vector<string> flist = ls();
    for (int i=0; i < flist.size(); i++){
        cout << flist[i] << endl;
    }
    return 1;
}


//Adds file from shell
int Shell::add(string file, string buffer){
    int x, y;
    vector<string> blocks = block(buffer, getblocksize());
    x = newfile(file);
    if (x == 1){
        for (int i=0; i < blocks.size(); i++){
            y = addblock(file, buffer);
            if (y != 1){
                cout << "buffer could not be added";
                return 0;
            }
        }

        if (y == 1) {
            return 1;
        }
    }
    else{
        cout << "\nFailed somewhere";
    }
    return 0;
}//add a new file using buffer as data

//Deletes file from shell
int Shell::del(string file){
    int x;
    x = rmfile(file);
    if (x == 1){
        return 1;
    }
    return 0;
}

//type function (not sure if it works with multiple buffers)
int Shell::type(string file){
    vector<string> filename = getfilename();
    vector<int> firstblock = getfirstblock();
    vector<int> fat = getfat();
    string buffer;
    string biggerbuffer;

    int check;
    if (!filechecker(file)){
        return 0;
    }

    int i = 0;
    for (i; i < filename.size(); i++){
        if (filename[i] == file){
            break;
        }
    }

    //if file has 1 block f_block is 13 (its where the firstblock is)
    vector<int> file_blocks;
    int f_block = firstblock[i];

    if (fat[f_block] == 0){
        int x = readblock(file, f_block, buffer);
        if (x != 1){
            cout << "Coudn't read block";
            return 0;
        }

        cout << endl << buffer;
        return 1;
    }

    else if (fat[f_block] != 0){
        while(fat[f_block] != 0){
            file_blocks.push_back(f_block);
            f_block = fat[f_block];
        }

        file_blocks.push_back(f_block);

        for (int counter = 0; counter < file_blocks.size(); counter ++){
            int x = readblock(file, file_blocks[counter], buffer);
            biggerbuffer += buffer;
        }

        cout << biggerbuffer;
        return 1;

    }

    return 0;

}// End of type

//Shell copy, using the same code as above pretty much have no idea if multiple buffers will work?
int Shell::copy(string file1, string file2){
    vector<string> filename = getfilename();
    vector<int> firstblock = getfirstblock();
    vector<int> fat = getfat();

    int x, y, z;
    string newbuffer, buffer;

    int check;
    if (!filechecker(file1)){
        return 0;
    }

    //add clause for if file doesn't exist too lazy to do it now
    //it'll bite you later you monkey

    // if (filechecker(file2)){
    //     cout << "\nA file named " << file2;
    //     cout << " already exists";
    //     return 0;
    // }



    int i = 0;
    for (i; i < filename.size(); i++){
        if (filename[i] == file1){
            break;
        }
    }//returns file's firstblock
    cout << "\ncopy here";
    //if file has 1 block f_block is 13 (its where the first block is)
    vector<int> file_blocks;
    int f_block = firstblock[i];

    if (fat[f_block] == 0){
        int x = readblock(file1, f_block, newbuffer);
        if (x != 1){
            cout << "Couldn't read block";
            return 0;
        }

        y = add(file2, newbuffer);
        if (y !=1){
            cout << "file copy was unsuccessful";
            return 0;
        }

        return 1;
    }

    //this works not too sure if there are multiple blocks if that works?
    else if(fat[f_block] != 0){
        while (fat[f_block] != 0){
            file_blocks.push_back(f_block);
            f_block = fat[f_block];
        }

        file_blocks.push_back(f_block);
        for (int counter = 0; counter < file_blocks.size(); counter ++){
            int x = readblock(file1, file_blocks[counter], buffer);
            if (x != 1){
                cout << "\nReadblock in copy function has failed";
                return 0;
            }
            newbuffer += buffer;
        }

        y = add(file2, newbuffer);
        if (y != 1){
            cout << "\nSomething went wrong with copy function";
            return 0;
        }
    }
    cout << "Something went wrong who tf knows";
    return 0;
}//end of copy function

/*************************
 * Table Functions
 ************************/

//constructor for Table
Table::Table(string disk_name, int number_of_blocks, int block_size, string flatfile, string indexfile): Filesys(disk_name, number_of_blocks, block_size){
    newfile(flatfile);
    newfile(indexfile);
}

int Table::Build_Table(string input_file){
    string file_name = "flatfile";
    ifstream infile(input_file);
    string line, buffer, date, allocate;
    vector<string> blocks;
    vector<int> fat;
    vector<string> date_allocate;
    int x;
    int inc = 1;
    int bs = getblocksize();
    bs = roundten(bs);  //Block size is 128, bs is 120

    //Bug Fix
    fat = getfat();
    if (fat[192] != 193){
        cout << "\nDatabase already exists, Database not added";
        return 1;
    }

    //writes every line from the file onto a new block
    while(getline(infile, line)){
        cout << "\nAdding db entry " << inc;
        cout << " to flatfile";

        fat = getfat();
        allocate = to_string(fat[0]);

        date = line.substr(0, 5);

        buffer += combine(date, allocate); //buffer is gathering the things
        
        //if it hits over 120 characters it will add it to a vector
        if (buffer.size() == bs){
            date_allocate.push_back(buffer);
            buffer.clear();
        }

        x = addblock(file_name, line);
        if (x != 1){
            cout << "\nadd block failed in Build Table";
            return 0;
        }
        inc++;
    }


    for (int ii = 0; ii < date_allocate.size(); ii++){
        cout << "\nIndex file being added";
        x = addblock("indexfile", date_allocate[ii]);
        if (x != 1){
            cout << "\nadd block failed for index file in buildblock";
            return 0;
        }
    }
    return 1;
}


//Table search function
int Table::Search(string value){
    string buffer; //holds string value from block
    string value_check;
    string block_content;
    string delimitter = "*";
    int firstblock = getfirstblock("indexfile");
    bool found = false;
    bool found2 = false;
    int inc = 0;
    int inc2 = 0;
    int block_number;
    size_t index;

    while (value.size() != 5){
        value += " ";
    }
    //cout << "\nlooking for " << value;

    while (!found){ //checks all of index file
        buffer.clear();
        inc++;
        int x = getblock(firstblock, buffer);
        if (x != 1){
            cout << "\nSomething went wrong using getblock in Search function";
        }

        // cout << "\nBlock being checked\n" << buffer;
        // cout << endl;

        //searches the first value in the block before going into the loop
        value_check = buffer.substr(0, 5);
        if (value_check == value){ //found
            found = true;
            found2 = true;
            break;
        }
        else{
            index = buffer.find(delimitter) + 1;
        }
        while (!found2){ //checks current index block
            value_check = buffer.substr(index, 5);
            if (value_check == value){ //found
                found = true;
                found2 = true;
                break;
            }
            if (index == 120){ //file isn't in the current block
                found2 = true;
            }
            inc2++;
            index = buffer.find(delimitter) + index + 1;
        }

        found2 = false;
        if (inc >= 15){
            cout << "\nStopping loop in Search function";
            found = true;
        }

        //value was not found in the block
        //so looking at the next block
        if (found == false){
            firstblock = nextblock("indexfile", firstblock);
            if (firstblock <= 0){
                cout << "\nsomething went wrong getting next block in search function";
                return 0;
            }
        }

    }
    //cout << "\nfound?: " << value_check;

    //this line exists to cover when the first item on the block, the index returns 9, we want 0
    if(index < 10){
        index = 0;
    }

    //cout << "\ncurrent index " << index;
    string newbuffer = buffer.substr(index+5, 4);
    block_number = std::stoi(newbuffer);

    int y = getblock(block_number, block_content);
    if (y != 1){
        cout << "\nsomething went wrong getting block in search function";
    }

    read_single_block(block_content, getblocksize());
    return 1;
}

void read_single_block(string block_content, int blocksize){
    int pos = block_content.find("#");
    int howmuchtoerase = blocksize - pos;
    block_content.erase(pos, howmuchtoerase);
    cout << "\n" << block_content;
}

//rounds down to the nearest tenth
int roundten(int bs){
    bs = bs/10;
    bs = bs*10;
    return bs;
}

//combines date and allocated number into a 10 char string
string combine(string date, string h_s){
    string buffer;
    buffer += date;
    buffer += h_s;
    while (buffer.size() < 9){
        buffer += " ";
    }
    buffer += "*";
    return buffer;
}

