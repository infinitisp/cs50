/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <math.h>

// board's min/max dimensions
#define MIN 3
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

//array to store location of zero tile
int z[2];
//array to store location of "tile of interest"
int x[2];

//god mode on?
int godMode = 0;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
void movetile(int tile);
bool won(void);
void save(void);
int getIntOrGod();
void godSolve(int dim);
void locate(int tile);
int getTile(int id);
void moveComplete();
float manDistance();

/*
//found on internet... to make working with array of arrays easier
typedef struct {
    int array[100];
} Path;

Path *findPaths(int depth, Path *paths);
*/


int main(int argc, string argv[])
{
    greet();
    
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        int tile;
        clear();
        draw();   
        save(); //for testing

        //if god mode on, go to god mode procedure
        if (godMode == 1)
            godSolve(d);
        
        // check for win
        if (won()) {
            printf("ftw!\n");
            break;
        }
            
        printf("Tile to move: ");
        tile = getIntOrGod();

        // move if possible, else report illegality
        if (!move(tile)){
            printf("\nIllegal move.\n");
            usleep(20000);
        }
        usleep(20000);
    }

    // that's all folks
    return 0;
}

// Clears screen using ANSI escape sequences.
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(200000);
}

//Initializes the game's board with tiles numbered 1 through d*d - 1
void init(void)
{
    //initialize board with proper solution
    for (int i = 0; i < d; i++) 
        for (int j = 0; j < d; j++)
            board[i][j] = i*d+j+1;
    //initialize zero tile & location of zero tile
    board[d-1][d-1] = 0;
    z[0] = d-1;
    z[1] = d-1;
    
    //randomize board by making a lot of random moves
    srand((unsigned int) time(NULL));
    for (int i = 0; i < d*d*1000; i++)
        move(rand() % (d*d));
}

