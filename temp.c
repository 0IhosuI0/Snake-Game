#include <stdio.h>	//표준입출력헤더
#include <stdlib.h>	//malloc, free 등
#include <Windows.h>//windows console api 헤더
#include <time.h>	//랜덤 시드용 time 함수 헤더
#include <conio.h>	//gotoxy에서 사용할 api 헤더

#define FIELD_WIDTH 70	//필드가로길이
#define FIELD_HEIGHT 25	//필드세로길이
#define MENU_WIDTH 83
#define MENU_HEIGHT 40
#define LEFT 75			//키보드 좌 화살표의 char값
#define RIGHT 77		//키보드 우
#define UP 72			//키보드 위
#define DOWN 80			//지보드 아래
#define ITEM_MAX 4	//화면에 표시되는 아이템개수
#define ITEM_GOLD 101	//골드 아이템 인디케이터
#define ITEM_EXP 102	//경험치 아이템 인디케이터
#define LEFT_MARGIN 30	//화면왼쪽마진(공백)
#define TOP_MARGIN 10	//화면 상단마진(공백)
//#define DELAYTIME 100	//Sleep함수에 들어갈 x/1000 초

int DELAYTIME = 100;
int NowSpeed = 0;
int Select = 0;
int cntTail = 9;
int score = 0;

//지렁이를 구현할 이중연결리스트 구조체
#pragma pack(push,1)
typedef struct _WORM
{
	int x;	//x좌표
	int y;	//y좌표
	char direction;	//진행방향
	struct _WORM *next;	// 다음노드주소
	struct _WORM *before;//이전노드주소
}WORM, *pWORM, *rWORM;

//아이템을 구현할 단일연결리스트 구조체
typedef struct _ITEM
{
	int x;
	int y;
	int itemType;
	int itemNo;
	int renditem;
	char data;
	char ITEMt;
	struct _ITEM* next;
}ITEM, *pITEM;
#pragma pack(pop)

void DeleteCursor(){
	CONSOLE_CURSOR_INFO infocursor = {0, };
	infocursor.dwSize = 1;
	infocursor.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &infocursor);
}

//커서를 일정 좌표로 이동
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x + LEFT_MARGIN;//가로세로 비율을 맞추기 위해서 가로의 좌표는 2배
	Pos.Y = y + TOP_MARGIN;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void SetColor(int num){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}

//게임영역출력
void PrintField() 
{
	for (int i = 1; i < FIELD_WIDTH; i++)
	{
		gotoxy(i, 0);
		printf("─");
		gotoxy(i, FIELD_HEIGHT);
		printf("─");
	}

	for (int i = 1; i < FIELD_HEIGHT; i++)
	{
		
		gotoxy(0, i);
		printf("│");
		gotoxy(FIELD_WIDTH,i);
		printf("│");
	}
	gotoxy(0, 0);
	printf("┌");
	gotoxy(0, FIELD_HEIGHT);
	printf("└");
	gotoxy(FIELD_WIDTH, 0);
	printf("┐");
	gotoxy(FIELD_WIDTH, FIELD_HEIGHT);
	printf("┘");
}
//메인화면 생성
void PrintMenu() 
{
	for (int i = 13; i < MENU_WIDTH; i++)
	{
		gotoxy(i, 15);
		printf("■");
		gotoxy(i, MENU_HEIGHT);
		printf("■");
	}

	for (int i = 15	; i < MENU_HEIGHT; i++)
	{
		gotoxy(13, i);
		printf("■");
		gotoxy(MENU_WIDTH, i);
		printf("■");
	}
	gotoxy(13, 15);
	printf("■");
	gotoxy(13, MENU_HEIGHT);
	printf("■");
	gotoxy(MENU_WIDTH, 15);
	printf("■");
	gotoxy(MENU_WIDTH, MENU_HEIGHT);
	printf("■");
	gotoxy(0, 0);
	printf("    //   ) )                                                  //   ) )                                     ");
	gotoxy(0, 1);
	printf("   ((             __        ___       / ___      ___         //            ___        _   __        ___    ");
	gotoxy(0, 2);
	printf("     \\        //   ) )   //   ) )   //\\ \\     //___) )     //  ____     //   ) )   // ) )  ) )   //___) ) ");
	gotoxy(0, 3);
	printf("       ) )    //   / /   //   / /   //  \\ \\   //           //    / /    //   / /   // / /  / /   //        ");
	gotoxy(0, 4);
	printf("((___ / /    //   / /   ((___( (   //    \\ \\ ((____       ((____/ /    ((___( (   // / /  / /   ((____     ");

	gotoxy(44, 20);
	printf("새로  하기");
	gotoxy(44, 25);
	printf("이어  하기");
	gotoxy(44, 30);
	printf("게임  설명");
	gotoxy(44, 35);
	printf("종료  하기");
}
//키보드 화살표를 이용한 메뉴 선택
int SelectMenu(){
	system("color 0F");
	while(1){
		if(GetAsyncKeyState(VK_UP)){
			if(Select == 0) Select = 3;
			else Select--;
		}
		else if(GetAsyncKeyState(VK_DOWN)){
			if(Select == 3) Select = 0;
			else Select ++;
		}
		else if(GetAsyncKeyState(VK_RETURN)){
			return Select;
		}
		switch (Select) {
			case 0: 
				SetColor(11);
				gotoxy(44, 20); printf("새로  하기");
				SetColor(15); 
				gotoxy(44, 25); printf("이어  하기");
				gotoxy(44, 30); printf("게임  설명");
				gotoxy(44, 35); printf("게임  종료"); 
				break;
			case 1:
				gotoxy(44, 20); printf("새로  하기");
				SetColor(11);
				gotoxy(44, 25); printf("이어  하기");
				SetColor(15);
				gotoxy(44, 30); printf("게임  설명");
				gotoxy(44, 35); printf("게임  종료");
				break;
			case 2:
				gotoxy(44, 20); printf("새로  하기");
				gotoxy(44, 25); printf("이어  하기");
				SetColor(11);
				gotoxy(44, 30); printf("게임  설명");
				SetColor(15);
				gotoxy(44, 35); printf("게임  종료");
				break;
			case 3:
				gotoxy(44, 20); printf("새로  하기");
				gotoxy(44, 25); printf("이어  하기");
				gotoxy(44, 30); printf("게임  설명");
				SetColor(11);
				gotoxy(44, 35); printf("게임  종료");
				SetColor(15);
				break;
			default: 
				break;
		}
		Sleep(DELAYTIME);
	}
}
//게임설명창 생성
void PrintGuide() 
{
	for (int i = 1; i < MENU_WIDTH; i++)
	{
		gotoxy(i, 1);
		printf("■");
		gotoxy(i, MENU_HEIGHT);
		printf("■");
	}

	for (int i = 1	; i < MENU_HEIGHT; i++)
	{
		gotoxy(1, i);
		printf("■");
		gotoxy(MENU_WIDTH, i);
		printf("■");
	}
	gotoxy(1, 1);
	printf("■");
	gotoxy(1, MENU_HEIGHT);
	printf("■");
	gotoxy(MENU_WIDTH, 1);
	printf("■");
	gotoxy(MENU_WIDTH, MENU_HEIGHT);
	printf("■");
	
	gotoxy(35, 4);
	printf("<게임 규칙>");
	gotoxy(10, 10);
	printf("1. 조작은 방향키(←, →, ↑, ↓)로 한다.");
	gotoxy(13, 11);
	printf(" (2P의 경우 w, a, s, d 로 조작한다.) ");
	gotoxy(10, 15);
	printf("2. 아이템 설명");
	gotoxy(15, 17);
	printf("@ : 속도 증가");
	gotoxy(15, 19);
	printf("# : 속도 감소");
	gotoxy(15, 21);
	printf("$ : 꼬리 2개 증가");
	gotoxy(15, 23);
	printf("& : 꼬리 1개 감소");
	gotoxy(10, 28);
	printf("3. 벽이나 몸체에 닿을 시 게임이 종료된다.");
	gotoxy(15, 29);
	printf("(벽이나 몸체에 부딛혀 게임이 종료 될 시 저장된");
	gotoxy(40, 30);
	printf("세이브는 삭제된다.)");
	gotoxy(MENU_WIDTH - 15, MENU_HEIGHT - 3);
	SetColor(11);
	printf("규칙 닫기");
	SetColor(15);
}

