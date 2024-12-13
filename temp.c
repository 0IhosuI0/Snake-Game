#include <stdio.h>	//ǥ����������
#include <stdlib.h>	//malloc, free ��
#include <Windows.h>//windows console api ���
#include <time.h>	//���� �õ�� time �Լ� ���
#include <conio.h>	//gotoxy���� ����� api ���

#define FIELD_WIDTH 70	//�ʵ尡�α���
#define FIELD_HEIGHT 25	//�ʵ弼�α���
#define MENU_WIDTH 83
#define MENU_HEIGHT 40
#define LEFT 75			//Ű���� �� ȭ��ǥ�� char��
#define RIGHT 77		//Ű���� ��
#define UP 72			//Ű���� ��
#define DOWN 80			//������ �Ʒ�
#define ITEM_MAX 10		//ȭ�鿡 ǥ�õǴ� �����۰���
#define ITEM_GOLD 101	//��� ������ �ε�������
#define ITEM_EXP 102	//����ġ ������ �ε�������
#define LEFT_MARGIN 30	//ȭ����ʸ���(����)
#define TOP_MARGIN 10	//ȭ�� ��ܸ���(����)
//#define DELAYTIME 100	//Sleep�Լ��� �� x/1000 ��

int DELAYTIME = 100;
int NowSpeed = 100;
int Select = 0;
int cntTail = 9;

//�����̸� ������ ���߿��Ḯ��Ʈ ����ü
#pragma pack(push,1)
typedef struct _WORM
{
	int x;	//x��ǥ
	int y;	//y��ǥ
	char direction;	//�������
	struct _WORM *next;	// ��������ּ�
	struct _WORM *before;//��������ּ�
}WORM, *pWORM;

//�������� ������ ���Ͽ��Ḯ��Ʈ ����ü
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

//Ŀ���� ���� ��ǥ�� �̵�
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x + LEFT_MARGIN;//���μ��� ������ ���߱� ���ؼ� ������ ��ǥ�� 2��
	Pos.Y = y + TOP_MARGIN;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void SetColor(int num){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
}

//���ӿ������
void PrintField() 
{
	for (int i = 1; i < FIELD_WIDTH; i++)
	{
		gotoxy(i, 0);
		printf("��");
		gotoxy(i, FIELD_HEIGHT);
		printf("��");
	}

	for (int i = 1; i < FIELD_HEIGHT; i++)
	{
		
		gotoxy(0, i);
		printf("��");
		gotoxy(FIELD_WIDTH,i);
		printf("��");
	}
	gotoxy(0, 0);
	printf("��");
	gotoxy(0, FIELD_HEIGHT);
	printf("��");
	gotoxy(FIELD_WIDTH, 0);
	printf("��");
	gotoxy(FIELD_WIDTH, FIELD_HEIGHT);
	printf("��");
}

void PrintMenu() 
{
	for (int i = 13; i < MENU_WIDTH; i++)
	{
		gotoxy(i, 15);
		printf("��");
		gotoxy(i, MENU_HEIGHT);
		printf("��");
	}

	for (int i = 15	; i < MENU_HEIGHT; i++)
	{
		gotoxy(13, i);
		printf("��");
		gotoxy(MENU_WIDTH, i);
		printf("��");
	}
	gotoxy(13, 15);
	printf("��");
	gotoxy(13, MENU_HEIGHT);
	printf("��");
	gotoxy(MENU_WIDTH, 15);
	printf("��");
	gotoxy(MENU_WIDTH, MENU_HEIGHT);
	printf("��");
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

	gotoxy(45, 20);
	printf("����  �ϱ�");
	gotoxy(45, 25);
	printf("�̾�  �ϱ�");
	gotoxy(45, 30);
	printf("����  ����");
	gotoxy(45, 35);
	printf("����  �ϱ�");
}

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
				gotoxy(45, 20); printf("����  �ϱ�");
				SetColor(15); 
				gotoxy(45, 25); printf("�̾�  �ϱ�");
				gotoxy(45, 30); printf("����  ����");
				gotoxy(45, 35); printf("����  ����"); 
				break;
			case 1:
				gotoxy(45, 20); printf("����  �ϱ�");
				SetColor(11);
				gotoxy(45, 25); printf("�̾�  �ϱ�");
				SetColor(15);
				gotoxy(45, 30); printf("����  ����");
				gotoxy(45, 35); printf("����  ����");
				break;
			case 2:
				gotoxy(45, 20); printf("����  �ϱ�");
				gotoxy(45, 25); printf("�̾�  �ϱ�");
				SetColor(11);
				gotoxy(45, 30); printf("����  ����");
				SetColor(15);
				gotoxy(45, 35); printf("����  ����");
				break;
			case 3:
				gotoxy(45, 20); printf("����  �ϱ�");
				gotoxy(45, 25); printf("�̾�  �ϱ�");
				gotoxy(45, 30); printf("����  ����");
				SetColor(11);
				gotoxy(45, 35); printf("����  ����");
				SetColor(15);
				break;
			default: 
				break;
		}
		Sleep(DELAYTIME);
	}
}
void PrintGuide() 
{
	for (int i = 1; i < MENU_WIDTH; i++)
	{
		gotoxy(i, 1);
		printf("��");
		gotoxy(i, MENU_HEIGHT);
		printf("��");
	}

	for (int i = 1	; i < MENU_HEIGHT; i++)
	{
		gotoxy(1, i);
		printf("��");
		gotoxy(MENU_WIDTH, i);
		printf("��");
	}
	gotoxy(1, 1);
	printf("��");
	gotoxy(1, MENU_HEIGHT);
	printf("��");
	gotoxy(MENU_WIDTH, 1);
	printf("��");
	gotoxy(MENU_WIDTH, MENU_HEIGHT);
	printf("��");
	
	gotoxy(35, 4);
	printf("<���� ��Ģ>");
	gotoxy(10, 9);
	printf("1. ������ ����Ű(��, ��, ��, ��)�� �Ѵ�.");
	gotoxy(10, 17);
	printf("2. ������ ����");
	gotoxy(15, 19);
	printf("@ : �ӵ� ����");
	gotoxy(15, 21);
	printf("# : �ӵ� ����");
	gotoxy(15, 23);
	printf("$ : ���� 2�� ����");
	gotoxy(15, 25);
	printf("& : ���� 1�� ����");
	gotoxy(MENU_WIDTH - 15, MENU_HEIGHT - 3);
	SetColor(11);
	printf("��Ģ �ݱ�");
	SetColor(15);
}

