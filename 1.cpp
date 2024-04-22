//***************************************
//
//	GA by Xtb
//
//	Author: Tao Liu
//
//***************************************
#include <fstream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <stdlib.h> 

#include <chrono>
#include <thread>

using namespace std;
 
const double CROSSOVER_RATE= 0.7;
const double MUTATION_RATE= 0.01;
const int pop= 100;
const int CHROMO_LENGTH=  10;
const int  MAX_ALLOWABLE_GENERATIONS =  80;


struct genotype
{
	int ID=-1;
	string Name="";
	double Fitness=0.0;
	double IP=0.0;
	double EA=0.0;
	double GAP=0.0  ;
	bool goodIPEA = false;
	double Esol=0.0;
	double Absp = 10.0;
	double Binding  = 0.0 ;
	genotype():ID(-1),Name(""),Fitness(0.0),IP(0.0),EA(0.0),goodIPEA(false),Esol(0.0),Absp(10.0),Binding(0.0){};
	genotype(int id, string name,double ftns,double abs_, double binding_):ID(id),Name(name),Fitness(ftns),Absp(abs_),Binding(binding_){};
};

struct genotype Donor[25];
struct genotype Acceptor[24];
struct genotype Bridge[17];
struct genotype Anchor[15];
struct genotype solution[pop]; // [100] need to be changed according to the size of the population.
struct genotype solutionPheno[pop];
struct genotype tempsolution[pop];



//make single charactor string into 2 charactor string, for example string "2" into "02"
string makeString2(int a){
        string b;
        if(to_string(a).length() == 1){
                b = "0"+to_string(a);
        } else {
                b = to_string(a);
        }

        return b;
}


string makeString(int a){
        string b;
        if(to_string(a).length() == 2){
                b = "0"+to_string(a);
        } else if (to_string(a).length() == 1){
                b = "00"+to_string(a);
        } else {
                b = to_string(a);
        }

        return b;
}



void Mutate(string &bits)
{

    for (int i=0; i < 10; i=i+2)
    {
	double RANDOM_NUM= ((double)rand()/(RAND_MAX));
	RANDOM_NUM= ((double)rand()/(RAND_MAX));
        if (RANDOM_NUM < MUTATION_RATE){
            if (i==0) {
                int randDonorID = rand()%25;
                cout << randDonorID <<  endl;
                string str;
                str = makeString2(randDonorID);
                cout << str<<endl;
                bits.replace(i,2,str);
            }
        }
        RANDOM_NUM= ((double)rand()/(RAND_MAX));
        if (RANDOM_NUM < MUTATION_RATE){
        if (i==2) {

            int randBridgeID = rand()%17;
            cout << randBridgeID <<  endl;
            string str;
            str = makeString2(randBridgeID);
            cout << str<<endl;
            bits.replace(i,2,str);
        }
        }
	RANDOM_NUM= ((double)rand()/(RAND_MAX));
        if (RANDOM_NUM < MUTATION_RATE){
        if (i==4) {

            int randAcceptorID = rand()%24;
            cout << randAcceptorID <<  endl;
            string str;
            str = makeString2(randAcceptorID);
            cout << str<<endl;
            bits.replace(i,2,str);
        }
        }
	RANDOM_NUM= ((double)rand()/(RAND_MAX));
        if (RANDOM_NUM < MUTATION_RATE){
        if (i==6) {

            int randBridgeID = rand()%17;
            cout << randBridgeID <<  endl;
            string str;
            str = makeString2(randBridgeID);
            cout << str<<endl;
            bits.replace(i,2,str);
        }
        }
	RANDOM_NUM= ((double)rand()/(RAND_MAX));
        if (RANDOM_NUM < MUTATION_RATE){
        if (i==8) {

            int randAnchorID = rand()%15;
            cout << randAnchorID <<  endl;
            string str;
            str = makeString2(randAnchorID);
            cout << str<<endl;
            bits.replace(i,2,str);
        }
        }      
        
    }
}