//지렁이를 늘리는 함수(이중연결리스트의 테일쪽에 노드 추가)
void AddWorm(pWORM wormTailNode)
{
	pWORM newNode = malloc(sizeof(WORM));
	pWORM temp = wormTailNode->next;

	//새 노드의 주솟값 이어주기;
	newNode->before = wormTailNode;
	newNode->next = wormTailNode->next;
	wormTailNode->next = newNode;
	newNode->next->before = newNode;
	newNode->direction = temp->direction;
	newNode->x = temp->x;
	newNode->y = temp->y;
	switch (temp->direction)
	{
	case LEFT:
		newNode->x = newNode->x + 1;
		break;
	case RIGHT:
		newNode->x = newNode->x - 1;
		break;
	case UP:
		newNode->y = newNode->y + 1;
		break;
	case DOWN:
		newNode->y = newNode->y - 1;
		break;
	default:
		break;
	}
}

void AddWorm2P(rWORM wormTailNode)
{
	rWORM newNode = malloc(sizeof(WORM));
	rWORM temp = wormTailNode->next;

	//새 노드의 주솟값 이어주기;
	newNode->before = wormTailNode;
	newNode->next = wormTailNode->next;
	wormTailNode->next = newNode;
	newNode->next->before = newNode;
	newNode->direction = temp->direction;
	newNode->x = temp->x;
	newNode->y = temp->y;
	switch (temp->direction)
	{
	case LEFT:
		newNode->x = newNode->x + 1;
		break;
	case RIGHT:
		newNode->x = newNode->x - 1;
		break;
	case UP:
		newNode->y = newNode->y + 1;
		break;
	case DOWN:
		newNode->y = newNode->y - 1;
		break;
	default:
		break;
	}
}

//연결리스트를 받아서 지렁이 출력
void PrintWorm(pWORM wormTailNode, pWORM wormHeadNode)
{
	pWORM curr = wormTailNode->next;
	while (curr != wormHeadNode)
	{
		if(curr->next == wormHeadNode){
			gotoxy(curr->x, curr->y);
			printf("O");
		}
		else{
			gotoxy(curr->x, curr->y);
			printf("o");
		}
		curr = curr->next;
	}
}
// 2인플레이 1P의 지렁이 생성
void PrintWorm1P(pWORM wormTailNode, pWORM wormHeadNode)
{
	pWORM curr = wormTailNode->next;
	while (curr != wormHeadNode)
	{
		if(curr->next == wormHeadNode){
			gotoxy(curr->x, curr->y);
			SetColor(12);
			printf("O");
			SetColor(15);
		}
		else{
			gotoxy(curr->x, curr->y);
			SetColor(12);
			printf("o");
			SetColor(15);
		}
		curr = curr->next;
	}
}
//2인플레이 2P의 지렁이 생성
void PrintWorm2P(rWORM wormTailNode, rWORM wormHeadNode)
{
	rWORM curr = wormTailNode->next;
	while (curr != wormHeadNode)
	{
		if(curr->next == wormHeadNode){
			gotoxy(curr->x, curr->y);
			SetColor(9);
			printf("O");
			SetColor(15);
		}
		else{
			gotoxy(curr->x, curr->y);
			SetColor(9);
			printf("o");
			SetColor(15);
		}
		curr = curr->next;
	}
}