//�����̸� �ø��� �Լ�(���߿��Ḯ��Ʈ�� �����ʿ� ��� �߰�)
void AddWorm(pWORM wormTailNode)
{
	pWORM newNode = malloc(sizeof(WORM));
	pWORM temp = wormTailNode->next;

	//�� ����� �ּڰ� �̾��ֱ�;
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

//���Ḯ��Ʈ�� �޾Ƽ� ������ ���
void PrintWorm(pWORM wormTailNode, pWORM wormHeadNode)
{
	pWORM curr = wormTailNode->next;
	while (curr != wormHeadNode)
	{
<<<<<<< HEAD
		if(curr->next == wormHeadNode){
			gotoxy(curr->x, curr->y);
			printf("O");
		}
		else{
			gotoxy(curr->x, curr->y);
			printf("o");
		}
=======
		if (curr->next == wormHeadNode) {
			gotoxy(curr->x, curr->y);
			printf("O");
		}
		else {
			gotoxy(curr->x, curr->y);
			printf("o");
		}

>>>>>>> a156a5582e1693ba3c65d9b346b02dca3e7bbb3e
		curr = curr->next;
	}
}

//�������� ���
void PrintScore(int score)	
{
	gotoxy(FIELD_WIDTH + 3,  3);
	printf("���� : %d��",score);
	gotoxy(FIELD_WIDTH + 3,  5);
	printf("���� ���� : %3d��", cntTail);
	gotoxy(FIELD_WIDTH + 3,  7);
	printf("���� �ӵ� : %3d%%", NowSpeed);
	gotoxy(FIELD_WIDTH + 3, FIELD_HEIGHT - 2); 
	printf("�Ͻ������Ϸ��� O�� ��������");
	gotoxy(FIELD_WIDTH + 3,  FIELD_HEIGHT - 3);
	printf("�����Ϸ��� Q�� ��������");
}

//���� ������ �ڸ� �����
void ClearWorm(int x, int y)
{
	gotoxy(x, y);
	printf(" ");
}

//�� �����̱� 
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


//�� ����Ʈ�� �Ҵ�� �޸𸮸� ������ ����
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

//item �߻���Ű��
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

//������ ȭ�鿡 ���
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

//����Ʈ���� itemNo�� ��ġ�� ã�Ƽ� ī���͸� ����
int findItemNoInList(pITEM itemNode, int itemNo)	
{
	int numberOfNode = 0;	//����ȣ�� ������ ����

	pITEM curr;		//�����̸� ������ ������
	curr = itemNode->next;		//ù��° �����͸� ����Ű�� �ּҸ� curr�� �Ҵ�

	while (curr != NULL)	//�ݺ����� �����鼭 ã�� �����͸� �߰��ϸ� ����ȣ�� ����
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



//�������� ����ȣ�� �޾� �� ��ȣ�� ��带 ����
int delItemFromList(pITEM itemNode, int targetNodeNumber)
{	
	pITEM beforeTarget;		//������ ����� �ճ��
	pITEM target;		//������ ���

	int counter = 0;

	beforeTarget = itemNode;

	if (targetNodeNumber < 0)   // 0�� �ԷµǸ� FirstNode�� �����ϱ� ������ �������� �ʰ� ����
		return 0;

	//��带 targetNodeNumber-1 ��ŭ �̵����Ѽ� beforeTarget�� ������ ��� ������ �̵���Ŵ
	while (counter < targetNodeNumber - 1)
	{
		beforeTarget = beforeTarget->next;
		counter = counter + 1;
	}

	//��带 ����Ʈ���� ����
	target = beforeTarget->next;
	beforeTarget->next = target->next;
	free(target);
}

//������(�����)�� ���� ��尡 �������� �˻�, 
//delItemNo�� ���� �������� ����� �Լ��� �Ѱ��� ����
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
				NowSpeed += 10;
				DELAYTIME -= 10;
				cntTail ++;
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[@] ��(��) �����ϰ� �ӵ��� 10%% �����Ͽ����ϴ�.");

			}
			else if (curr->ITEMt == '#') {
				NowSpeed -= 10;
				DELAYTIME += 10;
				cntTail ++;
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[#] ��(��) �����ϰ� �ӵ��� 10%% �����Ͽ����ϴ�.");
			}
			else if (curr->ITEMt == '$') {
				AddWorm(wormTailNode);
				cntTail ++;
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[$] ��(��) �����ϰ� ������ 2�� �����Ͽ����ϴ�.    ");
			}
			else if (curr->ITEMt == '&') {
				CleanTail(wormTailNode);
				CleanTail(wormTailNode);
				cntTail --;
				gotoxy(FIELD_WIDTH + 3,  17);
				printf("[&] ��(��) �����ϰ� ������ �����Ͽ����ϴ�.    ");
			}

			if (curr->itemType == ITEM_EXP)
			{
				*delItemNo = curr->itemNo;
				return 1; //������ ������ 1����
			}
		}
		nodeNo++;
		curr = curr->next;
	}
	return 0;//�������� �ȸ����� 0
}

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
	return 0; //������ �ȸ����� 0
}


