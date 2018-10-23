//
//  main.cpp
//  OpenGL
//
//  Created by Student on 9/12/18.
//  Copyright (c) 2018 Student. All rights reserved.
//

#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	float x;
	float y;
} point2D_t;

typedef struct {
	float x;
	float y;
	float z;
} point3D_t;

typedef struct {
	float r;
	float g;
	float b;
} color_t;

typedef struct {
	float v[4];
} vector3D_t;

typedef struct {
	float m[4][4];
} matrix3D_t;

typedef struct {
    int numberOfPoints;
    int p[100];
} face_t;

typedef struct {
    int numberOfPoints;
    int numberOfFace;
    point3D_t p[100];
    face_t fc[100];
}object3D_t;

vector3D_t point2vector(point3D_t p) {
	vector3D_t vec;
	vec.v[0] = p.x;
	vec.v[1] = p.y;
	vec.v[2] = p.z;
	vec.v[3] = 1;
	return vec;
}

point3D_t vector2point(vector3D_t vec) {
	point3D_t p;
	p.x = vec.v[0];
	p.y = vec.v[1];
	p.z = vec.v[2];
	return p;
}

point2D_t vector2point2D(vector3D_t vec) {
	point2D_t p;
	p.x = vec.v[0];
	p.y = vec.v[1];
	return p;
}

matrix3D_t createIdentity() {
	matrix3D_t mat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
                mat.m[i][j] = 0;
		mat.m[i][i] = 1;
	}
	return mat;
}

matrix3D_t translationMTX(float dx, float dy, float dz) {
	matrix3D_t mat = createIdentity();
	mat.m[0][2] = dx;
	mat.m[1][2] = dy;
	mat.m[2][2] = dz;
	return mat;
}

matrix3D_t scalingMTX(float sx, float sy, float sz) {
	matrix3D_t mat = createIdentity();
	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;
	return mat;
}

matrix3D_t rotationXMTX(float a) {
	matrix3D_t mat = createIdentity();
	mat.m[1][1] = cos(a);
	mat.m[1][2] = -sin(a);
	mat.m[2][1] = sin(a);
	mat.m[2][2] = cos(a);
	return mat;
}

matrix3D_t rotationYMTX(float a) {
	matrix3D_t mat = createIdentity();
	mat.m[0][2] = cos(a);
	mat.m[0][0] = -sin(a);
	mat.m[2][2] = sin(a);
	mat.m[2][0] = cos(a);
	return mat;
}

matrix3D_t rotationZMTX(float a) {
	matrix3D_t mat = createIdentity();
	mat.m[0][0] = cos(a);
	mat.m[0][1] = -sin(a);
	mat.m[1][0] = sin(a);
	mat.m[1][1] = cos(a);
	return mat;
}

vector3D_t operator * (matrix3D_t mat, vector3D_t vec) {
	vector3D_t vec1;

	for (int i = 0; i < 4; i++)
	{
		vec1.v[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			vec1.v[i] += mat.m[i][j] * vec.v[j];
		}
	}
	return vec1;
}

matrix3D_t operator * (matrix3D_t mat1, matrix3D_t mat2) {
	matrix3D_t mat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				mat.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
			}
		}
	}
	return mat;
}

void setColor(color_t col) {
	glColor3f(col.r, col.g, col.b);
}

void drawPolygon(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_LOOP);
	setColor({ 1,1,1 });

	for (i = 0; i < n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawLimas(){
    static float a = 0;
    object3D_t limas={4,4,
        {{0,0,0},{100,0,0},{0,100,0},{0,0,100}},
        {{3,{0,2,1}},{3,{0,3,2}},{3,{0,1,3}},{3,{1,2,3}}}
        };
    vector3D_t vec[100];
    point2D_t p[100];
    matrix3D_t mat = rotationXMTX(-a)*rotationYMTX(-a)*rotationZMTX(a);
    for(int i=0; i< limas.numberOfPoints; i++){
        vec[i] = point2vector(limas.p[i]);
        vec[i] = mat * vec[i];
    }
    glColor3f(1,1,1);
    for(int i = 0; i < limas.numberOfFace;i++){
        for(int j = 0; j < limas.fc[i].numberOfPoints;j++){
            p[j] = vector2point2D(vec[limas.fc[i].p[j]]);
        }
        drawPolygon(p, limas.fc[i].numberOfPoints);
    }
    a += 0.001;
}

void drawKubus(){
    static float a = 0;
    object3D_t kubus={8,6,
        {{0,0,0},{100,0,0},{100,0,100},{0,0,100},{0,100,0},{100,100,0},{100,100,100},{0,100,100}},
        {{4,{0,1,2,3}},{4,{4,7,6,5}},{4,{2,6,7,3}},{4,{0,4,5,1}},{4,{0,3,7,4}},{4,{2,1,5,6}}}
        };
    vector3D_t vec[100];
    point2D_t p[100];
    matrix3D_t mat = rotationXMTX(a)*rotationYMTX(a)*rotationZMTX(a);
    for(int i=0; i< kubus.numberOfPoints; i++){
        vec[i] = point2vector(kubus.p[i]);
        vec[i] = mat * vec[i];
    }
    glColor3f(1,1,1);
    for(int i = 0; i < kubus.numberOfFace;i++){
        for(int j = 0; j < kubus.fc[i].numberOfPoints;j++){
            p[j] = vector2point2D(vec[kubus.fc[i].p[j]]);
        }
        drawPolygon(p, kubus.fc[i].numberOfPoints);
    }
    a += 0.001;
}

void userdraw(void) {
    drawKubus();
    drawLimas();
}

void display(void) {
	glClear( GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(720, 720);
	glutCreateWindow("Limas");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	gluOrtho2D(-360., 360., -360.0, 360.0);
	glutIdleFunc(display);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
