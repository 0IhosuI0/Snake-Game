#include <stdio.h>	//표준입출력헤더
#include <stdlib.h>	//malloc, free 등
#include <Windows.h>//windows console api 헤더
#include <time.h>	//랜덤 시드용 time 함수 헤더
#include <conio.h>	//gotoxy에서 사용할 api 헤더

#define FIELD_WIDTH 70	//필드가로길이
#define FIELD_HEIGHT 25	//필드세로길이
#define LEFT 75			//키보드 좌 화살표의 char값
#define RIGHT 77		//키보드 우
#define UP 72			//키보드 위
#define DOWN 80			//지보드 아래
#define ITEM_MAX 2		//화면에 표시되는 아이템개수
#define ITEM_GOLD 101	//골드 아이템 인디케이터
#define ITEM_EXP 102	//경험치 아이템 인디케이터
#define LEFT_MARGIN 30	//화면왼쪽마진(공백)
#define TOP_MARGIN 10	//화면 상단마진(공백)
#define DELAYTIME 100	//Sleep함수에 들어갈 x/1000 초

//지렁이를 구현할 이중연결리스트 구조체
#pragma pack(push,1)
typedef struct _WORM
{
	int x;	//x좌표
	int y;	//y좌표
	char direction;	//진행방향
	struct _WORM *next;	//다음노드주소
	struct _WORM *before;//이전노드주소
}WORM, *pWORM;

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