//게임점수 출력
void PrintScore(int score)	
{
	gotoxy(FIELD_WIDTH + 3,  3);
	printf("점수 : %d점",score);
	gotoxy(FIELD_WIDTH + 3,  5);
	printf("꼬리 개수 : %3d개", cntTail);
	gotoxy(FIELD_WIDTH + 3,  7);
	printf("현재 속도 : %2d단계", NowSpeed);
	gotoxy(FIELD_WIDTH + 3, FIELD_HEIGHT - 1); 
	printf("저장 : V");
	gotoxy(FIELD_WIDTH + 3, FIELD_HEIGHT - 2); 
	printf("일시정지 : O");
	gotoxy(FIELD_WIDTH + 3,  FIELD_HEIGHT - 3);
	printf("종료 : Q");
}

//웜이 지나간 자리 지우기
void ClearWorm(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
}

//웜 움직이기 
void MoveWorm(pWORM wormTailNode, pWORM wormHeadNode)
{
	pWORM curr;

	curr = wormTailNode->next;
	while (curr != NULL && curr != wormHeadNode->before)
	{
		curr->x = curr->next->x;
		curr->y = curr->next->y;
		curr = curr->next;	
	}
	curr = wormHeadNode->before;
	if (curr->direction == LEFT)
	{
		curr->x--;
	}else if (curr->direction == RIGHT)
	{
		curr->x++;
	}
	else if (curr->direction == UP)
	{
		curr->y--;
	}
	else if (curr->direction == DOWN)
	{
		curr->y++;
	}
}
//2P 웜 움직이기
void MoveWorm2P(rWORM wormTailNode, rWORM wormHeadNode)
{
	rWORM curr;

	curr = wormTailNode->next;
	while (curr != NULL && curr != wormHeadNode->before)
	{
		curr->x = curr->next->x;
		curr->y = curr->next->y;
		curr = curr->next;	
	}
	curr = wormHeadNode->before;
	if (curr->direction == LEFT)
	{
		curr->x--;
	}else if (curr->direction == RIGHT)
	{
		curr->x++;
	}
	else if (curr->direction == UP)
	{
		curr->y--;
	}
	else if (curr->direction == DOWN)
	{
		curr->y++;
	}
}

//웜 리스트에 할당된 메모리를 순차로 해제
void FreeWormList(pWORM wormTailNode)
{
	pWORM curr;
	pWORM temp;
	curr = wormTailNode;
	while (curr != NULL)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
}
//2P의 웜 리스트에 할당된 메모리를 순차로 해제
void FreeWormList2P(rWORM wormTailNode)
{
	rWORM curr;
	rWORM temp;
	curr = wormTailNode;
	while (curr != NULL)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
}
//꼬리 한개 삭제
void CleanTail(pWORM wormTailNode)
{
	pWORM curr;
	curr = wormTailNode->next;

	gotoxy(curr->x, curr->y);
	printf(" ");


	curr->next->before = curr->before;
	curr->before->next = curr->next;

	free(curr);

}
//2인플레이시 꼬리 한개 삭제
void CleanTail2P(rWORM wormTailNode)
{
	rWORM curr;
	curr = wormTailNode->next;

	gotoxy(curr->x, curr->y);
	printf(" ");


	curr->next->before = curr->before;
	curr->before->next = curr->next;

	free(curr);

}

//item 발생시키기
void CreateItem(pITEM itemNode, int* itemNo) {
		
	pITEM newItem = malloc(sizeof(ITEM));
	
	newItem->next = itemNode->next;
	newItem->itemNo = (*itemNo)++;
	itemNode->next = newItem;
	newItem->x = 3 + rand() % (FIELD_WIDTH - 3);
	newItem->y = 3 + rand() % (FIELD_HEIGHT - 3);
	newItem->renditem = rand() % 4;
	newItem->itemType = ITEM_EXP;
	if (newItem->renditem == 0)
	{
		newItem->ITEMt = '@';
	}
	else if (newItem->renditem == 1) {
		newItem->ITEMt = '#';
	}
	else if (newItem->renditem == 2) {
		newItem->ITEMt = '$';
	}
	else if (newItem->renditem == 3) {
		newItem->ITEMt = '&';
	}
}

//아이템 화면에 출력
void PrintItem(pITEM itemNode)
{
	
	pITEM curr = itemNode->next;
	while (curr != NULL)
	{
		gotoxy(curr->x, curr->y);
		if (curr->renditem == 0)
		{
			printf("%c", curr->ITEMt);
		}
		else if (curr->renditem == 1) {
			printf("%c", curr->ITEMt);
		}
		else if (curr->renditem == 2) {
			printf("%c", curr->ITEMt);
		}
		else if (curr->renditem == 3) {
			printf("%c", curr->ITEMt);
		}
		curr = curr->next;
	}
}

//리스트에서 itemNo의 위치를 찾아서 카운터를 리턴
int findItemNoInList(pITEM itemNode, int itemNo)	
{
	int numberOfNode = 0;	//노드번호를 저장할 변수

	pITEM curr;		//노드사이를 움직일 포인터
	curr = itemNode->next;		//첫번째 데이터를 가지키는 주소를 curr에 할당

	while (curr != NULL)	//반복문을 돌리면서 찾는 데이터를 발견하면 노드번호를 리턴
	{
		numberOfNode++;
		if (itemNo == curr->itemNo)
		{
			return numberOfNode;
		}
		curr = curr->next;
	}

	return numberOfNode;
}



