//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>


/*
char smm_node_name[MAX_NODE][MAX_CHARNAME];
int smm_node_type[MAX_NODE];
int smm_node_credit[MAX_NODE];
int smm_node_energy[MAX_NODE];
*/


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
    "lecture",
    "restaurant",
    "laboratory",
    "home",
    "gotoLab",
    "foodChance",
    "festival"
};

/*
static char smmFoodName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"치킨",
	"맥주",
	"탕후루",
	"컵라면",
	"감자탕",
	"컵밥",
	"짜장면",
	"학식",
	"파스타",
	"피자",
	"햄버거",
	"샌드위치",
	"요거트",
	"비빔밥"	
};
// already defined in marbleFoodConfig.txt
*/


static char smmGradeName[MAX_GRADE][3] = {
	"Ap",
    "A0",
    "Am",
    "Bp",
    "B0",
    "Bm",
    "Cp",
    "C0",
    "Cm"
};


float smmGradeVaule[MAX_GRADE] = {
	4.3, 4.0, 3.7, 3.3, 3.0, 2.7, 2.3, 2.0, 1.7 };
//   A+,  A0,  A-,  B+,  B0,  B-,  C+,  C0,  C-


char* smmObj_getTypeName(int type)
{
    return (char*)smmNodeName[type];
}


// 1. Define Structual Formats
	// 1-1. object struct
typedef struct smmObject
{
	char name[MAX_CHARNAME];
	smmObjType_e objType; //enum 형식이라는 뜻 
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t; //객체라는 구조체 이름으로 만듦 

	// 1-2. food struct
typedef struct smmFood
{
	char name[MAX_CHARNAME];
	int energy;
} smmFood_t;

	// 1-3. festival struct
typedef struct smmFestival
{
	char card[MAX_CHARNAME];
} smmFestCard_t;


// 2. 구조체 배열 변수 정의
//static int smmObj_noNode = 0;

// 3. change related functions
// 3-1. object generation about player
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{
	smmObject_t* ptr;
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy (ptr->name, name);
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
    
    
    #if 0
	struct boardnode smmObj_genNode;
	strcpy(smmObj_name[smmObj_noNode], name);
    smmObj_type[smmObj_noNode] = type;
    smmObj_credit[smmObj_noNode] = credit;
    smmObj_energy[smmObj_noNode] = energy;
    smmObj_noNode++;
    #endif
}

// 3-2. object generation about food
void* smmObj_genFood(char* name, int energy)
{
	// memory allocating
	smmFood_t* foodPtr;
    foodPtr = (smmFood_t*)malloc(sizeof(smmFood_t));
    
    strcpy (foodPtr->name, name);
    foodPtr->energy = energy;
    
    return foodPtr;
}

// 3-3. object generation about festival
void* smmObj_genFestival(char* card)
{
	// memory allocating
	smmFestCard_t* festivalPtr;
    festivalPtr = (smmFestCard_t*)malloc(sizeof(smmFestCard_t));
    
    strcpy (festivalPtr->card, card);
    
    return festivalPtr;
}

// 3. 관련 함수 변경 
char* smmObj_getNodeName(void* obj)
{
	smmObject_t* ptr = (smmObject_t*) obj;
	return ptr->name;
}

int smmObj_getNodeType(void* obj) //int node_nr -> void* obj
{
	smmObject_t* ptr = (smmObject_t*) obj;
	return ptr->type;
}

int smmObj_getNodeCredit(void* obj)
{
	smmObject_t* ptr = (smmObject_t*) obj;
	return ptr->credit;	
}

int smmObj_getNodeEnergy(void* obj)
{
	smmObject_t* ptr = (smmObject_t*) obj;
	return ptr->energy;
}

smmObjGrade_e smmObj_getNodeGrade(void* obj)
{
	smmObject_t* ptr = (smmObject_t*) obj;	
    return ptr->grade;
}


// Food
char* smmObj_getFoodName(void* obj)
{
	smmFood_t* foodPtr = (smmFood_t*)obj;
	return foodPtr->name;
}


int smmObj_getFoodEnergy(void* obj)
{
    smmFood_t* foodPtr = (smmFood_t*)obj;
	return foodPtr->energy;
}

#if 0
// Festival
char* smmObj_getFestivalCard(void* obj) {
	smmCard_t* festivalPtr = (smmCard_t*)obj;
	return festivalPtr->name;
}
#endif

#if 0
//member retrieving
//element to string
char* smmObj_getNodeName(smmNode_e type)
{
    return smmNodeName[type];
}
#endif


