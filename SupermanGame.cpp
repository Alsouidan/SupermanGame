#include <random>
#include <Windows.h>
#include <MMSystem.h>
#include <glut.h>
#include <cstdlib>
#include <iostream>

using namespace std;

//-----------------

//  Methods Signatures
void drawRect(int x, int y, float w, float h);
void drawCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
void decrementHealth();
void pickUpPowerUp();
void pickUpHealthPowerUp();

//-----------------
int health = 4;
int supermanY = 300; 
int supermanX = 300;  
int powerUpCounter = 0;
int powerUpX=1000;
int powerUpY=1000;
bool powerUpFlag = true;
bool powerUpActive = false;
bool randomize = true;
int buildingHeights[7];
int buildingY [7];
int buildingX[7];
int buildingLocation=1500;
int currentLocation = 1500;
int level = 0;
int counter = 0;
int freeze = 0;
bool didHit[7];
					 //-----------------

void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(1600, 800);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Superman Game");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
	glutMouseFunc(Mouse);       // sets the Mouse handler function; called when a mouse button is clicked
	glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(1, 1, 1, 0);
	gluOrtho2D(0, 1600, 0, 800);

	glutMainLoop();
}
void winLevel() {
	supermanX = 300;
	supermanY = 300;
	randomize = true;
	currentLocation = 1500;
	level++;
}
// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawDiamond(int x, int y) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x+30, y);
	glVertex2f(x+45, y-10);
	glVertex2f(x+15, y-35);
	glVertex2f(x-15, y-10);

	glEnd();
}
void drawHealthPowerUp(int x,int y) {

	if (powerUpFlag)
		glColor3f(1, 0, 0);
	else
		glColor3f(0, 0, 1);
	drawCircle(x, y, 10);
	glColor3f(0, 0, 0);
	drawRect(x - 2, y - 2, 4, 4);
}

void drawRect(int x, int y, float w, float h) {
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}

void drawTri(int x, int y, int w, int h) {
	glBegin(GL_TRIANGLES);
	glVertex2f(x + w * 0.5, y - h);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	

	glEnd();
}
// draws a circle using OpenGL's gluDisk, given (x,y) of its center and its radius r
void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}
bool checkPowerUpCollision() {
	cout << "checking" << endl;
	cout << supermanX << endl;
	if ((powerUpX - 10 >= supermanX  && powerUpX + 10 <= supermanX+40 ) && (powerUpY - 10 >= supermanY && powerUpY + 10 <= supermanY+40 ) ||
		((powerUpX - 10 >= supermanX-13 && powerUpX + 10 <= supermanX-13+70) && (powerUpY - 10 >= supermanY-80 && powerUpY + 10 <= supermanY) )) {
		powerUpX = 1000;
		powerUpY = 1000;
		powerUpActive = false;
		return true;
	}
	else
		return false;
}
bool checkBuildingCollision() {
	for (int i = 0;i < 7;i++) {
		
			if ((((buildingX[i] >= supermanX && buildingX[i] <= supermanX + 40) || (buildingX[i] + 20 >= supermanX && buildingX[i] + 20 <= supermanX + 40)) &&
				((buildingHeights[i] <= supermanY && buildingHeights[i] + buildingY[i] >= supermanY) || (buildingHeights[i] <= supermanY + 40 && buildingHeights[i] + buildingY[i] >= supermanY + 40))) ||
				(((buildingX[i] >= supermanX - 13 && buildingX[i] <= supermanX - 13 + 70) || (buildingX[i] + 20 >= supermanX - 13 && buildingX[i] + 20 <= supermanX - 13 + 70)) &&
				((buildingHeights[i] <= supermanY - 80 && buildingHeights[i] + buildingY[i] >= supermanY - 80) || (buildingHeights[i] <= supermanY && buildingHeights[i] + buildingY[i] >= supermanY)))) {
				cout << "Check Passed ";
				cout << i << endl;
				if (!didHit[i]) {
					didHit[i] = true;
					return true;
				}
			}
			else {
				didHit[i] = false;
			}
		
	}
	return false;
}
// Keyboard handler function
//   key: the key pressed on the keyboard
//   x  : the X coordinate of the mouse cursor at the moment of pressing the key
//   y  : the Y coordinate of the mouse cursor at the moment of pressing the key
void Key(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':supermanY =(supermanY>=800)?supermanY:supermanY+ 10;break;
	case 'a':supermanX = (supermanX <=0) ? supermanX : supermanX - 10;break;
	case 's':supermanY=(supermanY <= 40) ? supermanY : supermanY - 10;break;
	case 'd':supermanX = (supermanX >=1620) ? supermanX : supermanX + 10;break;
	case 'g':decrementHealth();break;
	}
	if (supermanX == 1600) {
		PlaySound(TEXT("lvlUp.wav"), NULL, SND_ASYNC);

		winLevel();
	}
	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();
}
void pickUpHealthPowerUp() {
	health = 4;
}
void decrementHealth() {
	PlaySound(TEXT("hit.wav"), NULL, SND_ASYNC);
	health = (health > 0) ? health - 1 : 0;
	if (health == 0) {
		glClearColor(1, 0, 0, 0);

	}
}