void Crossover(string &offspring1, string &offspring2){

	double RANDOM_NUM= ((double)rand()/(RAND_MAX));
        RANDOM_NUM= ((double)rand()/(RAND_MAX));
	if (RANDOM_NUM < CROSSOVER_RATE){
		int crossover = 1;
		while (crossover%2 !=0) {  // in this case, there are 2 charactors in any block building
		crossover = rand()%7+2;    // get a random number between 2 and 8
		}
		cout << crossover<<endl;
 
		string t1 = offspring1.substr(0, crossover) + offspring2.substr(crossover, CHROMO_LENGTH-crossover);
		string t2 = offspring2.substr(0, crossover) + offspring1.substr(crossover, CHROMO_LENGTH-crossover);
 
		offspring1 = t1; offspring2 = t2;                 
	}
}

// select parents.
string Roulette(double total_fitness, genotype* Population){

	
        double RANDOM_NUM= ((double)rand()/(RAND_MAX));
	RANDOM_NUM= ((double)rand()/(RAND_MAX));
	double Slice = RANDOM_NUM * total_fitness;
	float FitnessSoFar = 0.0f;
	for (int i=0; i<pop; i++){
		FitnessSoFar += Population[i].Fitness;
		if (FitnessSoFar >= Slice){
		return Population[i].Name;
		}
	}
}

// change smile code "1223060215"  to"ccc1ccn2....." 
genotype solution2Pheno(genotype a){
	genotype b;
	string asub="";
        int c;
        for(int i = 0; i < a.Name.length(); i = i+2 ){
        asub = a.Name.substr(i,2);
        c = atoi(asub.c_str());
	if (i == 0) {
		b.Name += Donor[c].Name;
		cout << "donor is " <<  Donor[c].Name << endl;
	}
        if (i == 2) {
		b.Name += Bridge[c].Name;
                cout << "bridge is " << Bridge[c].Name << endl;
	}
        if (i == 4) {
		b.Name += Acceptor[c].Name;
                cout << "acceptor is " << Acceptor[c].Name << endl;
	}
        if (i == 6) {
		b.Name += Bridge[c].Name;
                cout << "bridge is " << Bridge[c].Name << endl;
	}
        if (i == 8) {
		b.Name += Anchor[c].Name;
                cout << "anchor is " <<Anchor[c].Name << endl;
	}
        }
	return b;
}


