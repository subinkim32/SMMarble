//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_NODE        100

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

#if 0 
static char smmFoodName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"ġŲ",
	"����",
	"���ķ�",
	"�Ŷ��",
	"������",
	"�Ź�",
	"¥���",
	"�н�",
	"�Ľ�Ÿ",
	"����",
	"�ܹ���",
	"������ġ",
	"���Ʈ",
	"�����"	
};
#endif


#if 0
static char smmGradeName[MAX_GRADE] = {
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
// ����ü? 2���� �迭? �̿��� ���ĺ� ������ ������ �´� ���� ����
// �̰� ��� ����ؾ� �ϴ���(after lecture, with adding credit), �̰� ��� �����ϰ� ����س� ������ �˾ƾ� ��


typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;
#endif


char* smmObj_getTypeName(int type)
{
    return (char*)smmNodeName[type];
}


// 1. Define Structual Formats
	// 1-1. object struct
typedef struct smmObject
{
	char name[MAX_CHARNAME];
	smmObjType_e objType; //enum �����̶�� �� 
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
} smmObject_t; //��ü��� ����ü �̸����� ���� 

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


// 2. ����ü �迭 ���� ����
//static smmObject_t smm_node[MAX_NODE];, �迭�� �ƴ϶� database.c�� linked list�� ������ �ְԲ� �� ��. 
//static int smmObj_noNode = 0;

// 3. change related functions
// 3-1. object generation about board? node? player?
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

#if 0
// 3-2. object generation about food
void smmObj_food(char* name, int charge)
{
	strcpy (smm_node[smmObj_noNode].name, name);
    smm_node[smmObj_noNode].type = type;
    smm_node[smmObj_noNode].credit = credit;
    smm_node[smmObj_noNode].energy = energy;
	
	//ctrlc + ctrlv part
	smmObject_t* ptr;
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy (ptr->name, name);
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}
#endif

// 3. ���� �Լ� ���� 
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

char* smmObj_getGradeName(smmGrade_e grade)
{
    return smmGradeName[grade];
}
#endif