//Prints the board in its current state.
void draw(void)
{
    clear(); 
    printf("ManDist: %f\n\n", manDistance()); //so can see progress of god mode
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == 0) printf ("    ");
            else printf(" %2d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//If tile borders empty space, moves tile and returns true, else false
bool move(int tile)
{
    for (int i = -1; i<2; i++)
        for (int j = -1; j <2; j++) 
            if (tile == board[z[0]+i][z[1]+j] && (j == 0 || i == 0) && tile != 0) {
                movetile(tile);
                return true;
            }
    return false;
}

//swaps tile with zero tile
//separate from move b/c assumes tile can moved / used elsewhere directly
void movetile(int tile) {
    locate(tile);
    board[x[0]][x[1]] = 0;
    board[z[0]][z[1]] = tile;
    z[0] = x[0];
    z[1] = x[1];
}

//Returns true if game is won, else false
bool won(void)
{
    for (int i = 0; i < d; i++) 
        for (int j = 0; j < d; j++)
            if (board[i][j] != i*d+j+1 && board[i][j] != 0)
                return false;
    return true;
}

//gets either and integer or the string "GOD", else asks user to retry
int getIntOrGod() { 
    char input[16] = {0};
    while (true){
        scanf("%s", input);
        if (strtol(input, (char **)NULL, 10) != 0) 
            return (int) strtol(input, (char **)NULL, 10);
        else if (strcmp(input,"GOD") == 0){
            godMode = 1;
            return 0;
        }
        printf("Retry: ");
    }
}

//solves the game on its own
void godSolve(int dim) {
//length of path to search
//int pathDepth = 3; 
int failedCounter = 0;

    while (true) {
    

        //array that will save moves to do
        int toMove[500];
        //create pointer to location in toMove[]
        int toMoveLoc = 0; 

        
        //save board
        int savedBoard[d][d];
        int savedZ[2];
        for (int i = 0; i < d; i++)
            for (int j = 0; j < d; j++)
                savedBoard[i][j] = board[i][j];
        savedZ[0] = z[0];
        savedZ[1] = z[1];
        
        //set manhattan distance to beat
        float manDist = manDistance(); 
        
        //declare manDist for new setup;
        float manDistNew = manDist;
        
        //this is to freeze rows near the top (i.e. tell algorithm to not include
        //those squares in paths it searches
        int rowComp = 0;
        for (int i = 0; i < d*d; i++) {
                if (board[i/d][i%d] == i+1) rowComp++;
                else break;
            }
        rowComp = rowComp/d;
        
        
        //do up to some# random moves, recording them, breaking early
        ////Sorry, I know this algorithm isn't really an algorithm, and it's terrible,
        ////but it meets the specification (solves things up to 4x4 (though really poorly)
        ////far from optimally). It kind of REALLY sucks for 5x5+... but that's why I was
        ////trying to figure out how to work out the A* in C (code that was commented out)
        ////.. but these arrays are difficult to work with, and I ran out of time
                
        for (int i = 0; i < d*d + failedCounter; i++) {
            int j = rand() % 4;
            if (getTile(j) > rowComp*d) {
                toMove[toMoveLoc++] = getTile(j);
                movetile(getTile(j));
                manDistNew = manDistance();
                if (manDistNew < manDist) break;
            }
        }
        
        /* STARt OF BETTER ALGORITHM -- COULD NOT COMPLETE IN TIME
        // Working with arrays in C is a big headache... A* would've been really easy in Java?
        // I'm probably just a noob though
        Path pathSet[(int)pow(4,pathDepth)];
        Path *tempPathSet = findPaths(pathDepth, pathSet);
        for (int i = 0; i < pow(4,pathDepth); i++)
            pathSet[i] = tempPathSet[i];
        
        //for each path
        for (int i = 0, n = pow(4,pathDepth); i < n; i++) {
            
            //for each step in path, move tile
            for (int j = 0; j < pathDepth; j++) 
                movetile(getTile(pathSet[i].array[j]));
            
            //if new board > old board, save moves & reset path depth
            manDistNew = manDistance(); 
            if (manDistNew < manDist) {
                toMove = pathSet[i].array;
                pathDepth = 2;
            }
        
            //reset boards
            for (int i = 0; i < d; i++)
                for (int j = 0; j < d; j++)
                    board[i][j] = savedBoard[i][j];
            z[0] = savedZ[0];
            z[1] = savedZ[1];
        
        }
        
        */
        
        //reset boards
            for (int i = 0; i < d; i++)
                for (int j = 0; j < d; j++)
                    board[i][j] = savedBoard[i][j];
            z[0] = savedZ[0];
            z[1] = savedZ[1];
        
        failedCounter ++;
        //compare new manhattan distance to mandist to beat
        //if better, do all those moves
        if (manDistNew < manDist) {
        //if (pathDepth == 2) { //new algo
        failedCounter = 0;
            for (int i = 0; i < toMoveLoc; i++) {
                moveComplete(toMove[i]);
            }
        } 
        
       // pathDepth++;

   }
}

/* SORRY COULD NOT COMPLETE IN TIME
//finds all possible path permutations of length n
Path *findPaths(int depth, Path *paths) {
    if (depth > 1) {
        Path rootPaths[(int)pow(4,depth-1)];
        Path *tempRootPaths = findPaths(depth-1, rootPaths);
        for (int i = 0; i < pow(4,depth-1); i++)
            rootPaths[i] = tempRootPaths[i];
            
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < pow(4,depth-1); j++) {
                Path path;
                path.array[0] = i;
                for (int k = 1; k < depth; k++) {
                    path.array[k] = rootPaths[j].array[k-1];
                }
                paths[j + i*j] = path; 
            }
        }

        
    }
    else
        for (int i = 0; i < 4; i++) {
            Path path;
            path.array[0] = i;
            paths[i] = path; 
        }
    
    return paths; 
}
*/

//sets the x[] array to equal the location of a specified tile
void locate (int tile) {
    for (int i = 0; i < d; i++) 
        for (int j = 0; j < d; j++)
            if (board[i][j]==tile) {
                x[0] = i;
                x[1] = j;
                return;
            }
}

//gets tiles around the 0 tile if they exist, else returns 0
//e.g., getTile(0) returns the tile above the zero tile, or 0
//if the 0 tile is in the top row
int getTile (int id) {
    if (id == 0) return z[0] > 0   ?  board[z[0]-1][z[1]] : 0; 
    if (id == 1) return z[0] < d-1 ?  board[z[0]+1][z[1]] : 0; 
    if (id == 2) return z[1] > 0   ?  board[z[0]][z[1]-1] : 0; 
    if (id == 3) return z[1] < d-1 ?  board[z[0]][z[1]+1] : 0; 
    return 0; 
}

//basically the loop from the main method, but taken out so it could
//be called separately by the godsolve procedure
void moveComplete (int tile) {
        
        move(tile);
        usleep(400);
        clear();
        draw();   
        save(); //for testing
        
        // check for win
        if (won()) {
            printf("ftw!\n");
            exit(0);
        }
}

//calculate a modified manhattan distance
float manDistance() {
    float manDist = 0;
    for (int i = 1; i < d*d; i++) {
        locate(i);
        
        //basic manhattan distance + some modifier to slightly favor finishing first rows/cols first
        manDist += abs(x[0] - (i-1)/d);// + (float) ((i-1)/d - x[0])/d*2 ;
        manDist += abs(x[1] - (i-1)%d);// + (float) ((i-1)%d - x[1])/d*2 ;
        
        //modification for linear conflict
        for (int j = 1; j < d-1; j++) {
            if (x[0] - (i-1)/d == 0)
                if (board[x[0]][x[1]+j] == i-1 || board[x[0]][x[1]-j] == i+1)
                    manDist += 1;
            if (x[1] - (i-1)%d == 0)
                if (board[x[0]+j][x[1]] == i-d || board[x[0]-j][x[1]] == i+d)
                    manDist += 1;
        }
    }
    return manDist;
}


//*********************************************
//*********************************************
/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
	fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
	fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