// make run.sh for every calculation.
void makeRunsh(){
	ofstream myfile;
        ofstream myfileIP;
        ofstream myfileEA;
        ofstream myfileGAS;
        ofstream myfileIPS;
	ofstream myfileABS;
	ofstream myfileWFN;
        ofstream myfileAutodock;


	string str;
	str = "run_OPT.sh";
	myfile.open(str);
        str = "run_IP.sh";
        myfileIP.open(str);
        str = "run_IPS.sh";
        myfileIPS.open(str);
        str = "run_EA.sh";
        myfileEA.open(str);
        str = "run_GAS.sh";
        myfileGAS.open(str);
        str = "run_ABS.sh";
        myfileABS.open(str);
        str = "run_WFN.sh";
        myfileWFN.open(str);
        str = "run_Autodock.sh";
        myfileAutodock.open(str);


	myfile << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J opt \n" << "#SBATCH -p    cooper  -n 1\n"<< "#SBATCH -N 1    \n" << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
        myfileIPS << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J ips \n" << "#SBATCH -p cooper  -n 1\n"<< "#SBATCH -N 1  \n" << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
        myfileIP << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J IP \n" << "#SBATCH -p  cooper   -n 1\n"<< "#SBATCH -N 1  \n"  << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
        myfileEA << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J EA \n" << "#SBATCH -p cooper     -n 1\n"<< "#SBATCH -N 1  \n"  << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
        myfileGAS << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J GAS \n" << "#SBATCH -p cooper  -n 1\n"<< "#SBATCH -N 1   \n"  << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
        myfileABS << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J ABS \n" << "#SBATCH -p cooper  -n 1\n"<< "#SBATCH -N 1   \n"  << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
        myfileWFN << "#!/bin/bash -l\n" <<  "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n"  << "#SBATCH -o std.out\n" << "#SBATCH -J WFN \n" << "#SBATCH -p cooper  -n 1\n"<< "#SBATCH -N 1   \n"  << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;
  myfileAutodock << "#!/bin/bash -l\n"<< "#SBATCH -D ./\n"<<  "#SBATCH --export=ALL\n" << "#SBATCH -o std.out\n" << "#SBATCH -J docking \n" << "#SBATCH -p cooper  -n 1\n"<< "#SBATCH -N 1   \n"  << "#SBATCH -t 1-00:00:00\n" << "hostname\n" ;


	myfile <<"export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;
        myfileIP <<" export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;
        myfileEA <<"export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;
        myfileGAS <<"export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;
        myfileIPS <<" export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;
        myfileABS <<" export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;
        myfileWFN <<" export XTBHOME=\"~/Xtb5/xtb_exe/ \" \n export MKL_NUM_THREADS=1 \n export OMP_THREAD_LIMIT=1 \n export OMP_NUM_THREADS=${OMP_THREAD_LIMIT},1 \n export OMP_STACKSIZE=500m \n  ulimit -s unlimited \n" ;

        myfile    << " ~/Xtb5/xtb_exe/xtb 1.xyz -opt -gfn3 --gbsa h2o -h  > 1.outOPT \n";
        myfileIP  << " ~/Xtb5/xtb_exe/xtb xtbopt.xyz -gfn1 -vip  --gbsa h2o  > 1.outIP \n";
        myfileEA  << " ~/Xtb5/xtb_exe/xtb xtbopt.xyz -gfn1 -vea  --gbsa h2o  > 1.outEA \n";
        myfileGAS << " ~/Xtb5/xtb_exe/xtb xtbopt.xyz -gfn3 -opt                       > 1.outGAS \n" ;
        myfileIPS  << " ~/Xtb5/xtb_exe/xtb xtbopt.xyz -gfn1 -vip   > 1.outIPS \n";
        myfileWFN  << " ~/Xtb5/xtb_exe/xtb xtbopt.xyz -h --gbsa h2o   > 1.outWFN \n";
        myfileABS  << " ~/Xtb5/xtb_exe/stda_1.6 -xtb -e 7   > 1.outABS \n";
        myfileAutodock  << "   ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_receptor4.py -r protein.pdb \n ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_ligand4.py -l ligand.pdb \n ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_gpf4.py -l ligand.pdbqt -r protein.pdbqt  -y \n ~/AutoDock/autogrid4 -p protein.gpf \n ~/Mgl/mgltools_x86_64Linux2_1.5.6/bin/pythonsh ~/Mgl/mgltools_x86_64Linux2_1.5.6/MGLToolsPckgs/AutoDockTools/Utilities24/prepare_dpf4.py -l ligand.pdbqt  -r protein.pdbqt \n sed -i 's/2500000/2500000/g' ligand_protein.dpf  \n sed -i 's/ga_run 10/ga_run 7/g' ligand_protein.dpf \n  ~/AutoDock/autodock4  -p ligand_protein.dpf -l scoring_result.log \n   " ;
	
	myfile.close();
        myfileIP.close();
        myfileEA.close();
        myfileGAS.close();
        myfileIPS.close();
        myfileWFN.close();
        myfileABS.close();
        myfileAutodock.close();

}


//run Gaussian calculation for nutural, positive, and negative molecules.
void runXtbOPT(int i){
	string str="";
	str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
	const char *xtbrun1 = str.c_str();
	system(xtbrun1);
	str = "echo \" sbatch run_OPT.sh \" >> "+ makeString(i)+".e";
	const char *xtbrun2 = str.c_str();
	system(xtbrun2);
	str = "echo \" cd .. \" >> "+ makeString(i)+".e";
	const char *xtbrun3 = str.c_str();
	system(xtbrun3);
	str = "chmod a+x "+ makeString(i)+".e";
	const char *xtbrun4 = str.c_str();
	system(xtbrun4);
	str = "./"+ makeString(i)+".e";
	const char *xtbrun5 = str.c_str();
	system(xtbrun5);
}

void runXtbIPS(int i){
        string str="";
        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" sbatch run_IPS.sh \" >> "+ makeString(i)+".e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+".e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+".e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+".e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}

void runXtbIP(int i){
        string str="";
        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" sbatch run_IP.sh \" >> "+ makeString(i)+".e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+".e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+".e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+".e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}

void runXtbEA(int i){
        string str="";
        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" sbatch run_EA.sh \" >> "+ makeString(i)+".e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+".e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+".e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+".e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}

void runXtbWFN(int i){
        string str="";
        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" sbatch run_WFN.sh \" >> "+ makeString(i)+".e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+".e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+".e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+".e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}

void runXtbABS(int i){
        string str="";
        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" sbatch run_ABS.sh \" >> "+ makeString(i)+".e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+".e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+".e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+".e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}


