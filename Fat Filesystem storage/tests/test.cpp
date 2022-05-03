#include <iostream>
using namespace std;

int main(){
    string buffer = "-172013  *-175014  *-129015  *-125016  *-118417  *-117518  *-922 19  *-950 20  *-776 21  *-753 22  *-750 23  *-725 24  *########";
    string value = "-990";
    while(value.size() != 5){
        value += ' ';
    }
    cout << value << endl;
    cout << value.size();
}