#include <gl/freeglut.h>
#include<math.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "image.h"
#define scalefactor 1 //Move view  adjust
/*
* Copyright (C) Lin Shu You, 2022
*
* This program applies 3D visualize.
* Use OpenGL build voxel,Voxel is a hexahedron.
* Last update date: 2023 / 06 / 18
* Renew subject: dynamic memory.
*/

using namespace std;

typedef unsigned char uint_8;
typedef unsigned short uint_16;

struct XYZ_RGB_t {
	uint_8 R, G, B;
	short X = 0, Y = 0;
	int Z = 0;
};

// volume point cloud 
struct Voxel_V {
	uint_8 R, G, B;//顏色
	//uint_16 x, y;
	int X = 0, Y = 0;
	int Z;
};

uint8_t img_H, img_W;
vector<XYZ_RGB_t> volume_t; //store voxel volume
XYZ_RGB_t Stack_voxel;

struct Point {
	int x, y, z;
};
struct Point2i {
	int x, y;
};
struct Point3f {
	float x, y, z;
};

struct Point3d {
	double x, y, z;
};

struct Point3i {
	int x, y, z;
};


int LINE_t = 0;

int old_rot_x = 0;
int old_rot_y = 0;

int rot_x = 0;
int rot_y = 0;

int record_x = 0;
int record_y = 0;

float x_shift = 0;
float y_shift = 0;
float z_shift = 0;

long long getSystemTime() {
	struct timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm;
}

void Display()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 0, 0, 0, -10000, 0, 1, 0);

	glTranslatef(x_shift, y_shift, z_shift);
	glRotatef((float)rot_y + (float)record_y, 1.0, 0.0, 0.0);
	glRotatef((float)rot_x + (float)record_x, 0.0, 1.0, 0.0);

	float R, G, B;//color 
	float scale = scalefactor;
	int x, y, z;//


	for (int n = 1; n < volume_t.size(); n++) {
		glBegin(GL_QUADS);
		R = volume_t[n].R / 255.0, G = volume_t[n].G / 255.0, B = volume_t[n].B / 255.0;

		glColor3f(R, G, B);//normalize RGB Value. Opengl need!!!
		x = volume_t[n].X;
		y = -volume_t[n].Y;
		z = -volume_t[n].Z;

		// Voxel 
		glVertex3f(x + (-0.5 * scale), y + (0.5 * scale), z + (0.5 * scale));  glVertex3f(x + (-0.5 * scale), y + (-0.5 * scale), z + (0.5 * scale));  glVertex3f(x + (0.5 * scale), y + (-0.5 * scale), z + (0.5 * scale));
		glVertex3f(x + (0.5 * scale), y + (0.5 * scale), z + (0.5 * scale));

		glVertex3f(x + (0.5 * scale), y + (0.5 * scale), z + (-0.5 * scale));  glVertex3f(x + (0.5 * scale), y + (-0.5 * scale), z + (-0.5 * scale));  glVertex3f(x + (-0.5 * scale), y + (-0.5 * scale), z + (-0.5 * scale));
		glVertex3f(x + (-0.5 * scale), y + (0.5 * scale), z + (-0.5 * scale));

		glVertex3f(x + (0.5 * scale), y + (0.5 * scale), z + (0.5 * scale)); glVertex3f(x + (0.5 * scale), y + (-0.5 * scale), z + (0.5 * scale)); glVertex3f(x + (0.5 * scale), y + (-0.5 * scale), z + (-0.5 * scale));
		glVertex3f(x + (0.5 * scale), y + (0.5 * scale), z + (-0.5 * scale));

		glVertex3f(x + (-0.5 * scale), y + (0.5 * scale), z + (-0.5 * scale)); glVertex3f(x + (-0.5 * scale), y + (-0.5 * scale), z + (-0.5 * scale)); glVertex3f(x + (-0.5 * scale), y + (-0.5 * scale), z + (0.5 * scale));
		glVertex3f(x + (-0.5 * scale), y + (0.5 * scale), z + (0.5 * scale));

		glVertex3f(x + (-0.5 * scale), y + (0.5 * scale), z + (-0.5 * scale)); glVertex3f(x + (-0.5 * scale), y + (0.5 * scale), z + (0.5 * scale)); glVertex3f(x + (0.5 * scale), y + (0.5 * scale), z + (0.5 * scale));
		glVertex3f(x + (0.5 * scale), y + (0.5 * scale), z + (-0.5 * scale));

		glVertex3f(x + (-0.5 * scale), y + (-0.5 * scale), z + (0.5 * scale)); glVertex3f(x + (-0.5 * scale), y + (-0.5 * scale), z + (-0.5 * scale)); glVertex3f(x + (0.5 * scale), y + (-0.5 * scale), z + (-0.5 * scale));
		glVertex3f(x + (0.5 * scale), y + (-0.5 * scale), z + (0.5 * scale));

		glEnd();
	}
	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	//printf("你所按按鍵的碼是%x\t此時視窗內的滑鼠座標是(%d,%d)\n", key, x, y);
	switch (key)
	{
	case 'w':
		y_shift += 100 * scalefactor;
		break;
	case 's':
		y_shift -= 100 * scalefactor;
		break;
	case 'd':
		x_shift += 100 * scalefactor;
		break;
	case 'a':
		x_shift -= 100 * scalefactor;
		break;
	case 'q':
		z_shift += 100 * scalefactor;
		break;
	case 'e':
		z_shift -= 100 * scalefactor;
		break;
	case 'z':
		break;
	case 27:

		exit(0);
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	if (state) {
		record_x += x - old_rot_x;
		record_y += y - old_rot_y;

		rot_x = 0;
		rot_y = 0;
	}
	else {
		old_rot_x = x;
		old_rot_y = y;
	}
}