//run xtb calculation for nutural molecules without solvents.
void runXtbGAS(int i){
        string str="";
        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+".e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" sbatch run_GAS.sh \" >> "+ makeString(i)+".e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+".e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+".e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+".e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}

//run autodock
void runAutodock(int i){
        string str="";

        str = "echo \" cd "+ makeString(i) + "\" > "+ makeString(i)+"AD.e";
        const char *xtbrun1 = str.c_str();
        system(xtbrun1);
        str = "echo \" obabel -ixyz xtbopt.xyz -O ligand.pdb \" >> " + makeString(i)+"AD.e";
        const char *xtbrun0 = str.c_str();
        system(xtbrun0);
        str = "echo \" sbatch run_Autodock.sh \" >> "+ makeString(i)+"AD.e";
        const char *xtbrun2 = str.c_str();
        system(xtbrun2);
        str = "echo \" cd .. \" >> "+ makeString(i)+"AD.e";
        const char *xtbrun3 = str.c_str();
        system(xtbrun3);
        str = "chmod a+x "+ makeString(i)+"AD.e";
        const char *xtbrun4 = str.c_str();
        system(xtbrun4);
        str = "./"+ makeString(i)+"AD.e";
        const char *xtbrun5 = str.c_str();
        system(xtbrun5);
}


void makeSolution() {
    int TotalNumber;
    int MaxLength=0;


//************ READ IN DONOR, ACCEPTOR, BRIDGE, ANCHOR THEN MAKE 50 RANDOM COMBINATION*********
//
//
//
   ifstream infile; 

// read in Donor
//
//
   infile.open("DonorList"); 
 
   infile >> TotalNumber; 

    for (int i = 0; i < TotalNumber; i++) {
        infile >> Donor[i].ID >> Donor[i].Name;
        cout <<  Donor[i].ID  << "  " << Donor[i].Name<< ' ' << Donor[i].Name.length()<< '\n';
	if (MaxLength < Donor[i].Name.length()){
	MaxLength = Donor[i].Name.length();
	}
    }

   infile.close();

// read in Acceptor
//
//
   infile.open("AcceptorList");

   infile >> TotalNumber;

    for (int i = 0; i < TotalNumber; i++) {
        infile >> Acceptor[i].ID >> Acceptor[i].Name;
        cout <<  Acceptor[i].ID  << "  " << Acceptor[i].Name<< ' ' << Acceptor[i].Name.length()<< '\n';
        if (MaxLength < Acceptor[i].Name.length()){
        MaxLength = Acceptor[i].Name.length();
	}
    }

   infile.close();

// read in Bridge 
// 
// 

   infile.open("BridgeList");

   infile >> TotalNumber;

    for (int i = 0; i < TotalNumber; i++) {
        infile >> Bridge[i].ID >> Bridge[i].Name;
        cout <<  Bridge[i].ID  << "  " << Bridge[i].Name<< ' ' << Bridge[i].Name.length()<< '\n';
        if (MaxLength < Bridge[i].Name.length()){
        MaxLength = Bridge[i].Name.length();
	}
    }

   infile.close();

// read in Anchor
// 
//
   infile.open("AnchorList");

   infile >> TotalNumber;

    for (int i = 0; i < TotalNumber; i++) {
        infile >> Anchor[i].ID >> Anchor[i].Name;
        cout <<  Anchor[i].ID  << "  " << Anchor[i].Name<< ' ' << Anchor[i].Name.length()<<'\n';
        if (MaxLength < Anchor[i].Name.length()){
        MaxLength = Anchor[i].Name.length();
	}
    }

   infile.close();

	cout << MaxLength << endl;

//************   end of     READ IN DONOR, ACCEPTOR, BRIDGE, ANCHOR THEN MAKE 50 RANDOM COMBINATION*********

////---------------------------------------------------

/////*******start of making solution part********
// generate 50 random solution
  

  for(int i=0; i < pop; i++){

	double skip11 = rand();

  int randDonorID = rand()%25;
  cout << randDonorID <<  endl;

  int randAcceptorID = rand()%24;
  cout << randAcceptorID << endl;

  int randBridgeOneID = rand()%17;
  cout << randBridgeOneID <<endl;

  int randBridgeTwoID = rand()%17;
  cout << randBridgeTwoID << endl;

  int randAnchorID = rand()%15;
  cout << randAnchorID << endl;

  solution[i].ID = i;
  solution[i].Name = makeString2(randDonorID)+makeString2(randBridgeOneID)+makeString2(randAcceptorID)+makeString2(randBridgeTwoID)+makeString2(randAnchorID);
  cout << "solution is " << solution[i].Name << endl;
  cout << solution[i].Name.length() << endl;
  }


// make population(solution list) write into file.
//      ofstream myfile;
//      myfile.open("solutions");
//      cout << "solution list is" << endl;
//      for (int i = 0 ; i< pop ; i++){
//      	cout << i << "   " << solution[i].Name<<endl;
//      	myfile << solution[i].Name<<endl;
//      }
//      myfile.close();
} // end of makeSolution()

