//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"


//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;
static int player_nr;

#define MAX_GRADE       9

typedef struct player
{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
	int labTime;
} player_t;

static player_t *cur_player;
//static player_t cur_player[MAX_PLAYER];

#if 0 
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
void generatePlayers(int n, int initEnergy); //generate a new player
void printGrades(int player); //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif


int isGraduated(int player) //check if any player is graduated
{
	if (cur_player[player].position == 0 && cur_player[player].accumCredit >= GRADUATE_CREDIT) {
		// 'flag_graduate = 1' means 'can graduate'
		cur_player[player].flag_graduate = 1;
	}
	
	else {
		cur_player[player].flag_graduate = 0;
	}
	
	return cur_player[player].flag_graduate;
}


void generatePlayers (int n, int initEnergy) //generate a new player
{
	int i;
	
	// n time loop
	for(i=0; i<n; i++)
	{
		//input name
		printf("Input player %i's name:", i);
		//scanf("%s", player_name[i]);
		scanf("%s", cur_player[i].name);		
		fflush(stdin);
		
		// set position
		// player_position[i] = 0;
		cur_player[i].position = 0;
		
		// set energy
		// player_energy[i] = initEnergy;
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit = 0;
		cur_player[i].flag_graduate = 0;
	}
}


void printPlayerStatus(void) //print all player status at the beginning of each turn
{
	int i;
	
	printf("\n\n");
	printf("============================ PLAYER STATUS ============================\n");
		
	for (i=0; i<player_nr; i++)
	{
		void* boardObj = smmdb_getData(LISTNO_NODE, cur_player[i].position);
		
		printf("%s: credit %i, energy %i, position %i\n",
					cur_player[i].name,
					cur_player[i].accumCredit,
					cur_player[i].energy,
					cur_player[i].position);
	}
	
	printf("=======================================================================");
	printf("\n\n");
}


float calcAverageGrade(int player) //calculate average grade of the player
{
	
}

int rolldie(int player)
{
    char c;
    printf("Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
   	if (c == 'g')
    	printGrades(player);
#endif
    
    return (rand() % MAX_DIE + 1);
}


int getRandFoodEnergy() {
	int randFood = (rand() % food_nr);
	return randFood;
}


int getRandFestCard() {
	int randCard = (rand() % festival_nr);
	return randCard;
}


smmObjGrade_e getRandGrade() {
	return (smmObjGrade_e) (rand() % MAX_GRADE);
}


//action code when a player stays at a node
void actionNode(int player)
{
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
		//void *foodPtr = smmdb_getData(LISTNO_FOODCARD, cur_food[player].position);
	//int type = smmObj_getNodeType(cur_player[player].position);
	int type = smmObj_getNodeType( boardPtr );
	char *name = smmObj_getNodeName( boardPtr );
	void *gradePtr;
	int inLabTime;
		
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
        	// if energy is bigger than 0, decrease energy
        	if (cur_player[player].energy > 0) {
				cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr );
        		cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr );
			}
			
        	// grade generation
			gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit( boardPtr ), 0, getRandGrade());
			
			// 'smmdb_addTail' : adding grade information to the database
			smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
        	break;
        	
        	
        case SMMNODE_TYPE_RESTAURANT:
			printf("Player %s arrived at the restaurant and charge 7 energies. (Remain energy: %i)",
					cur_player[player].name, cur_player[player].energy + 7);
			break;
			
			
		case SMMNODE_TYPE_LABORATORY:
			// 1. Check if player is experimenting
			// 2. If player is not experimenting, let the player go through
			
			// 1-1. 'inLabTime == 1' means that the player is experimenting.
				// 1-1(1) roll the dice
				// 1-1(2) end the experiment if the number exceeds the specified standard
				// 1-1(3) otherwise stay in the experimental state
			if (inLabTime == 1) {
				printf("Experiment time! Let's see if you can satisfy professor (threshold: 3)");
				getchar();
				
				// 1-1(1) The player's dice result
				int dieResult = rolldie(player);
			
				// 1-1(2)(3) Compare dice result to standard vaule
					// Succeed to escape from a lab
				if (dieResult >= EXP_THRESHOLD) {
					inLabTime = 0;
					printf("  -> Experiment result : %i, Success! %s can exit this lab!",
							dieResult, cur_player[player]);
				}
			
					// Fail to escape from a lab - Repeat
				else {
					printf("  -> Experiment result : %i, Fail... %s needs more experiment.",
							dieResult, cur_player[player]);
				}
			}
			
			// 2. Let the player go through (Not in an experimental state)
			else {
				printf("It is not experiment time. You can go through this lab.");
			}
			break;	
		
		
		case SMMNODE_TYPE_HOME:
			// Add as much supplemental energy as specified to the current energy
			// specified HOME energy : 18
			printf("returned to HOME! energy charged by 18 (total : %i)", cur_player[player].energy + 18);
			break;
		
			
		case SMMNODE_TYPE_GOTOLAB:
			printf("OMG! This is experiment time!! Player %s goes to the lab.", cur_player[player]);
			
			// turn into an experimental state
			inLabTime = 1;
			break;
	
				
        case SMMNODE_TYPE_FOODCHANCE:
        	// 1. Let the player pick the food card
        	printf("Player %s gets a food chance! press any key to pick a food card: ", cur_player[player].name);
        	getchar();
        	
        	// 2. Add energy of randomly selected food to existing energy
			int randFoodIndex = getRandFoodEnergy();	
        	cur_player[player].energy += smmObj_getFoodEnergy(randFoodIndex);
        	
        	// 3. print the name, charge of selected food
        	printf("Selected food: %s, Energy: %i\n",
					smmObj_getFoodName(randFoodIndex), smmObj_getFoodEnergy(randFoodIndex));
        	break;
    	
		
		case SMMNODE_TYPE_FESTIVAL:
			// 1. Let the player pick the festival card
			printf("  -> %s participates to Snow Festival! Press any key to pick a festival card: ",
					cur_player[player].name);
			getchar();
        	
        	// 2. Show the festival cards randomly
        	int randCardIndex = getRandFestCard();
        	//printf("Your mission is %s", smmObj_getFestivalCard(getRandFestCard));
			
					
        default:
            break;
    }
}