//아이템의 노드번호를 받아 그 번호의 노드를 삭제
int delItemFromList(pITEM itemNode, int targetNodeNumber)
{	
	pITEM beforeTarget;		//제거할 노드의 앞노드
	pITEM target;		//제거할 노드

	int counter = 0;

	beforeTarget = itemNode;

	if (targetNodeNumber < 0)   // 0이 입력되면 FirstNode를 삭제하기 때문에 실행하지 않고 리턴
		return 0;

	//노드를 targetNodeNumber-1 만큼 이동시켜서 beforeTarget을 제거할 노드 앞으로 이동시킴
	while (counter < targetNodeNumber - 1)
	{
		beforeTarget = beforeTarget->next;
		counter = counter + 1;
	}

	//노드를 리스트에서 제거
	target = beforeTarget->next;
	beforeTarget->next = target->next;
	free(target);
}

//아이템과 웜의 헤드가 만났는지 검사, 
//delItemNo는 먹은 아이템을 지우는 함수로 넘겨줄 변수
int CheckItemHit(pWORM wormHeadPointer, pITEM itemNode, int* delItemNo, pWORM wormTailNode)
{
	pITEM curr;
	int nodeNo = 0;
	curr = itemNode->next;
	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			if (curr->ITEMt == '@') {
				gotoxy(FIELD_WIDTH + 3, 17);
				printf("                                                            ");
				NowSpeed += 1;
				DELAYTIME -= 7;
				cntTail ++;
				if(NowSpeed > 10) {
					NowSpeed = 10;
					DELAYTIME = 30;
					gotoxy(FIELD_WIDTH + 52, 17);
					printf("[MAX]");
				}
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[@] 을(를) 섭취하고 속도가 1단계 증가하였습니다.");

			}
			else if (curr->ITEMt == '#') {
				gotoxy(FIELD_WIDTH + 3, 17);
				printf("                                                             ");
				NowSpeed -= 1;
				DELAYTIME += 7;
				cntTail++;
				if (NowSpeed < -10)
				{
					NowSpeed = -10;
					DELAYTIME = 170;
					gotoxy(FIELD_WIDTH + 52, 17);
					printf("[MIN]");
				}
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[#] 을(를) 섭취하고 속도가 1단계 감소하였습니다.");
			}
			else if (curr->ITEMt == '$') {
				AddWorm(wormTailNode);
				cntTail ++;
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("                                                             ");
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[$] 을(를) 섭취하고 꼬리가 2개 증가하였습니다.");
			}
			else if (curr->ITEMt == '&') {
				if (cntTail <= 5){
					CleanTail(wormTailNode);
					gotoxy(FIELD_WIDTH + 3, 17);
					printf("                                                        ");
					gotoxy(FIELD_WIDTH + 3, 17);
					printf("[&] 을(를) 섭취하고 꼬리가 감소하였습니다. [MIN]");
				}
				else {
				CleanTail(wormTailNode);
				CleanTail(wormTailNode);
				cntTail --;
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("                                                             ");
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[&] 을(를) 섭취하고 꼬리가 감소하였습니다.");

				}
				
			}

			if (curr->itemType == ITEM_EXP)
			{
				*delItemNo = curr->itemNo;
				return 1; //아이템 먹으면 1리턴
			}
		}
		nodeNo++;
		curr = curr->next;
	}
	return 0;//아이템을 안만나면 0
}
//2인 플레이시 1P의 헤드가 아이템과 만났는지 확인
int CheckItemHit1P(pWORM wormHeadPointer, pITEM itemNode, int* delItemNo, pWORM wormTailNode)
{
	pITEM curr;
	int nodeNo = 0;
	curr = itemNode->next;
	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			if (curr->ITEMt == '@') {
				if (NowSpeed <= 10)
				{
					NowSpeed += 1;
					DELAYTIME -= 7;
					cntTail++;
				}
			}
			else if (curr->ITEMt == '#') {
				if (NowSpeed >= -10)
				{
					NowSpeed -= 1;
					DELAYTIME += 7;
					cntTail++;
				}
			}
			else if (curr->ITEMt == '$') {
				AddWorm(wormTailNode);
				cntTail++;
			}
			else if (curr->ITEMt == '&') {
				if (cntTail <= 5) {
					CleanTail(wormTailNode);
				}
				else {
				CleanTail(wormTailNode);
				CleanTail(wormTailNode);
				cntTail--;
				}
			}

			if (curr->itemType == ITEM_EXP)
			{
				*delItemNo = curr->itemNo;
				return 1; //아이템 먹으면 1리턴
			}
		}
		nodeNo++;
		curr = curr->next;
	}
	return 0;//아이템을 안만나면 0
}
//2인 플레이시 2P가 아이템과 만났는지 확인
int CheckItemHit2P(rWORM wormHeadPointer, pITEM itemNode, int* delItemNo, rWORM wormTailNode)
{
	pITEM curr;
	int nodeNo = 0;
	curr = itemNode->next;
	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			if (curr->ITEMt == '@') {
				if (NowSpeed <= 10)
				{
					NowSpeed += 1;
					DELAYTIME -= 7;
					cntTail++;
				}

			}
			else if (curr->ITEMt == '#') {
				if (NowSpeed >= -10)
				{
					NowSpeed -= 1;
					DELAYTIME += 7;
					cntTail++;
				}
			}
			else if (curr->ITEMt == '$') {
				AddWorm(wormTailNode);
				cntTail ++;
			}
			else if (curr->ITEMt == '&') {
				if (cntTail <= 5) {
					CleanTail(wormTailNode);
				}
				else {
				CleanTail(wormTailNode);
				CleanTail(wormTailNode);
				cntTail--;
				}

			}

			if (curr->itemType == ITEM_EXP)
			{
				*delItemNo = curr->itemNo;
				return 1; //아이템 먹으면 1리턴
			}
		}
		nodeNo++;
		curr = curr->next;
	}
	return 0;//아이템을 안만나면 0
}