//-**************   MAIN START  ****************--------------------------------------

int main(){
	
    std::time_t result = std::time(nullptr);
    std::cout << std::asctime(std::localtime(&result));


	srand(time(NULL));
	double  RANDOM_NUM= ((double)rand()/(RAND_MAX));
	makeSolution();  //***ATTENTION!! if you restart GA after crash, comment this line,or it will start from begining !

	int GenerationsRequiredToFindASolution = 0;
	bool bFound = false;
	double TotalFitnessP=100000.0 ;

	ifstream infile;
// **************   Enter GA loop  *******************



while(!bFound){


     // read in solutions from saved file  and  geno to pheno
	infile.open("solutions");
	for (int i = 0; i < pop; i++){
		infile >> solution[i].Name ;
	}
	infile.close();	
	for (int i = 0; i < pop; i++){
		solutionPheno[i] = solution2Pheno(solution[i]);
		cout << solutionPheno[i].Name<<"_______"<<endl;
	}



/////********* make xyz coordinates using openbabel*******


    for (int i = 0; i < pop; i++){
    ofstream myfile;
    string a = makeString(i)+".smi";
    string b = makeString(i)+".xyz";
    myfile.open(a);
    myfile << solutionPheno[i].Name<<endl;
    myfile.close();
    
    string strRun = "obabel "+ a +"  -O "+ b + " --gen3d --conformer --nconf 50 --score energy";
    const char *obabelCommand = strRun.c_str();
    system(obabelCommand);
    }

	string str;
        for (int i = 0 ; i<pop; i++){
                str = "mkdir "+ makeString(i) ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }


      double x,y,z;
      int TotalAtomNumber;
      char skipLine[2];
      string ElementSymble;
      string b;

	// make up all the output files for Gaussian before it runs just in case there is no output file for WAITing check
	for (int i = 0 ; i<pop; i++){
		str = "echo \" This is some text\" > "+ makeString(i)+"/1.outOPT " ;
		const char *CreatTempFile1 = str.c_str();
		system(CreatTempFile1);
	}
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/1.outIP " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/1.outEA " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/1.outGAS " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/1.outIPS " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/1.outWFN " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/1.outABS " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }
        for (int i = 0 ; i<pop; i++){
                str = "echo \" This is some text\" > "+ makeString(i)+"/scoring_result.log " ;
                const char *CreatTempFile1 = str.c_str();
                system(CreatTempFile1);
        }

	//submit xtb jobs
	makeRunsh();
	for (int i = 0 ; i<pop; i++){ 
		str = "cp run_IPS.sh run_OPT.sh run_IP.sh run_GAS.sh run_EA.sh run_WFN.sh run_ABS.sh run_Autodock.sh protein.pdb " + makeString(i);
                const char *cpRunIn = str.c_str();
                system(cpRunIn);
		str = "cp "+  makeString(i)+".xyz " +  makeString(i)+"/1.xyz";
                const char *cpXYZin = str.c_str();
                system(cpXYZin);
		runXtbOPT(i);

	}

	// WAIT  until all the optimization calculation of to finish
	
	ifstream data_store;
	bool finished = false;
	int count = 0;
        string line;
        string NormalTarget="cpu  time for all";

	while (!finished) {
		str="grep \"cpu  time for all\" */1.outOPT > countfile";  
        	const char *runstr = str.c_str(); 
		system(runstr);
		
		data_store.open("countfile");

		count = 0;
		while(!data_store.eof()){
			getline(data_store,line);
			if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
				count++;
			}
		}
		if (count ==pop) {  
			finished = true; 
		}

		data_store.close();
	}


	//END of wait 


        for (int i = 0 ; i<pop; i++){
                runXtbIPS(i);
        }

	//wait IPS to finish
        finished = false;
        count = 0;

        while (!finished) {
                str="grep \"cpu  time for all\" */1.outIPS > countfile";
                const char *runstr = str.c_str();
                system(runstr);

                data_store.open("countfile");

                count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                if (count ==pop) {
                        finished = true;
                }

                data_store.close();
        }
	//end of IPS





        for (int i = 0 ; i<pop; i++){
                runXtbIP(i);
        }


	//wait IP calculation finish
        finished = false;
        count = 0;

        while (!finished) {
		str="grep \"cpu  time for all\" */1.outIP > countfile";
                const char *runstr = str.c_str();
                system(runstr);

                data_store.open("countfile");

                count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                if (count ==pop) {
                        finished = true;
                }

                data_store.close();
        }
	//END OF WAIT

        for (int i = 0 ; i<pop; i++){
                runXtbEA(i);
        }
	//wait EA calculation finish

        finished = false;
        count = 0;

        while (!finished) {
                str="grep \"cpu  time for all\" */1.outEA > countfile";
                const char *runstr = str.c_str();
                system(runstr);

                data_store.open("countfile");

                count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                if (count ==pop) {
                        finished = true;
                }

                data_store.close();
        }
        //END OF WAIT

	///EA IP fitness calculation
		str="grep \"delta SCC IP (eV)\" */1.outIP > IPdata"; //depends on what software you are using
	        const char *runIP = str.c_str();
	        system(runIP);
		
		ifstream IPfile;
		IPfile.open("IPdata");
		int pos;
		string strValue;
		double Energy=0.0;

	for (int i = 0 ; i<pop; i++){
			getline(IPfile,line);
			pos = line.find(")");
			strValue = line.substr(pos+2,10);
			Energy = atof(strValue.c_str());
		solution[i].IP = 0.3577*(Energy-4.44)-0.0974 ; //this correction is based on 30 points data
	}
		IPfile.close();

                str="grep \"delta SCC EA (eV)\" */1.outEA > EAdata"; //depends on what software you are using
                const char *runEA = str.c_str();
                system(runEA);

                ifstream EAfile;
                EAfile.open("EAdata");

        for (int i = 0 ; i<pop; i++){
                        getline(EAfile,line);
                        pos = line.find(")");
                        strValue = line.substr(pos+2,10);
                        Energy = atof(strValue.c_str());
                solution[i].EA =  0.731*(Energy-4.44)-0.9288  ; //this correction is based on 30 points data
                solution[i].GAP = solution[i].IP - solution[i].EA;
                cout << solution[i].Name << " Ea= " << solution[i].EA << " IP=" << solution[i].IP << " GAP=" <<solution[i].GAP<<endl;
        }
                EAfile.close();

