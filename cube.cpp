#include <iostream>
#include <fstream> //Not sure why this is here. Maybe to print all the cubes to a file.
#include <vector>
#include <set>			/*  This is better than vector.
							With a set, I can quickly check for duplicates.
*/
#include <stdlib.h>		// not sure what this is for
#include <time.h>		// Used to initialize the random seed.
char M[]={
0,21,2,23,6,4,7,5,19,9,17,11,12,13,14,15,16,1,18,3,20,10,22,8,
0,10,2,8,7,6,5,4,3,9,1,11,12,13,14,15,16,21,18,23,20,17,22,19,
0,17,2,19,5,7,4,6,23,9,21,11,12,13,14,15,16,10,18,8,20,1,22,3,
4,5,2,3,8,9,6,7,12,13,10,11,0,1,14,15,18,16,19,17,20,21,22,23,
8,9,2,3,12,13,6,7,0,1,10,11,4,5,14,15,19,18,17,16,20,21,22,23,
12,13,2,3,0,1,6,7,4,5,10,11,8,9,14,15,17,19,16,18,20,21,22,23,
2,0,3,1,18,5,19,7,8,9,10,11,12,20,14,21,16,17,15,13,6,4,22,23,
3,2,1,0,15,5,13,7,8,9,10,11,12,6,14,4,16,17,21,20,19,18,22,23,
1,3,0,2,21,5,20,7,8,9,10,11,12,19,14,18,16,17,4,6,13,15,22,23};
/*	An array that
	maps the new location of each sticker for every possible twist.
	The cube is described as a char array with 24 chars for the stickers, and 11 more chars for the move list to generate it.
*/
#define cubedatatypesize 35
#define prevmoveindex (22+depth)	// read as previous move's index
// #define break_on_find 1

using namespace std;

class cube{
	public:
		cube(){// default constructor. makes a solved cube.
			char solved[]="ggggwwwwbbbbyyyyoooorrrr";/*
				This is the color scheme of standard cubes, I think.
				green white blue yellow going couterclockwise, orange on top, and red on the bottom.*/
			for (int i=0;i<24;i++)
				c[i]=solved[i];
			c[24]=9;//9 is the move list terminator. Since the solved cube has zero twists necessary to generate it, the 9 goes immediately after the stickers.
		}
		cube(char n[]){// copy constructor
			for (int i=0;i<cubedatatypesize;i++)
				c[i]=n[i];
		}
		cube(const cube &x, char i,char depth){// Twist constructor. Parameters are a cube and a integer code that instructs which twist to make. depth is in here too for simplicity.
			char* z=M+i*24;//align z to the twist we need.
			int p=0;
			for (;p<24;p++)
				c[p]=x.c[z[p]];//twist
			for (;p<23+depth;p++)
				c[p]=x.c[p];//copy twist list
			c[p++]=i;//write down latest twist
			c[p]=9;//write down twist terminator
		}
		cube(int numtwists){//random cube constructor
			char solved[]="ggggwwwwbbbbyyyyoooorrrr";
			for (int i=0;i<24;i++)
				c[i]=solved[i];
			c[24]=9;
			for (int i=1;i<=numtwists;i++)
				randtwist(i);
		}
		int findlmi(){//find last move. deprecated
			int lmi=24;
			for (;c[lmi]!=9 and lmi<cubedatatypesize;lmi++);
			return lmi-1;
		}
		void randtwist(int depth){//Add random twist
			int m;
			if (depth==1)
				m=rand()%9;
			else{
				int lmr=c[depth+22]/3;// don't apply a twist in the same axis twice in a row
				m=rand()%6;
				if (lmr==0) m+=3;
				else if (lmr==1 and m>2) m+=3;
			}
			cube b(*this,m,depth);
			int i=0;
			for (;i<24+depth and i<cubedatatypesize;i++)
				c[i]=b.c[i];
			if (i!=cubedatatypesize) c[i]=9;
		}
		void twistchain(char* n){
			int i=0;
			for (;n[i]!=0;i++);
			cout<<i<<endl;
			for (int j=1;j<=i;j++){
				cube b(*this,n[j-1]-48,j);
				int k=0;
				for (;b.c[k]!=9 and k<cubedatatypesize;k++)
					c[k]=b.c[k];
				if (k!=cubedatatypesize) c[k]=9;
			}
		}
		cube& operator=(const cube &rhs){//cube assignment operator
			for (int i=0;i<cubedatatypesize;i++)
				c[i]=rhs.c[i];
				return *this;
		}
		bool operator<(const cube &rhs) const{// This is necessary to get the Set to work. Sets require sortability because it uses a binary tree.
			for (int i=0;i<24;i++)
				if (c[i]==rhs.c[i]) continue;
				else return(c[i]<rhs.c[i]);
			return 0;
		}
		bool operator==(const cube &rhs){// Only compares the stickers not the moves to get there. This is necessary for removing duplicates.
			for (int i=0;i<24;i++)
				if (c[i]!=rhs.c[i])	return 0;
			return 1;
		}
		void print(){	print(findlmi()-23);	}
		void printinv(){	printinv(findlmi()-23);	}
		
