#include <stdio.h>
#include <stdlib.h>
#include "glut.h"

// Muammer Yýlmaz

GLfloat Ball_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat Ball_diffuse[] = { 0.5, 0.5, 1.0, 1.0 };
GLfloat Ball_shininess[] = { 50.0 };
GLfloat Bar_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat Bar_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat Bar_shininess[] = { 50.0 };
GLfloat Bar2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat Bar2_diffuse[] = { 1.0, 0, 0, 1.0 };
GLfloat Bar2_shininess[] = { 50.0 };
GLfloat Lives_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat xx_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat xx_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat xx_shininess[] = { 50.0 };


float HizBallX = 0.0, HizBallY = 0.0;
int width = 1000, height = 600, lives = 5;
float BallX = width/2, BallY = 30, BarX = width/2, BarY = 10, Bar2X = width/2, Bar2Y = 590;

unsigned char header[54]; 
unsigned int dataPos;  
unsigned int imageWidth, imageHeight, imageSize;
unsigned char *data;
static GLuint texName;
int	x_old = 0;
int	y_old = 0;
float rota,rotb=0;
bool zor=false,kolay=false,multi=false;
float lookX=0,lookY=0;
bool basla=false;
int gamemode=1;

GLuint loadBMP_custom(const char * imagepath){ // Görsel Okuma
	FILE * file = fopen(imagepath, "rb");
	if (!file){
		printf("Image could not be opened\n"); 
		return 0;
	}
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file\n");
		return 0;
	}
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file\n");
		return 0;
	}
	dataPos     = *(int*)&(header[0x0A]);
	imageSize   = *(int*)&(header[0x22]);
	imageWidth  = *(int*)&(header[0x12]);
	imageHeight = *(int*)&(header[0x16]);
	if (imageSize==0)    imageSize=imageWidth*imageHeight*3; 
	if (dataPos==0)      dataPos=54;
	data = new unsigned char [imageSize];
	fread(data,1,imageSize,file);
	fclose(file);
}

void init(void){
	GLfloat light0_position[] = { 100, 100, 100 };
	GLfloat light1_position[] = { -100, -100, 100 };
	GLfloat light0_color[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light1_color[] = { 1.0, 1.0, 1.0, 0.0 };

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);

    GLuint image = loadBMP_custom("uzay2.bmp"); // *data içine BMP resmi yükleyen fonk.
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
}

void display(void){
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if(basla==true)  // Oyun Oynama Ekraný
{

    glBindTexture(GL_TEXTURE_2D, texName);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();	
	glBegin(GL_POLYGON);								
		glTexCoord2f(0, 0);		glVertex3f(0, 0, 0);     
		glTexCoord2f(0, 1);		glVertex3f(0, height, 0);       
		glTexCoord2f(1, 1);		glVertex3f(width, height, 0);        
		glTexCoord2f(1, 0);		glVertex3f(width, 0, 0);    
	glEnd();
	glPopMatrix();
    glDisable(GL_TEXTURE_2D);
		

	if (BallX <= 10 || BallX >= width-10) 
		HizBallX *= -1;
		
		
		// Yukarýsý için Yanma
	if (BallY >= height-30 && HizBallY>0) {
		if (abs(BallX-Bar2X) < 60){
			HizBallY *= -1.1;
			HizBallX *=  1.1;
			
				// Topu Barda Çarptýðý Yöne Döndürme
				if(BallX-Bar2X>0 && HizBallX<0){
				HizBallX *=  -1;
				}else
				if(BallX-Bar2X<0 && HizBallX>0){
				HizBallX *=  -1;
				}
		}
		else {
			lives--;
			if (!lives) basla=false; // Yanýnca ana ekran
			BallX = Bar2X;
			
			BallY = height-30;
			HizBallX = HizBallY = 0;
			}
		
	}
		
		// Aþaðýsý için yanma
	if (BallY < 30 && HizBallY<0)
		{
	
			if (abs(BallX-BarX) < 60)
			{
				HizBallY *= -1.1;
				HizBallX *= 1.1;
				
				// Topu Barda Çarptýðý Yöne Döndürme
				if(BallX-BarX>0 && HizBallX<0){
				HizBallX *=  -1;
				}else
				if(BarX-BallX>0 && HizBallX>0){
				HizBallX *=  -1;
				}
				
			}
				
			else {
				lives--;
				if (!lives) basla=false;// Yanýnca ana ekran
				BallX = BarX;
				
				BallY = 30;
				HizBallX = HizBallY = 0;
			}
		}

		
			
	BallX += HizBallX;
	BallY += HizBallY;
	
	// Top
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Ball_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Ball_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Ball_shininess);
		glTranslatef(BallX, BallY, 5);
		glutSolidSphere(10, 10, 8);
	glPopMatrix();
	
	// Bar
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Bar_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Bar_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Bar_shininess);
		glTranslatef(BarX, BarY, 5);
		glScalef(5, 1, 1.5);
		glutSolidCube(20);
	glPopMatrix();
	
	// 2. Bar
	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Bar2_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Bar2_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, Bar2_shininess);
		glTranslatef(Bar2X, Bar2Y, 5);
		glScalef(5, 1, 1.5);
		glutSolidCube(20);
	glPopMatrix();
	
	glPushMatrix();										// Kaplama Bar2'nin rengini alýyordu bu þekilde düzeldi
		glMaterialfv(GL_FRONT, GL_DIFFUSE, xx_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, xx_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, xx_shininess);
		glTranslatef(1, 1, 5);
		glScalef(5, 1, 1.5);
		glutSolidCube(0);
	glPopMatrix();
	
	if(gamemode==3){		// Tek kiþilik Zor modda ise
	Bar2X=BallX;	
	}

	
	}else
	
	if(basla==false){ // Ana Sayfa
		
		
		
		
		char *c;
		
		glRasterPos3f(width/2.5, height/1.1,0);

		for (c="HOSGELDINIZ"; *c != '\0'; c++) 
		{	
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
		}
		
		glRasterPos3f(width/3.5, height/1.6,0);
		
		for (c="Oyunu uc farkli modda oynayabilirsiniz."; *c != '\0'; c++) 
		{	
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
		}
		
		glRasterPos3f(10, height/1.9,0);
		
		for (c="Modu oyun esnasinda su tuslarla degistirebilirsiniz:  M: Iki kisi | N: Kolay Tek kisi | B: Zor Tek kisi "; *c != '\0'; c++) 
		{	
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
		}
		
		glRasterPos3f(width/3, height/2.5,0);
		for (c="Oyuna baslamak icin Tiklayiniz"; *c != '\0'; c++) 
		{	
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
		}
		
		glRasterPos3f(width/3, height/10,0);
		for (c="Hareket tuslari : A-D (2. oyuncu : J-L) "; *c != '\0'; c++) 
		{	
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
		}
		
		glRasterPos3f(width/4, 5,0);
		for (c="Yon tuslari ile Aci degistirebilirsiniz.(Sifirlamak Icin : P)"; *c != '\0'; c++) 
		{	
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);	
		}
		
	}

	glutSwapBuffers(); 
	glFlush();
}