void MotionMouse(int x, int y)
{
	rot_x = x - old_rot_x;
	rot_y = y - old_rot_y;
	glutPostRedisplay();
}

void WindowSize(int w, int h)
{
	printf("目前視窗大小為%dX%d\n", w, h);
	glViewport(0, 0, w, h);            //當視窗長寬改變時，畫面也跟著變
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float rate = (float)w / (float)h;
	//glOrtho(-150 * rate, 150 * rate, -200, 200, -1000, 3000);      //正交投影
	gluPerspective(45, rate, 1.0, 70000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void quadrant(Point3f p, const Point2i center, double& px, double& py) // 4 quadrant
{
	if (p.x <= center.x && p.y <= center.y) {
		px = center.x - p.x;
		py = center.y - p.y;
	}
	if (p.x >= center.x && p.y <= center.y) {
		px = -(p.x - center.x);
		py = center.y - p.y;
	}
	if (p.x >= center.x && p.y >= center.y) {
		px = -(p.x - center.x);
		py = -(p.y - center.y);
	}
	if (p.x <= center.x && p.y >= center.y) {
		px = center.x - p.x;
		py = -(p.y - center.y);
	}
}



// //  Coordinate_transformation
void Coordinate_trans(Point3f p, Point2i& pd)
{
	double sb = 3.4 * pow(10, -3);// 像素尺寸:unit: pixel transfer to unit: mm,  by 攝影機的image sensor (UNKNOW)
	double f = 4.0;//focal length

	Point2i center;//image center coordinate				
	double px, py;
	center.x = img_W / 2; center.y = img_H / 2;

	quadrant(p, center, px, py);

	//convert 3d coordinate
	double x = round((px * sb * p.z) / f);// X Y 
	double y = round((py * sb * p.z) / f);

	//回傳
	pd.x = -x;
	pd.y = -y;
}


void main()
{
	Image IMG_color("D:\\im6.png");
	Image Disp_R("D:\\disp6.png");

	double img_disp_1;
	double f = 4.0;
	double sb = 3.4 * pow(10, -3); /**/
	double B, Z, Z_2;
	Point2i pd;//3D
	Point3f img_p;//資料結構: 輸入影像座標與視差值
	int i = 0;

	B = 40;//baseline

	for (int y = 0; y < IMG_color.height; y++) {
		for (int x = 0; x < IMG_color.width; x++) {
			img_disp_1 = Disp_R.get_pixel(x, y, 0);//

			if (img_disp_1 < 1) //  no disparity value
				continue;
		
			Z = ((B * f) / (img_disp_1 * sb));// //3D depth ( Z )

			img_p.x = x; img_p.y = y; img_p.z = Z;
			Coordinate_trans(img_p, pd);//pd is output, 3D (X,Y)

			int X = pd.x;
			int Y = pd.y;
			/*printf("%d %d %.2f \n", X, Y, Z);
			printf("%d  \n", r_temp);*/
			Stack_voxel.X = X;
			Stack_voxel.Y = Y;
			Stack_voxel.Z = Z;
			Stack_voxel.R = IMG_color.get_pixel(x, y, 0);
			Stack_voxel.G = IMG_color.get_pixel(x, y, 1);
			Stack_voxel.B = IMG_color.get_pixel(x, y, 2);

			volume_t.push_back(Stack_voxel);
		}
	}

	printf("Esc退出\n");
	int argc = 0;
	char* argv[] = { 0 };
	glutInit(&argc, argv);//
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//single is 2D

	glutInitWindowPosition(800, 100);
	glutInitWindowSize(1000, 500);//size window
	glutCreateWindow("OpenGL cube");//
	glutReshapeFunc(WindowSize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MotionMouse);

	glutDisplayFunc(Display);//

	glutMainLoop();//

	system("pause");
}



////Read 3D Voxel data
//int main()//
//{
//	float extr[6];
//
//	ifstream infile;//
//	infile.open("D://street_0620_map_MY_street_1.txt");//開啟檔案 
//
//	infile >> LINE_t;
//	Stack_voxel = (XYZ_RGB_t*)malloc(sizeof(XYZ_RGB_t) * LINE_t);
//	//printf("voxel total num: %d\n",);
//
//	for (int i = 1; i < LINE_t; i++) {//資料行數  
//		for (int j = 0; j < 6; j++)//X Y Z R G B
//			infile >> extr[j];//讀取一個值（空格、製表符、換行隔開）就寫入到矩陣中，行列不斷迴圈進行
//
//		Stack_voxel[i].X = extr[0];
//		Stack_voxel[i].Y = extr[1];
//		Stack_voxel[i].Z = extr[2];
//		Stack_voxel[i].R = extr[3];
//		Stack_voxel[i].G = extr[4];
//		Stack_voxel[i].B = extr[5];
//	}
//	infile.close();//
//	printf("Esc退出\n");
//	int argc = 0;
//	char* argv[] = { 0 };
//	glutInit(&argc, argv);//
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);//single is 2D
//
//	glutInitWindowPosition(800, 100);
//	glutInitWindowSize(1000, 500);//size window
//	glutCreateWindow("OpenGL cube");//
//	glutReshapeFunc(WindowSize);
//	glutKeyboardFunc(Keyboard);
//	glutMouseFunc(Mouse);
//	glutMotionFunc(MotionMouse);
//
//	glutDisplayFunc(Display);//
//
//	glutMainLoop();//
//
//	char c;
//	while ((c = getchar()) != 27) {
//		free(Stack_voxel);
//		return 0;
//	}
//
//	system("pause");
//}

