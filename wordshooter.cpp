#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<math.h>
#include<cmath>
#include "util.h"
#include<fstream>
using namespace std;
int boardarray[10][15];  //making array to manage the board.
int ball[1000];        //to store the random the elements to be used for the ball placed on the shot gun.
float x1 = 465;      // x-coordinate of ball placed at shoot gun
int y1cor = 10;   //x-coordinate of ball placed at shoot gun
int row;     //for getting the row from pixels
int col;     //for getting the col from pixels
int roww=0;  // for changing the ball at shot gun  
bool click = false;    //bool for click
float slope;     //variable to calculate slope
int timee=123;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 26 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;
int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...

void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookie data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}


void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
int cheight = 60)
/*Draws a specfic cookie at given position coordinate
* sx = position of x-axis from left-bottom
* sy = position of y-axis from left-bottom
* cwidth= width of displayed cookie in pixels
* cheight= height of displayed cookiei pixels.
* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
glPopMatrix();

//glutSwapBuffers();

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int a, b, c, d = 0;
//upper_it = upper iteration check
int upper_it = 0;
int SizeOfArray = 0;
char VerticalCombinationArray[10] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; // array to store letters of column 
char HorizontalCombinationArray[15];
bool WholeCheck = 0, UFlag, DFlag;
int colcheck, rowcheck;
int  s; //number of spaces check variable 
void CombinationsChecks()   //function for checking combinations
{
	DFlag = 0;   // break inner loop condition
	UFlag = 0;   //break upper loop condition

	SizeOfArray = 0;
	WholeCheck = 0;   
	for (a = 0; a < 10; a++)
	{
		VerticalCombinationArray[a] = ' ';
	}


	for (b = 0; b <= rowcheck; b++)
	{
		VerticalCombinationArray[b] = char(boardarray[b][colcheck] + 97);
	}

	for (b = 0; VerticalCombinationArray[b] != ' '; b++)
	{
		++SizeOfArray;
	}

	upper_it = 0;
	if (WholeCheck == 0)
	{



		for (upper_it = 0; upper_it < SizeOfArray - 2 && UFlag == 0; upper_it++)
		{
			



			for (a = 0; a < dictionarysize; a++)
			{



				if (dictionary[a].length() == SizeOfArray - upper_it)



				{


					DFlag = 0;
					for (b = upper_it; b < SizeOfArray && DFlag == 0; b++)
					{


						if (VerticalCombinationArray[b] != dictionary[a][b - upper_it])
						{
							DFlag = 1;
						}

					}
					if (DFlag == 0)
					{



						//score addition
						score += SizeOfArray - upper_it;
						// reverting condition
						WholeCheck = 1;
						// Upper flag for break
						UFlag = 1;
						for (d = upper_it; d < SizeOfArray; d++)
							boardarray[d][colcheck] = 27;
					}



				}



			}


		}


	}

	
	SizeOfArray = 0;
	
	s = 0;
	DFlag = 0;
	UFlag = 0;


	for (a = 0; a < 15; a++)
	{
		HorizontalCombinationArray[a] = ' ';
	}

	//conversion into character 

	for (b = 0 ; b <= colcheck; b++)
	{
		HorizontalCombinationArray[b] = char(boardarray[rowcheck][b] + 97);
	}



	for (b = 0; b < 15; b++)
	{
		if (HorizontalCombinationArray[b] != char(124) && HorizontalCombinationArray[b] != ' ')
		{
			SizeOfArray++;
		}
	}

	for (c = 0; c < colcheck; c++)
	{
		if (HorizontalCombinationArray[c] == char(124))
		{
			s += 1;
		}
	}
	
	
upper_it = 0;
if (WholeCheck == 0)
{
	for (upper_it = 0; upper_it < SizeOfArray - 2 && UFlag == 0; upper_it++)
	{
		for (a = 0; a < dictionarysize; a++)
		{
			if (dictionary[a].size() == SizeOfArray - upper_it)

			{
				DFlag = 0;
				for (b = upper_it; b < SizeOfArray && DFlag == 0; b++)
				{
					if (HorizontalCombinationArray[s+b] != dictionary[a][b - upper_it])
					{
						DFlag = 1;
					}

				}
				if (DFlag == 0)
				{
					UFlag = 1;
					score += SizeOfArray - upper_it;
					WholeCheck = 1;

					for (d = upper_it; d < SizeOfArray; d++)
						boardarray[rowcheck][s + d] = 27;
				}

			}
		}
	}

}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GetAlphabet() {
	return GetRandInRange(0, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy)
{

	cy = px/ 60;    // for column(width)
	cx = (height-py)/60;   //for row(height)
	
	//cy = (px - 15) / 60;
	//cx = (height-(py-50))/60;
	/*int y = 560;
	for (int i = 0; i < 11; i++)
	{
	int x = 15;
	for (int j = 0; j < 15; j++)
	{
	if (px>x && px<x+60)
	if (py>y && py < y + 60)
	{
	cx = i;
	cy = j;
	break;
	}
	x += 60;
	}
	y+=60;
	}*/

}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
/*char currentAlphabetArr[10];
int a, b, c, d;
int bcheck;
void checkCombinations()
{
	//boardarray[row][col]
	//veritcal upward check
	for (a = 0; boardarray != ' ';a++)
//




}*/



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void move()   //function for moving the ball and collision and appening the shooter ball in the array and stopping it before next ball
{ 

	Pixels2Cell(x1, y1cor, row, col);   
	row = row - 2;    //to check the space after two rows from the current one 
	if (boardarray[row][col]>25)     // if there's a space, keep moving
	{
		if (x1 <= 15 || x1 >= width - 60)    //for bouncing
		{
			slope = -1 * slope;
		}
		x1 += (1 / slope);
		y1cor += 1;
		glutPostRedisplay();
	}
	else
	{
		row = row +1;   //if there's no space place in the next row 
		boardarray[row][col] = ball[roww];
		rowcheck = row; colcheck = col;
		glutPostRedisplay();
		CombinationsChecks();
		x1 = 465;   //reset the x-coordinate of the ball at the shot gun
		y1cor = 10;  //reset the y-coordinate of the ball at the shot gun
		roww++;   // for getting the next ball
		click = false;
	}
	int column = col;//storing the value of col for the further use 
	col = col - 1;   // check for the ball coming from the right
	if (boardarray[row][col]>25)
	{
		if (x1 <= 15 || x1 >= width - 60)
		{
			slope = -1 * slope;
		}
		x1 += (1 / slope);
		y1cor += 1;
		glutPostRedisplay();
	}
	else
	{
		col = col+1; //stop and place the ball if there's no space at upper check
		boardarray[row][col] = ball[roww];
		rowcheck = row; colcheck = col;
		glutPostRedisplay();
		CombinationsChecks();
		x1 = 465;
		y1cor = 10;
		roww++;
		click = false;
	}

	column = column+1;   //check for the ball moving from left to right 
	if (boardarray[row][column]>25)
	{
		if (x1 <= 15 || x1 >= width - 60)
		{
			slope = -1 * slope;
		}
		x1 += (1 / slope);
		y1cor += 1;
		glutPostRedisplay();
	}
	else
	{
		column = column - 1;  //stop and place the ball if there's no space 
		boardarray[row][column] = ball[roww];
		rowcheck = row; colcheck = col;
		glutPostRedisplay();
		CombinationsChecks();
		x1 = 465;
		y1cor = 10;
		roww++;
		click = false;
	}



	/*if (x1 <= 15 || x1 >= width - 60)
	{
		slope = -1*slope;
	}
	x1 += (1 / slope);
	y1cor += 1;
	glutPostRedisplay();

	if (y1cor >= height - 110 || collide)
	{
		x1 = 465;
		y1cor = 10;
		roww++;
		click = false;



	}
	
	//glutPostRedisplay();*/



	/*x1 += deltax;
	if (x1 <= 15 || x1 >= width - 60){
		deltax *= -1;
	}
	y1cor += deltay;
	glutPostRedisplay();
	if (y1cor >= height - 110 || collide)
	{
		x1 = 465;
		y1cor = 10;
		deltax = 0;
		deltay = 0;
		roww++;
		click = false;



	}
	glutPostRedisplay();*/

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void collision()
{



	/*rowb = y1cor / 60;
	column=x1 / 60;
	if (height_array[column] == y1cor)
	{
		stop = true;
		value1 = rowb;
		value2 = column;
	}
	if (stop == true)
	{
		boardarray[value1][value2] = ball[roww];
	}*/
	

	/*Pixels2Cell(x1, y1cor, row, col);
	if (boardarray[row + 1][col] >= 0 && boardarray[row + 1][col] <= 25 && boardarray[row][col]==27)
	{
		boardarray[row][col] = ball[roww];
	}




	int index;
	int index2;
	for (; row >=-1; row--)
	{
		for (; col<15; col++)
		{
			if (boardarray[row][col] > 0 && boardarray[row][col] < 27)
			{
				index = row-1;
				index2 = col;
			}
		}
	}
	boardarray[index][index2] = ball[roww];






	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (((y1cor - boardarray[i][j]) >= 0 && (x1 - boardarray[i][j]) <= 59) && ((x1 - boardarray[i][j]) >= 0 && (x1 - boardarray[i][j]) <= 59) && boardarray[i][j] != 27)
				{
					boardarray[i][j] = ball[roww];
                }
		}
	}*/
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)
 
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
/*
* Main Canvas drawing function.
* */
bool endcheck = 0;
void DisplayFunction() {
	glClearColor(0.5/*Red Component*/, 0.7/*Green Component*/,
		0.6/*Blue Component*/, 1.0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	for (int boardcheck = 0; boardcheck < 15; boardcheck++)
	{
		if (boardarray[9][boardcheck] != 27)
			endcheck = 1;
	}
	if (timee > 120 && timee <= 123 )   //for startup screen
	{
		glClearColor(1/*Red Component*/, 1/*Green Component*/,
			1/*Blue Component*/, 1.0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
		glClear(GL_COLOR_BUFFER_BIT); //Update the colors



		DrawAlphabet((alphabets)22, 120, 430, awidth, aheight);
		DrawAlphabet((alphabets)14, 180, 430, awidth, aheight);
		DrawAlphabet((alphabets)17, 240, 430, awidth, aheight);
		DrawAlphabet((alphabets)3, 300, 430, awidth, aheight);
		DrawAlphabet((alphabets)18, 380, 430, awidth, aheight);
		DrawAlphabet((alphabets)7, 440, 430, awidth, aheight);
		DrawAlphabet((alphabets)14, 500, 430, awidth, aheight);
		DrawAlphabet((alphabets)14, 560, 430, awidth, aheight);
		DrawAlphabet((alphabets)19, 620, 430, awidth, aheight);
		DrawAlphabet((alphabets)4, 680, 430, awidth, aheight);
		DrawAlphabet((alphabets)17, 740, 430, awidth, aheight);

		DrawString(450, 250, width, height + 5, "LOADING... ", colors[RED]);

	}
	
	else
	{
		if ((timee > 0 && timee <= 120) && endcheck == 0)
		{
			//for the upper two rows
			int widthh;          //initializing the height and width of the alphabets placed on the board
			int heightt = 550;
			for (int i = 0; i < 10; i++)   //loop for getting the random elements which are placed in 2d array
			{
				widthh = 15;

				for (int j = 0; j < 15; j++)
				{
					DrawAlphabet((alphabets)boardarray[i][j], widthh, heightt, awidth, aheight);

					widthh += 60;

				}
				heightt -= 60;
			}

			DrawAlphabet((alphabets)ball[roww], x1, y1cor, awidth, aheight); //for the ball placed on the shot gun
			DrawAlphabet((alphabets)ball[roww + 1], 400, 0, 30, 30);  //for the first next ball
			DrawAlphabet((alphabets)ball[roww + 2], 370, 0, 30, 30);  //for the second next ball

			if (click)
			{
				move();   //calling move function
				glutPostRedisplay();
			}
			/*Pixels2Cell(x1, y1cor, row, col);
			//cout << row << " " << col << endl;
			x1 += deltax;
			if (x1 <= 15 || x1 >= width - 60){
			deltax *= -1;
			}
			y1cor += deltay;
			glutPostRedisplay();*/
			DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
			DrawString(width / 2 - 30, height - 25, width, height, "Time Left:" + Num2Str(timee) + " secs", colors[RED]);
		}
		else  //game over screen
		{
			glClearColor(1/*Red Component*/, 1/*Green Component*/,
				1/*Blue Component*/, 1.0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
			glClear(GL_COLOR_BUFFER_BIT); //Update the colors


			DrawAlphabet((alphabets)6, 240, 330, awidth, aheight);
			DrawAlphabet((alphabets)0, 300, 330, awidth, aheight);
			DrawAlphabet((alphabets)12, 360, 330, awidth, aheight);
			DrawAlphabet((alphabets)4, 420, 330, awidth, aheight);
			DrawAlphabet((alphabets)14, 500, 330, awidth, aheight);
			DrawAlphabet((alphabets)21, 560, 330, awidth, aheight);
			DrawAlphabet((alphabets)4, 620, 330, awidth, aheight);
			DrawAlphabet((alphabets)17, 680, 330, awidth, aheight);

			DrawString(450, 250, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);

		}

	}

	DrawShooter((width / 2) -35, 0, bwidth, bheight);
	glutSwapBuffers();

}

/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{
			click = true;
			slope = (height - (y - y1cor)) / ((x-30) - x1);   //calculating the slope
			

			/*xmouse = x;
			ymouse = y;*/
			//if (!click);
		

			/*if (x>465)
			    deltax = (x - x1) / 80;
			if (x<465)
				deltax = ((x-60) - x1) / 80;

			deltay = (height - (y - 10)) / 80;*/


			
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
		roww += 1;   //condition to change the ball when clicking right button
		glutPostRedisplay();
	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {
	timee -= 1;
	glutPostRedisplay();
	glutTimerFunc(1000, Timer, 0);
}

int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for (int i = 0; i < 5; ++i)
		cout << " word " << i << " =" << dictionary[i] << endl;


	//loop for appending the random elements in the array of board
	for (int i = 0; i < 2; i++)  //for filling first two rows with random elements
	{
		for (int j = 0; j < 15; j++)
		{
			boardarray[i][j] = GetAlphabet();
		}
	}

	for (int i = 2; i < 10; i++)  //for filling further rows with spaces
	{
		for (int j = 0; j < 15; j++)
		{
			boardarray[i][j] = 27;
		}
	}

	for (int i = 0; i < 1000;i++)  //for the balls for shot gun
	{
		ball[i] = GetAlphabet();
	}



	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register functions to the library,
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000, Timer, 0);
	glutMainLoop();

	return 1;
}
#endif /* */







