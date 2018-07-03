
//* Program for the game of life problem *

#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

#ifdef _MSC_VER
// Microsoft compiler specific memory leak detection tool
#include <crtdbg.h>
#endif

// fills the 2D array double with a specified value
void showWorld(const vector<vector<char> > &V);

// copies the contents of the 1st 2D array to the 2nd 2D array
void copyWorld(const vector<vector<char> > &SRC, vector<vector<char> > &DST);

// calculates whether one cell should be dead or alive in next iteration
char calcCell(char oldcell, char neighborcount);

// calculates the new world based on the old world
void calcWorld(const vector<vector<char> > &OLD, vector<vector<char> > &NEW);

// counts the number of differences between values in two arrays
bool compWorld(const vector<vector<char> > &ONE, const vector<vector<char> > &TWO);

// counts how manyu cells are alive (i.e. !=0)
size_t statWorld(vector<vector<char> > &V);


// initial fill and functions for random generator
void initializeWorld(vector<vector<char> > &V);
void RandomInit();
char RandomGet();

int main()
{
	{
		size_t rows, cols;
		do {
			cout << "Please enter... ";
			cin >> rows >> cols;
		} while (rows<5 || cols<5);
		cin.ignore(256, '\n'); // gets rid of <ENTER> from keyboard buffer

							   //declares three 2D vectors of <char> rows by cols elements
		vector<vector<char> > ol2world(rows, vector<char>(cols));
		vector<vector<char> > oldworld(rows, vector<char>(cols));
		vector<vector<char> > curworld(rows, vector<char>(cols));

		// creates a random initial world
		initializeWorld(curworld);
		//        showWorld(curworld);
		//        cout << endl;

		size_t generation = 0;
		size_t alivecount = 0;
		bool stableworld;
		char ch;
		do {
			// copys the old state as an older state
			// so that a longer history can be preserved
			copyWorld(oldworld, ol2world);
			// copys the current state as an old state
			// so that a new current state can be calculated
			copyWorld(curworld, oldworld);
			//			showWorld(oldworld);
			//        	cout << endl;
			// calculates the next time step in the simulated world
			// and stores into the "current state" variable by overwriting old values
			calcWorld(oldworld, curworld);
			generation++;

			// computee some statistics
			alivecount = statWorld(curworld);

			// show the current simulation state
			cout << "GAME OF LIFE: generation:" << setw(5) << generation
				<< "          populatiuon: " << setw(5) << alivecount << endl;
			showWorld(curworld);
			cout << endl;

			// chack if situationchanges in the simulation
			stableworld = (alivecount == 0) || compWorld(curworld, oldworld) || compWorld(curworld, ol2world);
			// note: some states are cyclic with more than 2 states, such cases are not detected

			if (stableworld) cout << "Press <ENTER> to quit. The simulation is in stable state." << endl;
			else             cout << "Press <ENTER> to continue OR no <ENTER> to quit..." << endl;

			// wait for a user to look at it and press enter or something and enter
			cin.get(ch);
			// if it was something and enter then clear the characters that remin in keyboard buffer
			if (ch != '\n') cin.ignore(256, '\n');

		} while (!stableworld && ch != 'n');
		cout << "Simulation terminated." << endl;

		// destructs the vectors to reclaim dynamic memory
		ol2world.clear();
		oldworld.clear();
		curworld.clear();
	}

#ifdef _MSC_VER
	if (_CrtDumpMemoryLeaks())
		cerr << "Memory leaks occurred!" << endl;
	else    cerr << "No memory leaks detected!" << endl;
#else
	cerr << "Memory leaks detection unavailable!" << endl;
#endif

	return(0);
}



//=======================================================
// this function prints the contents of the array A
// using " " and "x" one row per line without extra spaces
// - if the array element is zero then a space is printed
// - if the array element is nonzero then "x" is printed
void showWorld(const vector<vector<char> > &V)
{
	for (size_t r = 0; r<V.size(); r++) {
		for (size_t c = 0; c<V[r].size(); c++) {
			if (V[r][c] == 0) {
				cout << ' ';
			}
			else
				cout << 'x';
		}
		cout << endl;
	}
}