//	run absorption
        for (int i = 0 ; i<pop; i++){
		//add IP EA restriction and copy fake absorption file into folder which is out of IP EA restraction
                runXtbWFN(i);
        }

        finished = false;
        count = 0;

        while (!finished) {
                str="grep \"cpu  time for all\" */1.outWFN > countfile";
                const char *runstr = str.c_str();
                system(runstr);

                data_store.open("countfile");

                count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                if (count ==pop) {
                        finished = true;
                }

                data_store.close();
        }

        for (int i = 0 ; i<pop; i++){
                runXtbABS(i);
        }

        finished = false;
        count = 0;
	NormalTarget = "sTDA done";
        while (!finished) {
                str="grep \"sTDA done.\" */1.outABS > countfile";
                const char *runstr = str.c_str();
                system(runstr);
        
                data_store.open("countfile");
                
                count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                if (count ==pop) {
                        finished = true;
                }

                data_store.close();
        }


//      absorption end
//

                str="grep -A 1 \"state    eV\" */1.outABS > data"; //depends on what software you are using
                const char *runabs = str.c_str();
                system(runabs);

		//depends on what software you are using
                str="sed \"/stat/d\" data | sed \"/--/d\" | sed \"s/\\/1.outABS-    1//\" > data1";
                const char *runabs1 = str.c_str();
                system(runabs1);

	        infile.open("data1");
	      	int num;
	      	str="";
		double F1abs;
	      	for (int i = 0; i < pop; i++){
	      	        infile >> num  >> F1abs ;
			solution[i].Absp=F1abs*0.9604-0.0858;
	      	        for(int j = 0; j < 12 ; j++){
	      	                infile >> str ;  //skip the rest colum
	      	        }
	      	        cout << solution[i].Name << "   " <<  solution[i].Absp << "   "  <<  endl;
	      	}
	      	infile.close();


        for (int i = 0 ; i<pop; i++){
                        if (solution[i].IP > 0.20 && solution[i].EA < -1.55 &&  solution[i].GAP < 2.0 && solution[i].Absp < 2.70 ) {  // need to be change
                                cout << solution[i].ID << " " << solution[i].Name << " " << solution[i].IP << " " <<  solution[i].EA ;
                                 cout << " " <<  solution[i].GAP << " " << solution[i].Absp << " "  << endl;
                }
        }



	cout << "***** Starting run Esol***" << endl;
	//Run Esol calculation

	for (int i = 0 ; i<pop; i++){

                if (solution[i].IP > 0.2 && solution[i].EA < -1.55 &&  solution[i].GAP < 2.0 && solution[i].Absp < 2.70 ) {  // need to be change
                        cout << solution[i].ID << " " << solution[i].Name << " " << solution[i].IP << " " <<  solution[i].EA ;
                        cout << " " <<  solution[i].GAP << " " << solution[i].Absp << " " << endl;
                        runXtbGAS(i);
                } else {
                        str = "cp " + makeString(i) + "/1.outOPT " + makeString(i) + "/1.outGAS"  ;
                        const char *CreatTempFile4 = str.c_str();
                        system(CreatTempFile4);
                }
 
	}
	cout <<"---"<<endl;

	//wait calculation finish
	
        finished = false;

        while (!finished) {

		NormalTarget="cpu  time for all";
                str="grep  \"cpu  time for all\" */1.outGAS > countfile";
                const char *runstr = str.c_str();
                system(runstr);

                data_store.open("countfile");
 
                int count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                if (count == pop) {  
                	finished = true;
                }
                data_store.close();
        }

	//Calculation Esol

		
                str="tail -n 100 */1.outGAS | grep \"total E\"  > E0Data";  //depends on your software
                const char *runstr2 = str.c_str();
                system(runstr2);

                str="tail -n 100 */1.outOPT | grep \"total E\"  > EsData";
                const char *runstr1 = str.c_str();
                system(runstr1);

                ifstream myfile0;
                myfile0.open("E0Data");
		ifstream myfilesol;
		myfilesol.open("EsData");

                pos=0;
                strValue="";
                double E0, Es;


        for (int i = 0 ; i<pop; i++){
                getline(myfile0,line);
                pos = line.find(":");
                strValue = line.substr(pos+1,16);
                E0 = atof(strValue.c_str());
		getline(myfilesol,line);
                pos = line.find(":");
                strValue = line.substr(pos+1,16);
                Es = atof(strValue.c_str());
		solution[i].Esol = (Es-E0)*27.2113;
                cout << solution[i].Name << " Esol= " << solution[i].Esol <<endl;
        }

                myfile0.close();
                myfilesol.close();