void goForward(int player, int step) //make player go "step" steps on the board (check if player is graduated)
{
	void *boardPtr;
	boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	cur_player[player].position += step;
	
	printf("%s go to node %i (name: %s)\n",
				cur_player[player].name,
				cur_player[player].position,
				smmObj_getNodeName(boardPtr));
}


int printGrades(int player) //print grade history of the player
{
	int i;
	void* gradePtr;

	
	for (i=0; i<smmdb_len(LISTNO_OFFSET_GRADE + player); i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n",
				smmObj_getNodeName(gradePtr),
				smmObj_getNodeGrade(gradePtr));
	}
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn = 0;
    void* boardObj;
    void* foodObj;
    void* festivalObj;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
    	//store the parameter set
		//(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
    	boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
		smmdb_addTail( LISTNO_NODE, boardObj );
    	// submit information of 'addTail' in 'smmObj_genObject'(int list_nr, void* obj); 
		
		if (type == SMMNODE_TYPE_HOME)
    		initEnergy = energy;
        board_nr++;
	}
    fclose(fp);
        
    for (i=0; i<board_nr; i++)
    {
    	boardObj = smmdb_getData(LISTNO_NODE, i);
    	
    	printf("=> node %i: %s, %i(%s) credit: %i, energy: %i\n",
				i,
				smmObj_getNodeName(boardObj),
				smmObj_getNodeType(boardObj),
				smmObj_getTypeName(smmObj_getNodeType(boardObj)),
				smmObj_getNodeCredit(boardObj),
				smmObj_getNodeEnergy(boardObj));
	}
	printf("Total number of board nodes : %i\n", board_nr);
	
	 
	//1-2. food card config
    // error handling
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        foodObj = smmObj_genObject(name, smmObjType_food, SMMNODE_TYPE_FOODCHANCE, 0, energy, 0);
		smmdb_addTail(LISTNO_FOODCARD, foodObj);
		        
		food_nr++;
    }
    fclose(fp);
    
    for (i=0; i<food_nr; i++)
    {
    	printf("=> %i: %s, charge: %i\n",
				i,
				smmObj_getNodeName(foodObj),
				smmObj_getNodeEnergy(foodObj));
	}
    printf("Total number of food cards : %i\n", food_nr);
    
    
    //1-3. festival card config
    // error handling
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
        //store the parameter set
        festivalObj = smmObj_genObject(name, smmObjType_card, SMMNODE_TYPE_FESTIVAL, 0, 0, 0);
        smmdb_addTail(LISTNO_FESTCARD, festivalObj);
        
		festival_nr++;  	
    }
    fclose(fp);
    
    for (i=0; i<festival_nr; i++)
    {
    	festivalObj = smmdb_getData(LISTNO_FESTCARD, i);
    	
    	printf("node %i : %s\n",
				i,
				smmObj_getNodeName(festivalObj));
	}
    
    printf("Total number of festival cards : %i\n", festival_nr);
    
    printf("\n\n\n\n");
    printf("=======================================================================\n");
	printf("-----------------------------------------------------------------------\n");
    printf("       Sookmyung Marble !! Let's Graduate (total credit : 30)!!       \n");
    printf("-----------------------------------------------------------------------\n");
    printf("=======================================================================\n");
    printf("\n\n\n\n");
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        //input player number to player_nr
        printf("\nInput player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr > MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    
    
    generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1) % player_nr;
    }
    
    free(cur_player);
    system("PAUSE");
	return 0;
}