//=======================================================
// this function copies elements of array SRC to array DST
void copyWorld(const vector<vector<char> > &SRC, vector<vector<char> > &DST)
{
	for (size_t r = 0; r<SRC.size(); r++) {
		for (size_t c = 0; c<SRC[r].size(); c++)
			DST[r][c] = SRC[r][c];
	}
}



//=======================================================
// this function calculates the value for the new cell based on value of the old cell
// and the numebr of nonzero neighboring cells calculated prior by you and passed to it
// it is suggested to use it inside of calcWorld function
char calcCell(char oldcell, char neighborcount)
{
	const static char lookup[2][10] = { { 0,0,0,1,0,0,0,0,0,0 },{ 0,0,1,1,0,0,0,0,0,0 } };
	cout << "My old cell is ->" << oldcell << endl;
	cout << "My neighbor is ->" << neighborcount << endl;
	cout << "calcCell value is ->" << lookup[oldcell != 0][neighborcount] << endl;
	return(lookup[oldcell != 0][neighborcount]);

	/*
	if (oldcell!=0)
	if (neighborcount>=2 && neighborcount<=3)
	return(1);
	else
	return(0);
	else
	if (neighborcount==3)
	return(1);
	else
	return(0);
	*/
}


//=======================================================
// this function performs one step of life simulation
// use OLD for the current state of the world
// use NEW to store the new state of the world
// according to the description given in your textbook
// - assume that the value of zero means an empty cell
// - assume that the value of one  means alive cell
// - set the cell value to zero when setting the cell as alive
void calcWorld(const vector<vector<char> > &OLD, vector<vector<char> > &NEW)
{
	for (size_t r = 1; r<OLD.size(); r++) {
		for (size_t c = 1; c<OLD[r].size(); c++) {
			int count = 0;
			//			count = OLD[c-1][r] +
			//					OLD[c-1][r-1] +
			//					OLD[c][r-1] +
			//					OLD[c+1][r-1] +
			//					OLD[c+1][r] +
			//					OLD[c+1][r+1] + 
			//					OLD[c][r+1] + 
			//					OLD[c-1][r+1];
			//			NEW[r][c] = calcCell(OLD[r][c], count);
			for (int a = -1; a<2; a++) {
				for (int b = -1; b<2; b++) {
					if (!(a == 0 && b == 0)) {
						cout << "My current row is ->" << r << endl;
						cout << "My current cols is ->" << c << endl;
						cout << "My new row is ->" << r + a << endl;
						cout << "My new col is ->" << c + b << endl;
						if (r + a < OLD.size() && c + b < OLD[r].size()) {
							cout << "My neighbour value is ->" << OLD[r + a][c + b] << endl;
							if (OLD[r + a][c + b] != 0) {
								++count;
							}
						}
					}
				}
			}
			NEW[r][c] = calcCell(OLD[r][c], count);
		}
	}
}

//=======================================================
// this function returns true of the content of two arrays is identical,
// otehrwise returns false on first difference spotted
bool compWorld(const vector<vector<char> > &ONE, const vector<vector<char> > &TWO)
{
	int check = 0;
	for (size_t r = 0; r<ONE.size(); r++) {
		for (size_t c = 0; c<ONE[r].size(); c++)
			if (ONE[r][c] != TWO[r][c]) {
				check++;
				break;
			}
	}
	if (check > 0)
		return false;
	else
		return true;
}

//=======================================================
// this function returns the number of nonzero elements
// in the portion of the array A speciofied by rows and cols
size_t statWorld(vector<vector<char> > &V)
{
	size_t count = 0;
	for (size_t r = 0; r<V.size(); r++) {
		for (size_t c = 0; c<V[r].size(); c++)
			if (V[r][c] != 0) {
				count++;
			}
	}
	return(count);
}



//=======================================================
// this function initializes contents of the array A
// using random number generator and certain probablility. 
void initializeWorld(vector<vector<char> > &V)
{
	RandomInit();
	for (size_t r = 0; r<V.size(); r++)
		for (size_t c = 0; c<V[r].size(); c++)
			V[r][c] = RandomGet();
}

void RandomInit() {
	// current time in seconds since 1/1/1980
	time_t TimeInSeconds; time(&TimeInSeconds);
	// set the random numebr generator variable to it
	srand(static_cast<unsigned int>(TimeInSeconds));
}

char RandomGet() {
	// if(rand()%3==0) return(1) else return(0);
	return(static_cast<char>(rand() % 3 == 0));
}