//binding energy calculation

	int NumberGoodIPEA=0;
        for (int i = 0 ; i<pop; i++){

//		if (solution[i].IP> 0.30 && solution[i].EA < -1.45 &&  solution[i].GAP < 4.2 && solution[i].Absp < 2.90 && solution[i].Esol < 0.0 ) {
		if ( solution[i].Esol < 0.0 && solution[i].IP > 0.20 && solution[i].EA < -1.55 &&  solution[i].GAP < 2.0 && solution[i].Absp < 2.70 ) {
			cout << solution[i].ID << " " << solution[i].Name << " " << solution[i].IP << " " <<  solution[i].EA ;
			cout << " " <<  solution[i].GAP << " " << solution[i].Absp << " " <<  solution[i].Esol << endl;
                        runAutodock(i);
                        NumberGoodIPEA++;
                        solution[i].goodIPEA = true;
                } else {
                        str = "cp fakescoring_result.log " + makeString(i) + "/scoring_result.log"  ; //don't forget to make or copy fakescoring, binding energy in fakescoring.log should be 0.0
                        const char *CreatTempFile4 = str.c_str();
                        system(CreatTempFile4);
                }

        }

//wait binding energy calculation finish
//
        finished = false;

        while (!finished) {

                NormalTarget="finished";
                str="grep  \"finished\" */scoring_result.log > countfile";
                const char *runstr = str.c_str();
                system(runstr);

                data_store.open("countfile");

                int count = 0;
                while(!data_store.eof()){
                        getline(data_store,line);
                        if (strstr(line.c_str(),NormalTarget.c_str())!= NULL){
                                count++;
                        }
                }
                        if (count == pop) {
                                finished = true;
                        }
                data_store.close();
        }


