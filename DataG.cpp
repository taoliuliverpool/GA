#include <fstream>
#include <iostream>
using namespace std;

int main(){
  
  int DonorID;
  string DonorName[200];

  ifstream input;
  input.open ( "DonorList");
  input >> TotalNumber;

  for (int i = 0; i < TotalNumber; i++ )
  {
    input >> DonorID >> DonorName;
  }

  input.close ( );

  for (int i =0; i < TotalNumber; i++)
  {
    cout<< DonorID << ' ' <<Donorname <<' ' << len(Donorname) << endl;
  }

}