//커서를 일정 좌표로 이동
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x + LEFT_MARGIN;//가로세로 비율을 맞추기 위해서 가로의 좌표는 2배
	Pos.Y = y + TOP_MARGIN;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void PrintMenu() 
{
	for (int i = 1; i < FIELD_WIDTH; i++)
	{
		gotoxy(i, 20);
		printf("■");
		gotoxy(i, FIELD_HEIGHT + 20);
		printf("■");
	}

	for (int i = 20; i < FIELD_HEIGHT + 20; i++)
	{
		gotoxy(0, i);
		printf("■");
		gotoxy(FIELD_WIDTH,i);
		printf("■");
	}
	gotoxy(0, 20);
	printf("■");
	gotoxy(0, FIELD_HEIGHT + 20);
	printf("■");
	gotoxy(FIELD_WIDTH, 20);
	printf("■");
	gotoxy(FIELD_WIDTH, FIELD_HEIGHT + 20);
	printf("■");
	gotoxy(FIELD_WIDTH,  0);
	printf("    //   ) )                                                  //   ) )                                     ");
	gotoxy(FIELD_WIDTH,  1);
	printf("   ((             __        ___       / ___      ___         //            ___        _   __        ___    ");
	gotoxy(FIELD_WIDTH,  2);
	printf("     \\        //   ) )   //   ) )   //\\ \\     //___) )     //  ____     //   ) )   // ) )  ) )   //___) ) ");
	gotoxy(FIELD_WIDTH,  3);
	printf("       ) )    //   / /   //   / /   //  \\ \\   //           //    / /    //   / /   // / /  / /   //        ");
	gotoxy(FIELD_WIDTH,  4);
	printf("((___ / /    //   / /   ((___( (   //    \\ \\ ((____       ((____/ /    ((___( (   // / /  / /   ((____     ");
	
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

//연결리스트를 받아서 지렁이 출력
void PrintWorm(pWORM wormTailNode, pWORM wormHeadNode)	
{
	pWORM curr = wormTailNode->next;
	while (curr != wormHeadNode)
	{
		gotoxy(curr->x, curr->y);
		printf("O");
		curr = curr->next;
	}
}

//게임점수 출력
void PrintScore(int score)	
{
	gotoxy(FIELD_WIDTH + 3,  3);
	printf("점수 : %d점",score);
	gotoxy(FIELD_WIDTH + 3, 5); 
	printf("일시정지하려면 S를 누르세요");
	gotoxy(FIELD_WIDTH + 3,  7);
	printf("종료하려면 Q를 누르세요");
	gotoxy(FIELD_WIDTH + 3,  9);
	printf("조작은 화살표키로");

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

//아이템(골뱅이)와 웜의 헤드가 만났는지 검사, 
//delItemNo는 먹은 아이템을 지우는 함수로 넘겨줄 변수
int CheckItemHit(pWORM wormHeadPointer, pITEM itemNode, int* delItemNo)
{
	pITEM curr;
	int nodeNo = 0;
	curr = itemNode->next;
	while (curr != NULL)
	{
		if (wormHeadPointer->x == curr->x && wormHeadPointer->y == curr->y)
		{
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

//실행시 테스트용으로 만들어 놓은 아이템 생성 리스트 출력함수
/* 
void PrintItemList(pITEM itemNode)
{
	pITEM curr;
	curr = itemNode->next;
	gotoxy( -LEFT_MARGIN, 2);
	while (curr != NULL)
	{
		printf("아이템번호 : %d\n", curr->itemNo);
		curr = curr->next;
	}
}
*/
int main()
{	
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	//system("mode 650");
	//system("mode con:cols=150 lines=40");
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
	SetConsoleTitle("준호수의 Snake-Game");
	
	restarting:
	pWORM wormHeadNode = malloc(sizeof(WORM));//이중연결리스트 헤드노드
	pWORM wormTailNode = malloc(sizeof(WORM));//이중연결리스트 테일노드
	pWORM addWorm = malloc(sizeof(WORM));//첫번째 웜몸통
	pITEM itemNode = malloc(sizeof(ITEM));//아이템용 단일 연결리스트
	char restart = 0;

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
	
	//지렁이 게임시작 지렁이 생성
	for(int i = 9; i>0 ; i--)
		AddWorm(wormTailNode);
	
	//웜의 머리를 가리키는 포인터
	pWORM wormHeadPointer = addWorm;

	int score = 0;			//최초점수
	int itemCounter = 0;	//아이템 생성 한도 카운터
	char key;				//키입력받을 변수
	int delItemNo=0;		//지울아이템넘버를 받을 변수초기화
	int itemNo = 10000		;//아이템의 최초번호

	//아이템 생성 위치 난수 시드
	srand((unsigned int)time(NULL));
	
	system("cls");	//화면지우고
	PrintField();	//필드 출력


	while (1)
	{
		//테스트용 출력부분
		//gotoxy(-LEFT_MARGIN, 0);
		//printf("먹은 아이템 : %d\n",delItemNo);
		//PrintItemList(itemNode);

		if (_kbhit() != 0)
		{
			
			key = _getch();
			if (key == 'q' || key == 'Q')
			{
				printf("%c", key);
				break;
			}
			if (key == 's' || key == 'S') {
				gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
				printf("일시정지 상태!\n");
				gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2 + 1);
				system("pause");
				system("cls");
				PrintField();
			}
			if (key == LEFT && wormHeadPointer->direction != RIGHT)
			{
				wormHeadPointer->direction = LEFT;
			}
			else if (key == RIGHT && wormHeadPointer->direction != LEFT)
			{
				wormHeadPointer->direction = RIGHT;
			}
			else if (key == UP && wormHeadPointer->direction != DOWN)
			{
				wormHeadPointer->direction = UP;
			}
			else if (key == DOWN && wormHeadPointer->direction != UP)
			{
				wormHeadPointer->direction = DOWN;
			}
		}
		
		//웜 지나간 자리 지우기
		ClearWorm(wormTailNode->next->x, wormTailNode->next->y);
		
		//웜 한칸씩 움직이기
		MoveWorm(wormTailNode, wormHeadNode);

		//벽에 부딛히면 게임오버
		if (wormHeadPointer->x == 0 || wormHeadPointer->x == FIELD_WIDTH || wormHeadPointer->y == 0 || wormHeadPointer->y == FIELD_HEIGHT)
		{
			while(restart != 'r' && restart != 'R' && restart != 'q' && restart != 'Q'){
				system("cls");
				gotoxy(FIELD_WIDTH / 2 -10 , FIELD_HEIGHT / 2);
				printf("벽에 부딛혔습니다. GAME OVER");
				gotoxy(FIELD_WIDTH / 2 -10 , FIELD_HEIGHT / 2 + 1);
				printf("다시 시작하기  R, 프로그램 종료  Q / input :  ");
				scanf_s("%c", &restart);
				if(restart == 'r' || restart == 'R'){
					getchar();
					FreeWormList(wormTailNode);
					FreeItemList(itemNode);
					goto restarting;
				}
				else if(restart == 'q' || restart == 'Q'){
					FreeWormList(wormTailNode);
					FreeItemList(itemNode);
					gotoxy(FIELD_WIDTH / 2 -10 , FIELD_HEIGHT / 2 + 2);
					printf("프로그램을 종료합니다");
					gotoxy(FIELD_WIDTH / 2 -10 , FIELD_HEIGHT / 2 + 3);
					system("pause");
					return 0;
				}
			}
		}

		

		//아이템을 생성
		while (itemCounter < ITEM_MAX)
		{
			CreateItem(itemNode,&itemNo);
			itemCounter++;
		}

		//아이템 먹었는지 확인
		if (CheckItemHit(wormHeadPointer, itemNode, &delItemNo))
		{
			AddWorm(wormTailNode);
			delItemFromList(itemNode, findItemNoInList(itemNode,delItemNo));
			score += 100;
			itemCounter--;
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