//calculate binding energy
//
        double TotalFitness = 0.0;


                str="grep -A 8  \"RMSD TABLE\" */scoring_result.log | grep \"RANKING\"  > bindingEnergy";
                const char *runstrbinding = str.c_str();
                system(runstrbinding);

                ifstream myfilebinding;
                myfilebinding.open("bindingEnergy");

                pos=0;
                strValue="";
                double Ebinding;



		cout << "Fitness**********************" << endl;

        for (int i = 0 ; i<pop; i++){
                getline(myfilebinding,line);
                pos = line.find("1      1");
                strValue = line.substr(pos+18,10);
                solution[i].Binding = atof(strValue.c_str());

		if (solution[i].Binding < -0.0) {
	                solution[i].Fitness = abs(3.0-solution[i].Absp); /// Fitness score = - Binding energy 
		} else { 
			solution[i].Fitness = 0.0;
		}

		cout << solution[i].Name << " Fitness Binding = " << solution[i].Binding << " Fitness absorption = " << solution[i].Absp << "  " ;


		if (-1.0*solution[i].Binding > 0.0) {
	                TotalFitness +=solution[i].Fitness;
		} else {
			solution[i].Fitness = 0.0;
		}


                cout << solution[i].Name << "  " << solution[i].Fitness<<endl ;
        }



		myfilebinding.close();
	
		cout << "TotalFitness = " << TotalFitness << "    ";

	double maxfit = 0.0;
	string elitism= "";
	for (int i = 0 ; i<pop; i++){
		if (maxfit < solution[i].Fitness) {
			maxfit = solution[i].Fitness;
			elitism = solution[i].Name;
		}
	}
	       cout << "best fitness = " << maxfit<< "   best one is " << elitism << endl;
	

        int cPop = 0;
	while (cPop < pop){

                string offspring1 = Roulette(TotalFitness, solution);
                string offspring2 = Roulette(TotalFitness, solution);


		cout << "parent 1 is " << offspring1 << endl;
		cout << "parent 2 is " << offspring2 << endl;
		
		cout << "before crossover" << endl;
                cout << offspring1 << endl;
                cout << offspring2 << endl;

		Crossover(offspring1, offspring2);

		cout << "after crossover" << endl;
		cout << offspring1 << endl;
		cout << offspring2 << endl;

		Mutate(offspring1);
		Mutate(offspring2); 

                cout << "after mutate" << endl;
                cout << offspring1 << endl;
                cout << offspring2 << endl;


		tempsolution[cPop] = genotype(cPop,offspring1, 0.0f,10.0,0.0);
		cPop++;
		tempsolution[cPop] = genotype(cPop,offspring2, 0.0f,10.0,0.0);
		cPop++;
	}

	ofstream myfile;
	myfile.open("solutions");

	for (int i=0; i<pop; i++){
		solution[i] = tempsolution[i];
	}
	
	solution[0].Name = elitism;
	
        for (int i=0; i<pop; i++){
                myfile << solution[i].Name << endl;
        }


	myfile.close();
 
	++GenerationsRequiredToFindASolution;

	if (abs(TotalFitness-TotalFitnessP) < 0.01){ 
		bFound = true;
		cout << "CONVERGED" << endl;
	} else {
		TotalFitnessP = TotalFitness;
	}

        if (GenerationsRequiredToFindASolution >= MAX_ALLOWABLE_GENERATIONS) {
                bFound = true;
                cout << "MAX_ALLOWABLE_GENERATIONS RESAERCHED" << endl;
        }


	myfile.open("logfile");
	myfile << "it is a generatio of " << GenerationsRequiredToFindASolution << "#" << "total fitness is " << TotalFitness << endl;
        myfile.close();
	
} //end of  while bfound


      std::time_t resultend = std::time(nullptr);
     std::cout << std::asctime(std::localtime(&resultend));

	return 0;
}





