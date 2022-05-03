#include <iostream>
#include "Sdisk.h"

using namespace std;

// int main() {
  
//   // This main program inputs commands to the shell.
//   // It inputs commands as : command op1 op 
//   // You should modify it to work for your implementation
  
//   Sdisk disk1("disk1", 256, 128);
//   Filesys fsys("disk1", 256, 128);
//   Shell shell1("disk1", 256, 128);
//   string s;
//   string command="go";
//   string op1, op2;
//   int checker;
//   while (command != "quit"){
//     op1.clear();
//     op2.clear();
//     command.clear();
//     checker = 0;
//     // last character in s will be a $
//     cout << "\n$";
//     getline(cin, s);
//     int firstblank=s.find(' ');
    
//     // turns the first blank into a #
//     if (firstblank < s.length()){
//       s[firstblank]='#';
//     }
//     // returns the position of the "second" space
//     int secondblank=s.find(' ');
//     command=s.substr(0, firstblank); //creates copy of s starting at 0
    
//     // if the position of the firstblank < size of string
//     // ex. add filex is 2 words so firstblank(3) < 9 do:
//     if (firstblank < s.length()){
//       // seperates the second word from the entire string
//       op1 = s.substr(firstblank+1, secondblank-firstblank-1);
//     }
//     if (secondblank < s.length()){
//       op2=s.substr(secondblank+1);
//     }
//     if (command == "dir"){
//       if (op1.empty()){
//         checker = shell1.dir();
//       }
//       else {
//         cout << "\nOnly dir was read";
//         checker = shell1.dir();
//       }
//     }
//     // add
//     else if (command == "add"){
//       if (op1.empty()){
//         cout << "\nError, must include file name\n";
//         continue;
//       }
//       else if (op2.empty()){
//         cout << "\nError, must include file contents\n";
//         continue;
//       }
//       checker = shell1.add(op1, op2);
//       continue;
//     } //end of add
//     // del
//     else if (command == "del"){
//       if (op1.empty()){
//         cout << "\nError, must include file name";
//       }
//       else if (op2.empty()){
//         checker = shell1.del(op1);
//         continue;
//       }
//       else {
//         cout << endl;
//         cout << op2;
//         cout << " was not included in deletion";
//         checker = shell1.del(op1);
//         continue;
//       }
//     } //end of del


//     //type
//     else if (command=="type"){
//       if (op1.empty()){
//         cout << "\nError, must include file name";
//       }
//       else if (op2.empty()){
//         checker = shell1.type(op1);
//       }

//       else {
//         cout << endl;
//         cout << op2 << " was not included in type";
//         checker = shell1.type(op1);
//       }
//     } //end of type


//     else if (command == "copy"){
//       if (op1.empty()){
//         cout << "\nError, must include first file name";
//       }
//       else if (op2.empty()){
//         cout << "\nError, must include second file name";
//       }
//       else{
//         shell1.copy(op1, op2);
//       }
//     }


//     // quit
//     else if (command == "quit"){
//       continue;
//     }//end of quit
    
//     else {
//       cout << "\nCommand ";
//       cout << command;
//       cout << " not recognized\n";
//     } //end of last else


//   }//while end
// } // func end


int main(){
  Table table1("disk1", 256, 128, "flatfile", "indexfile");
  //table1.Build_Table("data.txt");
  table1.Search("-1720");
  
}