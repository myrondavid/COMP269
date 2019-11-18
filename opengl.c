#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#define min(a, b) ((a) < (b) ? a : b)

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;
int MOUSE_X;
int MOUSE_Y;
char WINDOW_NAME[] = "OpenGL Application";
float dt = 0.0f;
float t = 0.0f;
float ref_t = 0.0f;

enum {
	X,
	Y,
	Z
};


/* GLUT callbacks */
void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name);
void idle(void);
void reshape(int w, int h);
void motion(int w, int h);
void keyboard(unsigned char key, int x, int y);
void display();

void draw_line(float x0, float y0, float x1, float y1);
void draw_rectangle(float x, float y, float w, float h, float angle);
void draw_circle(float x, float y, float radius);

void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name){
	glutInit(argc, argv);
	glutInitDisplayMode(display_mode);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(window_name);

	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);	
}

void idle(void) {
    static float old_t = 0.0f;

    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - ref_t;
    dt = (t - old_t);
    old_t = t;

    glutPostRedisplay();
}

void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	glOrtho(-1.0f, 4.0f, -1.0f, 3.0f, -1.0f, 1.0f);
}

void motion(int x, int y){
}

void keyboard(unsigned char key, int x, int y){
	if(key == 'r'){
		ref_t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glColor3f(0.8f, 0.8f, 0.8f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int i;
	for(i = 0;i < 5;i++){
		draw_line(-1.0f, 1.0f * i, 4.0f, 1.0f * i);
		draw_line(1.0f * i, -1.0f, 1.0f * i, 3.0f);
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	draw_line(0.0f, -10.0f, 0.0f, 10.0f);
	draw_line(-10.0f, 0.0f, 10.0f, 0.0f);


	void draw_triangle(float sx, float sy){
		glPushMatrix();
		glScalef(sx, sy, 1.0f);

		glBegin(GL_TRIANGLES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 2.0f);
			glVertex2f(1.0f, 0.0f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.0f, 2.0f);
			glVertex2f(1.0f, 2.0f);
			glVertex2f(1.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);			
			glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();

		float radius = 0.1f;

		glColor3f(0.0f, 0.0f, 0.0f);
		draw_circle(1.0f + sx * 0.0f, sy * 0.0f, radius);
		draw_circle(1.0f + sx * 1.0f, sy * 2.0f, radius);
		draw_circle(1.0f + sx * 1.0f, sy * 0.0f, radius);
		radius = 0.075f;
		glColor3f(1.0f, 1.0f, 1.0f);
		draw_circle(1.0f + sx * 0.0f, sy * 0.0f, radius);
		draw_circle(1.0f + sx * 1.0f, sy * 2.0f, radius);
		draw_circle(1.0f + sx * 1.0f, sy * 0.0f, radius);
	}

	glTranslatef(1.0f, 0.0f, 0.0f);
	
	glColor3f(0.3f, 0.3f, 0.3f);
 	draw_triangle(1.0f, 1.0f);
	
	
	glColor3f(0.5f, 0.5f, 0.5f);
	draw_triangle(min(1.0f + t, 2.0f), 1.0f);

 	glutSwapBuffers();
}

void draw_circle(float x, float y, float radius){
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	glBegin(GL_POLYGON);
	float i;
	for(i = 0.0f; i < 2 * M_PI; i += M_PI / 20){

		glVertex2f(cos(i) * radius, sin(i) * radius);
	}
	glEnd();

	glPopMatrix();
}

void draw_line(float x0, float y0, float x1, float y1){
	glBegin(GL_LINES);
		glVertex2f(x0, y0);
		glVertex2f(x1, y1);
	glEnd();
}

void draw_rectangle(float x, float y, float w, float h, float angle){
	glPushMatrix();

	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	float vertices[][2] = {
		{-w/2, -h/2},
		{w/2, -h/2},
		{w/2, h/2},
		{-w/2, h/2}
	};

	glBegin(GL_QUADS);
	int i;
	for(i = 0;i < 4;i++){
		glVertex3f(vertices[i][X], vertices[i][Y], 0.0f);
	}
	glEnd();

	glPopMatrix();
}


int main(int argc, char** argv){
	init(&argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, GLUT_DOUBLE | GLUT_RGB, WINDOW_NAME);

	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}
