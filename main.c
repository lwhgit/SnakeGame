#include <stdio.h>
#include <windows.h>
#include <time.h>

typedef struct _Point {
	int x;
	int y;
} Point;

void startGame();
int getKey();
void hideCursor();
void gotoxy(int, int);
int checkMap(int, int);
void handleKeyEvent();
void handle();
void handleGraphic();
Point getNextPoint();

const KEY_UP = 0x48;
const KEY_LEFT = 0x4b;
const KEY_DOWN = 0x50;
const KEY_RIGHT = 0x4d;

Point playerTrace[28 * 28];
int end = 0;
int head = 5;

int playerX = 30;
int playerY = 15;
int direction = 0;

int feedX = -1;
int feedY = -1;

int main() {
	srand(time(0));
	
	hideCursor();
	system("mode con cols=100 lines=30");
	
	printf("Press any key to play.");
	getKey();
	
	startGame();
	
	getKey();
	
	return 0;
}

void startGame() {
	system("cls");
	int i = 0;
	for (i = 0;i < 60;i += 2) {
		setTile(i, 0, "▦\0");
		setTile(i, 29, "▦\0");
	}
	for (i = 0;i < 30;i ++) {
		setTile(0, i, "▦\0");
		setTile(58, i, "▦\0");
	}
	
	createFeed();
	while (1) {
		Sleep(100);
		handleKeyEvent();
		handle();
		handleGraphic();
	}
}

int getKey() {
	int key = getch();

	if (key == 0xE0) {
		key = getch();
		return key;
	} else {
		return key;
	}
}

void hideCursor() {
	CONSOLE_CURSOR_INFO consoleCursor;
	consoleCursor.bVisible = 0;
	consoleCursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleCursor);
}

void gotoxy(int x, int y) {
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setTile(int x, int y, char* s) {
	gotoxy(x, y);
	printf("%s", s);
}

void createFeed() {
	int i = 0;
	int length = 28 * 28 - (head - end);
	Point *emptyPoint = (Point*) malloc(sizeof(Point) * length);
	gotoxy(80, 5);
	printf("%d", length);
	
	int x = 2;
	int y = 2;
	int n = 0;
	for (;x < 56;x += 2) {
		for (y = 2;y < 28;y ++) {
			if (checkMap(x, y) == 1) {
				emptyPoint[i].x = x;
				emptyPoint[i].y = y;
				i ++;
			}
		}
	}
	
	int ran = rand() % length;
	Point target = emptyPoint[ran];
	feedX = target.x;
	feedY = target.y;
	setTile(feedX, feedY, "●\0");
	gotoxy(80, 10);
	printf("                       ");
	gotoxy(80, 10);
	printf("%d, %d", feedX, feedY);
	
	free(emptyPoint);
}

int checkMap(int x, int y) {
	int i = 0;
	for (;i < 28 * 28;i ++) {
		if (playerTrace[i].x == x && playerTrace[i].y == y) {
			return 0;
		}
	}
	
	return 1;
}

void handleKeyEvent() {
	if (kbhit()) {
		int keycode = getKey();
		
		if (keycode == KEY_UP && (direction == 1 || direction == 3)) {
			direction = 0;
		} else if (keycode == KEY_LEFT && (direction == 0 || direction == 2)) {
			direction = 1;
		} else if (keycode == KEY_DOWN && (direction == 3 || direction == 1)) {
			direction = 2;
		} else if (keycode == KEY_RIGHT && (direction == 2 || direction == 0)) {
			direction = 3;
		}
	}
}

void handle() {
	end ++;
	if (playerX == feedX && playerY == feedY) {
		end --;
		createFeed();
	}
	head ++;
	if (end == 28 * 28) {
		end = 0;
	}
	if (head == 28 * 28) {
		head = 0;
	}
	playerTrace[head] = getNextPoint(direction);
}

void handleGraphic() {
	if (end >= 0) {
		Point p = playerTrace[end];
		setTile(p.x, p.y, " \0");
		playerTrace[end].x = 0;
		playerTrace[end].y = 0;
		
	}
	if (head >= 0) {
		Point p = playerTrace[head];
		setTile(p.x, p.y, "■\0");
	}
}

Point getNextPoint(int dir) {
	if (dir == 0) {
		playerY --;
	} else if (dir == 1) {
		playerX -= 2;
	} else if (dir == 2) {
		playerY ++;
	} else if (dir == 3) {
		playerX += 2;
	}
	Point p = {playerX, playerY};
	return p;
}