// Mouse handler function
//   button: the mouse button clicked: left, right or middle
//   state:  the state of the button: clicked (down) or released (up)
//   x  : the X coordinate of the mouse cursor at the moment of clicking the mouse button
//   y  : the Y coordinate of the mouse cursor at the moment of clicking the mouse button
void Mouse(int button, int state, int x, int y) {
	//if(x>=powerUpX-10 && x<=powerUpX+10 && abs(800 - y) >=powerUpY -80 && abs(800 - y) <=powerUpY+80)
	if (checkPowerUpCollision()) {
		PlaySound(TEXT("Mario Power Up Sound Effect.wav"), NULL, SND_ASYNC);

		if (powerUpFlag) {
			pickUpHealthPowerUp();
		}
		else
		{
			glClearColor(0, 0, 1, 0);
			freeze = 10;
		}
	}
	glutPostRedisplay();
}

//Timer function is responsible here for regenerating the ball after a certain time interval
void Timer(int value) {
	// ask OpenGL to recall the display function to reflect the changes on the window
	powerUpCounter += 1;
	counter += 1;
	if (powerUpCounter == 30) {
		powerUpCounter = 0;
		powerUpX= rand() % 780;
		powerUpY = rand() % 780;
		powerUpFlag = !powerUpFlag;
		powerUpActive = true;
	}
	if (powerUpCounter == 15) {
		powerUpX =1000;
		powerUpActive = false;
		powerUpY = 1000;
	}
	int interval;
	if (freeze == 0 && health!=0) {
		glClearColor(1, 1, 1, 0);
		switch (level) {
		case 0:interval = 4;break;
		case 1:interval = 3;break;
		case 2:interval = 2;break;
		default:interval = 1000;break;
		}
		if (level == 3) {
			glClearColor(0, 1, 0, 0);
		}
		if (counter % interval == 0) {
			currentLocation -= 60;
		}
		if (currentLocation == 0) {
			PlaySound(TEXT("lvlUp.wav"), NULL, SND_ASYNC);

			winLevel();

		}
	}
	else
		freeze--;
	glutPostRedisplay();

	// recall the Timer function after 20 seconds (20,000 milliseconds)
	glutTimerFunc( 1000, Timer, 0);
}
void drawBuild(int x, int y,int h) {
	glColor3f(0.623, 0.345, 0.259);
	drawRect(x, y, 20, h);
	glColor3f(0, 0, 0);
	drawCircle(x + 10, y + 10, 10);
	glColor3f(1, 0, 0);
	drawTri(x , y + 40, 20, 40);

		
}
void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 1, 0);
	drawRect(1400, 750, 150 * ((float)health / (float )4), 30);
	//Drawing steps to control the different arm parts' movement:
	// 1) draw rectangle 1 (vertical rectangle 1)
	// 2) push
	// 3) translate bar1Y (horizontal bar 1)
	// 4) draw rectangle 2(horizontal bar 1)
	// 5) draw rectangle 3 (vertical rectangle 2)
	// 6) push
	// 7) translate bar2Y(horizontal bar 2)
	// 8) draw rectangle 4 (horizontal bar 2)
	// 9) draw rectangle 5 (vertical rectangle 3)
	// 10) push
	// 11) translate bar3Y(horizontal bar 3)
	// 12) draw rectangle 6 (horizontal bar 3)
	// 13) pop
	// 14) pop
	// 15) pop


	//Draw circle of radius 15 and x-coordinate of its center is 380. Its y-coordinate is a variable named ballY that will change randomly in the window.

	//----------------------------------------------------------------------------------------------------------------------------------------//
	glColor3f(1, 0, 0);
	drawTri(supermanX - 45, supermanY, 130, 70);
	glColor3f(0.945, 0.761, 0.49);
	drawRect(supermanX, supermanY, 40, 40);
	glPushMatrix();
	glColor3f(0, 0, 0);
	drawCircle(supermanX + 10, supermanY+20 ,3);
	glColor3f(0, 0, 0);
	drawCircle(supermanX + 30, supermanY +20,3);
	glColor3f(0.207, 0.317, 0.529);
	drawRect(supermanX -13, supermanY - 80, 70, 80);
	glPushMatrix();
	glColor3f(1, 0, 0);
	drawDiamond(supermanX+8, supermanY-20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	drawHealthPowerUp(powerUpX, powerUpY);
	if (randomize) {
		for (unsigned int i = 0;i < 7;i++) {
			didHit[i] = false;
			buildingHeights[i] = rand() % 400;
			if (buildingHeights[i] > 140) {
				buildingY[i] = rand() % 100 + 580;
			}else
				buildingY[i] = rand() % 100 + 580- buildingHeights[i];
		}
		randomize = false;
	}
	buildingLocation = currentLocation;

	for (int i = 0;i < 7;i++) {
		//drawBuild(buildingLocation, buildingHeights[i],buildingY[i]);
		drawBuild(buildingLocation, buildingHeights[i], buildingY[i]);
		buildingX[i]=buildingLocation;
		buildingLocation -= 160;
	}
	if (checkBuildingCollision()) {
		decrementHealth();
	}

	glFlush();
}