//�������� ��ũ�� ����Ʈ �޸� ����
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
				printf("�ٽ�  ����");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				SetColor(15); 
				printf("����  ����");
				break;
			case 1:
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				printf("�ٽ�  ����");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				SetColor(11);
				printf("����  ����");
				SetColor(15); 
				break;
			default: 
				break;
		}
		Sleep(DELAYTIME);
	}
}	

//����� �׽�Ʈ������ ����� ���� ������ ���� ����Ʈ ����Լ�
/* 
void PrintItemList(pITEM itemNode)
{
	pITEM curr;
	curr = itemNode->next;
	gotoxy( -LEFT_MARGIN, 2);
	while (curr != NULL)
	{
		printf("�����۹�ȣ : %d\n", curr->itemNo);
		curr = curr->next;
	}
}
*/

void Save(int score) {
	char fileName[50] = { 0 };
	FILE * fp = fopen("Save.sv", "wt");

	fprintf(fp, "%d\n", score);
	fprintf(fp, "%d\n", DELAYTIME);
	fprintf(fp, "%d\n", NowSpeed);
	fprintf(fp, "%d", cntTail);

	fclose(fp);
	gotoxy(FIELD_WIDTH / 2 - 10, 7); 
	printf("���� �Ϸ�");
}

int Load(){
	int score = 0;
	char fileName[50] = { 0 };
	FILE * fp = fopen("Save.sv", "rt");
	if (fp == NULL) return -1;

	if (fscanf_s(fp, "%d", &score) != 1) {
        score = 0; // �б� ���� �� �⺻�� ����
    }

    if (fscanf_s(fp, "%d", &DELAYTIME) != 1) {
        DELAYTIME = 100; // �б� ���� �� �⺻��
    }

    if (fscanf_s(fp, "%d", &NowSpeed) != 1) {
        NowSpeed = 100; // �б� ���� �� �⺻��
    }
	if (fscanf_s(fp, "%d", &cntTail) != 1) {
        cntTail = 9; // �б� ���� �� �⺻��
    }
    
	fclose(fp);
	return score;
}

int PrintLoadErr(){
	gotoxy(39, 25);
	SetColor(11);
	printf("����� �����Ͱ� �����ϴ�!");
	SetColor(15);
	while(1){
		if(GetAsyncKeyState(VK_RETURN)){
			return 0;
		}
	}
}