void reshape(int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, w, 0, h, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	width = w;
	height = h;
	Bar2Y = height-10;// Ekraný büyütünce üst bar kaymasýn diye
	if(HizBallX==0&&BallY>height/2){  // Top uygun yerde dursun
		BallY=Bar2Y-20;
		BallX=Bar2X;
	}
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 27:if(basla==0)exit(0);basla=0;break;
		case 'a':if(BarX>50)BarX=BarX-20;break;
		case 'd':if(BarX<width-50)BarX=BarX+20;break;
		case 'j':if(Bar2X>50)Bar2X=Bar2X-10;break;
		case 'l':if(Bar2X<width-50)Bar2X=Bar2X+10;break;
		case 'm':gamemode=1;break;
		case 'n':gamemode=2;break;
		case 'b':gamemode=3;break;
		case 'p':					//Açý sýfýrlama
			glViewport (0, 0, (GLsizei) width, (GLsizei) height);
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();
			glOrtho (0, width, 0, height, -700, 700);
			gluLookAt(0, 0, 2, 0,0,0 , 0, 1, 0);
			glMatrixMode(GL_MODELVIEW);
		break;
	}
		if (HizBallX == 0 && HizBallY == 0 &&BallY<200)
		BallX = BarX;
		if (HizBallX == 0 && HizBallY == 0 &&BallY>200)
		BallX = Bar2X;
}

void mouse(int button, int state, int x, int y){
	
	if(basla==true){ 
	
	
		if (button == 0 && state == 0 && HizBallX == 0 && HizBallY == 0 && BallY<height/2){	// Alt bar baþlýyorsa
			HizBallX = 0.05;
			HizBallY = 0.1;
		}
			if (button == 0 && state == 0 && HizBallX == 0 && HizBallY == 0 && BallY>height/2){	// Üst Bar Baþlýyorsa
			HizBallX = 0.05;
			HizBallY = -0.1;
		}
	
		if(button==1){

		}
	}else if(basla==false){ // Ana ekrandan oyun ekranýna geçilir
		basla=true;
	}
	glutPostRedisplay();
}

void passive_motion(int x, int y){
//	BarX = x;
//	if (HizBallX == 0 && HizBallY == 0)
//		BallX = BarX;
}

void timer(int value){
//	HizBallX *= 1.0; // Top her saniye %10 hýzlanýr
//	HizBallY *= 1.0;
	if(gamemode==2){   // Tek kiþilik kolay modda ise
		if(BallX>Bar2X && Bar2X<=width-50){
			Bar2X=Bar2X+40;
		}else if(BallX<Bar2X && Bar2X>=50){
			Bar2X=Bar2X-40;
		}
	}
	glutTimerFunc(1000, timer, 0);
}

void ozel_klavye(int tus, int a, int b){
			
	
		switch(tus){
			case GLUT_KEY_LEFT:
				if(lookX>-1.5)      // açý 1,5 ile -1,5 arasý deðiþebilir ayarlandý
				lookX=lookX-0.1;
				break;
			case GLUT_KEY_RIGHT:
				if(lookX<1.5)
				lookX=lookX+0.1;
				break;
			case GLUT_KEY_UP:
				if(lookY<1.5)
				lookY=lookY+0.1;
				break;
			case GLUT_KEY_DOWN:
				if(lookY>-1.5)
				lookY=lookY-0.1;
				break;
	}
	
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, width, 0, height, -700, 700);
	gluLookAt(lookX, lookY, 2, 0,-lookY, lookY, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}



void fareSurukleme(int x, int y){
	
	x_old = x;
	y_old = y;					// fare ile hareket ettirme eklenebilir
	glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1000, 600); 
	glutInitWindowPosition (100, 0);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(ozel_klavye);
	glutMotionFunc(fareSurukleme);
	glutPassiveMotionFunc(passive_motion);
	glutTimerFunc(1000, timer, 0);
	glutIdleFunc(display);
	glutMainLoop();
}