//웜의 머리와 몸통이 만났는지 확인
int CheckWormHit(pWORM wormHeadPointer)
{
	pWORM curr;
	curr = wormHeadPointer -> before;

	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			return 1;
		}
		
		curr = curr->before;
	}
	return 0; //몸통을 안만나면 0
}
// 2인플레이시 1P의 머리와 자신의 몸통 혹은 2P의 몸통 혹은 2P의 머리와 만났는지 확인
int CheckWormHit1P(pWORM wormHeadPointer, rWORM wormHeadPointer2)
{
	pWORM curr;
	curr = wormHeadPointer -> before;
	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			return 1;
		}
		if (wormHeadPointer2->x == curr->x && wormHeadPointer2->y == curr->y){
			return -1;
		}
		if (wormHeadPointer->x == wormHeadPointer2->x && wormHeadPointer->y == wormHeadPointer2->y)
		{
			return 2;
		}
		
		curr = curr->before;
	}
	
	return 0; //몸통을 안만나면 0
}
// 2인플레이시 2P의 머리와 자신의 몸통 혹은 1P의 몸통 혹은 1P의 머리와 만났는지 확인
int CheckWormHit2P(rWORM wormHeadPointer2, pWORM wormHeadPointer)
{
	rWORM curr;
	curr = wormHeadPointer2 -> before;

	while (curr != NULL)
	{
		if (wormHeadPointer2->x == curr->x && wormHeadPointer2->y == curr->y)
		{
			return 1;	
		}
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
			return -1;
		}
		if (wormHeadPointer2->x == wormHeadPointer->x && wormHeadPointer2->y == wormHeadPointer->y)
		{
			return 2;
		}
		
		curr = curr->before;
	}
	return 0; //몸통을 안만나면 0
}

//아이템의 링크드 리스트 메모리 해제
void FreeItemList(pITEM itemNode)
{
	pITEM curr;
	pITEM temp;
	curr = itemNode;
	while (curr != NULL)
	{
		temp = curr->next;
		free(curr);
		curr = temp;
	}
}
//웜이 게임오버 되었을 때 다시 시작과 게임 종료를 표시
int SelRestart(){
	int Restart = 0  ;
	system("color 0F");
	while(1){
		if(GetAsyncKeyState(VK_UP)){
			if(Restart == 0) Restart = 1;
			else Restart--;
		}
		else if(GetAsyncKeyState(VK_DOWN)){
			if(Restart == 1) Restart = 0;
			else Restart ++;
		}
		else if(GetAsyncKeyState(VK_RETURN)){
			return Restart;
		}
		switch (Restart) {
			case 0: 
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				SetColor(11);
				printf("다시  시작");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				SetColor(15); 
				printf("게임  종료");
				break;
			case 1:
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				printf("다시  시작");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				SetColor(11);
				printf("게임  종료");
				SetColor(15); 
				break;
			default: 
				break;
		}
		Sleep(DELAYTIME);
	}
}	

//점수, 딜레이타임, 현재 속도, 꼬리 개수, 웜의 좌표값을 저장
void Save(int score,pWORM wormHeadPointer, pWORM wormTailNode) {
	FILE * fp = fopen("Save.sv", "wt");
	pWORM carr;
	carr = wormHeadPointer;


	fprintf(fp, "%d\n", score);
	fprintf(fp, "%d\n", DELAYTIME);
	fprintf(fp, "%d\n", NowSpeed);
	fprintf(fp, "%d\n", cntTail);

	while (carr->before != NULL)
	{
		fprintf(fp, "%d %d %d\n", carr->x, carr -> y, carr -> direction);
		carr = carr->before;
	}


	fclose(fp);
	gotoxy(FIELD_WIDTH / 2 - 10, 7); 
	printf("저장 완료");
}
//점수, 딜레이타임, 현재 속도, 꼬리 개수를 불러오기
int Load(){
	FILE * fp = fopen("Save.sv", "rt");
	if (fp == NULL) return -1;

	if (fscanf_s(fp, "%d", &score) != 1) {
        score = 0; // 읽기 실패 시 기본값 설정
    }

    if (fscanf_s(fp, "%d", &DELAYTIME) != 1) {
        DELAYTIME = 100; // 읽기 실패 시 기본값
    }

    if (fscanf_s(fp, "%d", &NowSpeed) != 1) {
        NowSpeed = 0; // 읽기 실패 시 기본값
    }
	if (fscanf_s(fp, "%d", &cntTail) != 1) {
        cntTail = 9; // 읽기 실패 시 기본값
    }
	
    
	fclose(fp);
	return 1;
}
//이어 하기 선택시 저장된 값이 없다면 print출력
int PrintLoadErr(){
	gotoxy(37, 25);
	SetColor(11);
	printf("저장된 데이터가 없습니다!");
	SetColor(15);
	while(1){
		if(GetAsyncKeyState(VK_RETURN)){
			return 0;
		}
	}
}
// 새로 하기 선택시에 1P와 2P중에 선택
int Sel2P(){
	int Player = 1;
	gotoxy(44, 20);
	printf("1P");
	gotoxy(52, 20);
	printf("2P");
	gotoxy(46, 20);
	printf("      ");
	while(1){
		if(GetAsyncKeyState(VK_RIGHT)){
			if(Player == 1) Player = 2;
			else Player--;
		}
		else if(GetAsyncKeyState(VK_LEFT)){
			if(Player == 2) Player = 1;
			else Player ++;
		}
		else if(GetAsyncKeyState(VK_RETURN)){
			return Player;
		}
		switch (Player) {
			case 1: 
				gotoxy(44, 20);
				SetColor(11);
				printf("1P");
				gotoxy(52, 20);
				SetColor(15); 
				printf("2P");
				break;
			case 2:
				gotoxy(44, 20);
				printf("1P");
				gotoxy(52, 20);
				SetColor(11);
				printf("2P");
				SetColor(15); 
				break;
			default: 
				break;
		}
		Sleep(DELAYTIME);
	}
}