int main()
{	
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	SetConsoleTitle("��ȣ���� Snake-Game");
	
	DeleteCursor();
	int Select, Restart, score = 0;
	
	menu:
	
	system("cls");	//ȭ�������
	PrintMenu();	//�ʵ� ���
	Select = SelectMenu();
	if(Select == 0){
		starting:
		pWORM wormHeadNode = malloc(sizeof(WORM));//���߿��Ḯ��Ʈ �����
		pWORM wormTailNode = malloc(sizeof(WORM));//���߿��Ḯ��Ʈ ���ϳ��
		pWORM addWorm = malloc(sizeof(WORM));//ù��° ������
		pITEM itemNode = malloc(sizeof(ITEM));//�����ۿ� ���� ���Ḯ��Ʈ
		

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
		//������ ���ӽ��� ������ ����
		for(int i = cntTail; i>0 ; i--)
			AddWorm(wormTailNode);

		//���� �Ӹ��� ����Ű�� ������
		pWORM wormHeadPointer = addWorm;

	
		int itemCounter = 0;	//������ ���� �ѵ� ī����
		char key;				//Ű�Է¹��� ����
		int delItemNo=0;		//��������۳ѹ��� ���� �����ʱ�ȭ
		int itemNo = 10000;//�������� ���ʹ�ȣ

		//������ ���� ��ġ ���� �õ�
		srand((unsigned int)time(NULL));

		while (1)
		{
			//�׽�Ʈ�� ��ºκ�
			//gotoxy(-LEFT_MARGIN, 0);
			//printf("���� ������ : %d\n",delItemNo);
			//PrintItemList(itemNode);

			if (GetAsyncKeyState(0x51))
			{
				break;
			}
			if (GetAsyncKeyState(0x4F)) {
				gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2);
				printf("�Ͻ����� ����!\n");
				gotoxy(FIELD_WIDTH / 2 - 10, FIELD_HEIGHT / 2 + 1);
				system("pause");
				system("cls");
				PrintField();
			}
			if (GetAsyncKeyState(0x56)){
				Save(score);
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
			
			
			//�� ������ �ڸ� �����
			ClearWorm(wormTailNode->next->x, wormTailNode->next->y);
			
			//�� ��ĭ�� �����̱�
			MoveWorm(wormTailNode, wormHeadNode);

			//���� �ε����� ���ӿ���
			if (wormHeadPointer->x == 0 || wormHeadPointer->x == FIELD_WIDTH || wormHeadPointer->y == 0 || wormHeadPointer->y == FIELD_HEIGHT)
			{
				system("cls");
				gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
				printf("���� �ε������ϴ�. GAME OVER");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				printf("�ٽ�  ����");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				printf("����  ����");
				Restart = SelRestart();

				switch(Restart){
					case 0:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						score = 0;
						DELAYTIME = 100;
						NowSpeed = 100;
						cntTail = 9;
						goto starting;
					
					case 1:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
						printf("���α׷��� �����մϴ�");
						gotoxy(FIELD_WIDTH / 2 - 13 , FIELD_HEIGHT / 2 + 9);
						system("pause");
						return 0;
				}
			}

			

			//�������� ����
			while (itemCounter < ITEM_MAX)
			{
				CreateItem(itemNode,&itemNo);
				itemCounter++;
			}

			//������ �Ծ����� Ȯ��
			if (CheckItemHit(wormHeadPointer, itemNode, &delItemNo, wormTailNode))
			{
				AddWorm(wormTailNode);
				delItemFromList(itemNode, findItemNoInList(itemNode,delItemNo));
				score += 100;
				itemCounter--;
			}
			if (CheckWormHit(wormHeadPointer) == 1){
				system("cls");
				gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2);
				printf("���� �ε������ϴ�. GAME OVER");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 2);
				printf("�ٽ�  ����");
				gotoxy(FIELD_WIDTH / 2 - 2 , FIELD_HEIGHT / 2 + 4);
				printf("����  ����");
				Restart = SelRestart();

				switch(Restart){
					case 0:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						score = 0;
						DELAYTIME = 100;
						NowSpeed = 100;
						cntTail = 9;
						goto starting;
					
					case 1:
						FreeWormList(wormTailNode);
						FreeItemList(itemNode);
						gotoxy(FIELD_WIDTH / 2 - 8 , FIELD_HEIGHT / 2 + 8);
						printf("���α׷��� �����մϴ�");
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
		printf("���α׷��� �����մϴ�");
		gotoxy(FIELD_WIDTH / 2 - 10 , FIELD_HEIGHT / 2 + 1);
		system("pause");
		system("cls");
		return 0;
	}

	else if(Select == 1){
			if(Load() == -1) PrintLoadErr();
			else {
				score = Load();
				goto starting;
			}
			goto menu;
			
		}
		
	else if(Select == 2){
		system("cls");
		PrintGuide();
		while(1){
			if(GetAsyncKeyState(VK_RETURN)) break;
		}
		goto menu;
	}	
	else if(Select == 3){
		//FreeWormList(wormTailNode);
		//FreeItemList(itemNode);
		system("cls");
		return 0;
	}	
}