		void printmovelist(int depth){
			char m[]="R  R2 R' U  U2 U' F  F2 F' ";
			for (int i=24;i<24+depth;i++)
				cout<<m[3*c[i]]<<m[3*c[i]+1]<<m[3*c[i]+2];
			cout<<endl;
		}
		void printmovelistinv(int depth){
			char m[]="R' R2 R  U' U2 U  F' F2 F  ";// I thought for 5 minutes about how to mathematically calculate the inverse until I realized the simpler solution.
			for (int i=24+depth;i>=24;i--){
				
				cout<<m[3*c[i]]<<m[3*c[i]+1]<<m[3*c[i]+2];
			}
			cout<<endl;
		}
		void print(int depth){
			for (int i=0;i<24;i++)
				cout<<c[i];
			cout<<":  ";
			printmovelist(depth);
		}
		void printinv(int depth){
			for (int i=0;i<24;i++)
				cout<<c[i];
			cout<<":  ";
			printmovelistinv(depth);
		}
		void write(int depth, ofstream &out){
			for (int i=0;i<24;i++)
				out<<c[i];
			out<<"  ";
			char m[]="R  R2 R' U  U2 U' F  F2 F' ";
			for (int i=24;i<24+depth;i++)
				out<<m[3*c[i]]<<m[3*c[i]+1]<<m[3*c[i]+2];
			out<<endl;
		}
			
		char c[cubedatatypesize];
};	

bool findinpyr(cube &scrambled, cube &solution, vector < set<cube> > &pyr){//RETURN ATTENTION HERE
	std::set<cube>::iterator it;
	for (int i=0;i<pyr.size();i++){
		it = pyr[i].find(scrambled);
//		cube testme=*it;
		if (scrambled==*it){
			solution=*it;
			return 1;}
	}
	return 0;
}
bool isunique(cube &c, vector < set<cube> > &pyr, int highestindex){
	for (int i=0;i<=highestindex;i++){
		if (pyr[i].count(c)!=0) return 0;
	}
	return 1;
}
int expandcubeset (const cube &x,int &j, int &depth, vector < set<cube> > &pyr){
	cube b(x,j,depth);
	if (isunique(b,pyr,depth)){
		pyr[depth].insert(b); 
	}
	return 0;
}
vector < set<cube> >  buildsetpyramid(){ 
	cube start;
	vector < set<cube> > pyr;
	for (int i=0;i<=11;i++)
		pyr.push_back(set<cube>());
	int depth=0;
	pyr[depth].insert(cube());
	cout<<"depth "<<depth<<" has "<<pyr[depth].size()<<" element\n";
	
	depth=1;
	for (int i=0;i<9;i++){	//Don't skip any axis because there was no previous move.
		cube b(start,i,depth);
		pyr[depth].insert(b);
	}
	cout<<"depth "<<depth<<" has "<<pyr[depth].size()<<" elements\n";
	
	depth = 2;
	for (;depth<=11;depth++){
		for (set<cube>::iterator i=pyr[depth-1].begin(); i!=pyr[depth-1].end(); i++){	// Iterate through all cubes in the lower depth
			int skip = (*i).c[22+depth]-(*i).c[22+depth]%3;		// Find out which axis was previously twisted
			int j=0;
			cube x=*i;
			for (;j<skip;j++)
				expandcubeset(x,j,depth,pyr);
			for (j+=3;j<9;j++)
				expandcubeset(x,j,depth,pyr);
		}
	cout<<"depth "<<depth<<" has "<<pyr[depth].size()<<" elements\n";
	}
	return pyr;
}
int main(){
	vector < set<cube> > pyr=buildsetpyramid();
	char userinput[]="This is the user input buffer";
	cout<<"Enter a cube to solve, or 0 to quit:\n";
	cin>>userinput;
	while (userinput[0]!='0'){
		cube scrambled(userinput);
		cube solution;
		if (findinpyr(scrambled, solution,pyr)){
			solution.printinv();
		}
		cin>>userinput;
	} 
	
	char x;
	cin >> x;
}