int main()
{	
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SetConsoleTitle("준호수의 Snake-Game");
	
	DeleteCursor();
	int Select, Restart, Player, load = 0;
	
	menu:
	
	system("cls");	//화면지우고
	PrintMenu();	//필드 출력
	Sleep(300);
	Select = SelectMenu();
	if(Select == 0){
		Sleep(100);
		Player = Sel2P();
		if(Player == 1){
		starting:
		pWORM wormHeadNode = malloc(sizeof(WORM));//이중연결리스트 헤드노드
		pWORM wormTailNode = malloc(sizeof(WORM));//이중연결리스트 테일노드
		pWORM addWorm = malloc(sizeof(WORM));//첫번째 웜몸통
		pITEM itemNode = malloc(sizeof(ITEM));//아이템용 단일 연결리스트
		

		wormHeadNode->next = NULL;
		wormHeadNode->before = addWorm;
		addWorm->next = wormHeadNode;
		addWorm->before = wormTailNode;
		wormTailNode->next = addWorm;
		wormTailNode->before = NULL;
		addWorm->x = FIELD_WIDTH/2;
		addWorm->y = FIELD_HEIGHT/2;
		addWorm->direction = RIGHT;

		itemNode->next = NULL;
		itemNode->itemNo = -1;
		
		system("cls");
		PrintField();
		pWORM wormHeadPointer = addWorm;
		//지렁이 게임시작 지렁이 생성
		for(int i = cntTail; i>0 ; i--)
			AddWorm(wormTailNode);
		//저장된 값이 있을 경우 꼬리의 위치 설정
		if(load == 1){
			FILE * fp = fopen("Save.sv", "rt");
			pWORM curr = wormHeadPointer -> before;
			int temp, tmpx, tmpy, tmpz;
			for(int i = 0; i < 4; i++){
				fscanf_s(fp, "%d", &temp);
			}
			fscanf_s(fp, "%d %d %d",&tmpx, &tmpy, &tmpz);
			wormHeadPointer -> x = tmpx;
			wormHeadPointer -> y = tmpy;
			wormHeadPointer -> direction = tmpz;
			for(int i = 0; i < cntTail; i++){
				fscanf_s(fp, "%d %d %d", &tmpx, &tmpy, &tmpz);
				curr -> x = tmpx;
				curr -> y = tmpy;
				curr -> direction = tmpz;
				curr = curr -> before;
			}
			fclose(fp);
		}
		//웜의 머리를 가리키는 포인터
		

	
		int itemCounter = 0;	//아이템 생성 한도 카운터
		char key;				//키입력받을 변수
		int delItemNo=0;		//지울아이템넘버를 받을 변수초기화
		int itemNo = 10000;//아이템의 최초번호

		//아이템 생성 위치 난수 시드
		srand((unsigned int)time(NULL));

		while (1)
		{

			//Q를 입력받으면 종료
			if (GetAsyncKeyState(0x51))
			{
				break;
			}
			//O를 입력받으면 일시정지
			if (GetAsyncKeyState(0x4F)) {
				gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
				printf("일시정지 상태!\n");
				gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2 + 1);
				system("pause");
				system("cls");
				PrintField();
			}
			//V를 입력받으면 저장 후 종료
			if (GetAsyncKeyState(0x56)){
				Save(score, wormHeadPointer, wormTailNode);
				break;
			}
			if (GetAsyncKeyState(VK_LEFT) && wormHeadPointer->direction != RIGHT)
			{
				wormHeadPointer->direction = LEFT;
			}
			else if (GetAsyncKeyState(VK_RIGHT) && wormHeadPointer->direction != LEFT)
			{
				wormHeadPointer->direction = RIGHT;
			}
			else if (GetAsyncKeyState(VK_UP) && wormHeadPointer->direction != DOWN)
			{
				wormHeadPointer->direction = UP;
			}
			else if (GetAsyncKeyState(VK_DOWN) && wormHeadPointer->direction != UP)
			{
				wormHeadPointer->direction = DOWN;
			}
			
			
			//웜 지나간 자리 지우기
			ClearWorm(wormTailNode->next->x, wormTailNode->next->y);
			
			//웜 한칸씩 움직이기
			MoveWorm(wormTailNode, wormHeadNode);

			//벽에 부딛히면 게임오버
			if (wormHeadPointer->x == 0 || wormHeadPointer->x == FIELD_WIDTH || wormHeadPointer->y == 0 || wormHeadPointer->y == FIELD_HEIGHT)
			{
				remove("Save.sv");
				system("cls");
				gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
				printf("벽에 부딛혔습니다. GAME OVER");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				printf("다시  시작");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				printf("게임  종료");
				Sleep(300);
				Restart = SelRestart();
				// 다시 시작 선택시 점수, 속도, 꼬리 개수 초기화 후 처음으로 이동
				// 게임 종료 선택시 print출력 후 종료
				switch(Restart){
					case 0:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						load = 0;
						score = 0;
						DELAYTIME = 100;
						NowSpeed = 0;
						cntTail = 9;
						goto starting;
					
					case 1:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
						printf("프로그램을 종료합니다");
						gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
						system("pause");
						return 0;
				}
			}

			

			//아이템을 생성
			while (itemCounter < ITEM_MAX)
			{
				CreateItem(itemNode,&itemNo);
				itemCounter++;
			}

			//아이템 먹었는지 확인
			if (CheckItemHit(wormHeadPointer, itemNode, &delItemNo, wormTailNode))
			{
				AddWorm(wormTailNode);
				delItemFromList(itemNode, findItemNoInList(itemNode,delItemNo));
				score += 100;
				itemCounter--;
			}
			// 머리가 몸에 부딛혔는지 확인
			if (CheckWormHit(wormHeadPointer)){
				remove("Save.sv");
				system("cls");
				gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
				printf("몸에 부딛혔습니다. GAME OVER");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				printf("다시  시작");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				printf("게임  종료");
				Sleep(300);
				Restart = SelRestart();

				switch(Restart){
					case 0:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						score = 0;
						DELAYTIME = 100;
						NowSpeed = 0;
						cntTail = 9;
						goto starting;
					
					case 1:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
						printf("프로그램을 종료합니다");
						gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
						system("pause");
						return 0;
				}
			}
			PrintItem(itemNode);
			PrintWorm(wormTailNode, wormHeadNode);
			PrintScore(score);
			Sleep(DELAYTIME);
		}
		FreeWormList(wormTailNode);
		FreeItemList(itemNode);
		gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
		printf("프로그램을 종료합니다");
		gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2 + 1);
		system("pause");
		system("cls");
		return 0;
		}
		else if(Player == 2){
			pWORM wormHeadNode = malloc(sizeof(WORM));//이중연결리스트 헤드노드
			pWORM wormTailNode = malloc(sizeof(WORM));//이중연결리스트 테일노드
			pWORM addWorm = malloc(sizeof(WORM));//첫번째 웜몸통
			rWORM wormHeadNode2 = malloc(sizeof(WORM));//이중연결리스트 헤드노드
			rWORM wormTailNode2 = malloc(sizeof(WORM));//이중연결리스트 테일노드
			rWORM addWorm2 = malloc(sizeof(WORM));//첫번째 웜몸통
			pITEM itemNode = malloc(sizeof(ITEM));//아이템용 단일 연결리스트
			

			wormHeadNode->next = NULL;
			wormHeadNode->before = addWorm;
			addWorm->next = wormHeadNode;
			addWorm->before = wormTailNode;
			wormTailNode->next = addWorm;
			wormTailNode->before = NULL;
			addWorm->x = FIELD_WIDTH/2 + 10;
			addWorm->y = FIELD_HEIGHT/2 + 10;
			addWorm->direction = LEFT;

			wormHeadNode2->next = NULL;
			wormHeadNode2->before = addWorm2;
			addWorm2->next = wormHeadNode2;
			addWorm2->before = wormTailNode2;
			wormTailNode2->next = addWorm2;
			wormTailNode2->before = NULL;
			addWorm2->x = FIELD_WIDTH/2 - 10;
			addWorm2->y = FIELD_HEIGHT/2 - 10;
			addWorm2->direction = RIGHT;

			itemNode->next = NULL;
			itemNode->itemNo = -1;
			
			system("cls");
			PrintField();
			//지렁이 게임시작 지렁이 생성
			for(int i = cntTail; i>0 ; i--){
				AddWorm(wormTailNode);
				AddWorm2P(wormTailNode2);
			}
			//웜의 머리를 가리키는 포인터
			pWORM wormHeadPointer = addWorm;
			rWORM wormHeadPointer2 = addWorm2;
		
			int itemCounter = 0;	//아이템 생성 한도 카운터
			char key;				//키입력받을 변수
			int delItemNo=0;		//지울아이템넘버를 받을 변수초기화
			int itemNo = 10000;//아이템의 최초번호

			//아이템 생성 위치 난수 시드
			srand((unsigned int)time(NULL));

			gotoxy(FIELD_WIDTH + 3,  3);
			SetColor(12);
			printf("1P : ←, →, ↑, ↓");
			SetColor(15);
			gotoxy(FIELD_WIDTH + 3,  5);
			SetColor(9);
			printf("2P : W, A, S, D");
			SetColor(15);
			

			
			while (1)
			{
				if (GetAsyncKeyState(0x51))
				{
					break;
				}
				if (GetAsyncKeyState(0x4F)) {
					gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
					printf("일시정지 상태!\n");
					gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2 + 1);
					system("pause");
					system("cls");
					PrintField();
				}
				if (GetAsyncKeyState(VK_LEFT) && wormHeadPointer->direction != RIGHT)
				{
					wormHeadPointer->direction = LEFT;
				}
				else if (GetAsyncKeyState(VK_RIGHT) && wormHeadPointer->direction != LEFT)
				{
					wormHeadPointer->direction = RIGHT;
				}
				else if (GetAsyncKeyState(VK_UP) && wormHeadPointer->direction != DOWN)
				{
					wormHeadPointer->direction = UP;
				}
				else if (GetAsyncKeyState(VK_DOWN) && wormHeadPointer->direction != UP)
				{
					wormHeadPointer->direction = DOWN;
				}
				
				if (GetAsyncKeyState(0x41) && wormHeadPointer2->direction != RIGHT)
				{
					wormHeadPointer2->direction = LEFT;
				}
				else if (GetAsyncKeyState(0x44) && wormHeadPointer2->direction != LEFT)
				{
					wormHeadPointer2->direction = RIGHT;
				}
				else if (GetAsyncKeyState(0x57) && wormHeadPointer2->direction != DOWN)
				{
					wormHeadPointer2->direction = UP;
				}
				else if (GetAsyncKeyState(0x53) && wormHeadPointer2->direction != UP)
				{
					wormHeadPointer2->direction = DOWN;
				}
				
				
				//웜 지나간 자리 지우기
				ClearWorm(wormTailNode->next->x, wormTailNode->next->y);
				ClearWorm(wormTailNode2->next->x, wormTailNode2->next->y);
				
				//웜 한칸씩 움직이기
				MoveWorm(wormTailNode, wormHeadNode);
				MoveWorm2P(wormTailNode2, wormHeadNode2);

				//벽에 부딛히면 게임오버
				if (wormHeadPointer->x == 0 || wormHeadPointer->x == FIELD_WIDTH || wormHeadPointer->y == 0 || wormHeadPointer->y == FIELD_HEIGHT)
				{
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
					printf("벽에 부딛혔습니다. GAME OVER");
					gotoxy(FIELD_WIDTH / 2 , FIELD_HEIGHT / 2 + 2);
					SetColor(9);
					printf("2P WIN!");
					SetColor(15);
					gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
						FreeWormList(wormTailNode);
						FreeWormList2P(wormTailNode2);
						FreeItemList(itemNode);
						gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
						printf("프로그램을 종료합니다");
						gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
						system("pause");
						return 0;
				
				}

				if (wormHeadPointer2->x == 0 || wormHeadPointer2->x == FIELD_WIDTH || wormHeadPointer2->y == 0 || wormHeadPointer2->y == FIELD_HEIGHT)
				{
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
					printf("벽에 부딛혔습니다. GAME OVER");
					gotoxy(FIELD_WIDTH / 2 , FIELD_HEIGHT / 2 + 2);
					SetColor(12);
					printf("1P WIN!");
					SetColor(15);
					gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
						FreeWormList(wormTailNode);
						FreeWormList2P(wormTailNode2);
						FreeItemList(itemNode);
						gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
						printf("프로그램을 종료합니다");
						gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
						system("pause");
						return 0;
				
				}
				

				//아이템을 생성
				while (itemCounter < ITEM_MAX)
				{
					CreateItem(itemNode,&itemNo);
					itemCounter++;
				}

				//아이템 먹었는지 확인
				if (CheckItemHit1P(wormHeadPointer, itemNode, &delItemNo, wormTailNode))
				{
					AddWorm(wormTailNode);
					delItemFromList(itemNode, findItemNoInList(itemNode,delItemNo));
					score += 100;
					itemCounter--;
				}
				if (CheckWormHit1P(wormHeadPointer, wormHeadPointer2) == 2) {
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 6, FIELD_HEIGHT / 2);
					printf("동시에 부딛혔습니다.");
					gotoxy(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 2);
					printf("DRAW!");
					gotoxy(FIELD_WIDTH / 2 - 2, FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
					FreeWormList(wormTailNode);
					FreeWormList2P(wormTailNode2);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 - 8, FIELD_HEIGHT / 2 + 8);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT / 2 + 9);
					system("pause");
					return 0;
				}
				else if (CheckWormHit1P(wormHeadPointer, wormHeadPointer2) == 1){
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
					printf("몸에 부딛혔습니다. GAME OVER");
					gotoxy(FIELD_WIDTH / 2  , FIELD_HEIGHT / 2 + 2);
					SetColor(9);
					printf("2P WIN!");
					SetColor(15);
					gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
					FreeWormList(wormTailNode);
					FreeWormList2P(wormTailNode2);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
					system("pause");
					return 0;
					
				}
				else if (CheckWormHit1P(wormHeadPointer, wormHeadPointer2) == -1) {
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
					printf("몸에 부딛혔습니다. GAME OVER");
					gotoxy(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 2);
					SetColor(12);
					printf("1P WIN!");
					SetColor(15);
					gotoxy(FIELD_WIDTH / 2 - 2, FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
					FreeWormList(wormTailNode);
					FreeWormList2P(wormTailNode2);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 - 8, FIELD_HEIGHT / 2 + 8);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT / 2 + 9);
					system("pause");
					return 0;

				}
				if (CheckItemHit2P(wormHeadPointer2, itemNode, &delItemNo, wormTailNode2))
				{
					AddWorm2P(wormTailNode2);
					delItemFromList(itemNode, findItemNoInList(itemNode,delItemNo));
					score += 100;
					itemCounter--;
				}
				if (CheckWormHit2P(wormHeadPointer2, wormHeadPointer) == 2) {
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 6, FIELD_HEIGHT / 2);
					printf("동시에 부딛혔습니다.");
					gotoxy(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 2);
					printf("DRAW!");
					gotoxy(FIELD_WIDTH / 2 - 2, FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
					FreeWormList(wormTailNode);
					FreeWormList2P(wormTailNode2);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 - 8, FIELD_HEIGHT / 2 + 8);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT / 2 + 9);
					system("pause");
					return 0;
				}
				else if (CheckWormHit2P(wormHeadPointer2, wormHeadPointer) == 1){
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
					printf("몸에 부딛혔습니다. GAME OVER");
					gotoxy(FIELD_WIDTH / 2 , FIELD_HEIGHT / 2 + 2);
					SetColor(12);
					printf("1P WIN!");
					SetColor(15);
					gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
					FreeWormList(wormTailNode);
					FreeWormList2P(wormTailNode2);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
					system("pause");
					return 0;
					
				}
				else if (CheckWormHit2P(wormHeadPointer2, wormHeadPointer) == -1) {
					system("cls");
					gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
					printf("몸에 부딛혔습니다. GAME OVER");
					gotoxy(FIELD_WIDTH / 2, FIELD_HEIGHT / 2 + 2);
					SetColor(9);
					printf("2P WIN!");
					SetColor(15);
					gotoxy(FIELD_WIDTH / 2 - 2, FIELD_HEIGHT / 2 + 4);
					printf("게임  종료");
					FreeWormList(wormTailNode);
					FreeWormList2P(wormTailNode2);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 - 8, FIELD_HEIGHT / 2 + 8);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT / 2 + 9);
					system("pause");
					return 0;

				}
				
				PrintItem(itemNode);
				PrintWorm1P(wormTailNode, wormHeadNode);
				PrintWorm2P(wormTailNode2, wormHeadNode2);
				Sleep(DELAYTIME);
			}
			FreeWormList(wormTailNode);
			FreeWormList2P(wormTailNode2);
			FreeItemList(itemNode);
			gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
			printf("프로그램을 종료합니다");
			gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2 + 1);
			system("pause");
			system("cls");
			return 0;
		}

	}
		
	else if(Select == 1){
			Sleep(100);
			load = Load();
			if(load == -1) PrintLoadErr();
			else {
				goto starting;
			}
			goto menu;
			
		}
		
	else if(Select == 2){
		Sleep(100);
		system("cls");
		PrintGuide();
		while(1){
			if(GetAsyncKeyState(VK_RETURN)) break;
		}
		goto menu;
	}	
	else if(Select == 3){
		system("cls");
		return 0;
	}	
}