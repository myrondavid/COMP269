#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#define min(a, b) ((a) < (b) ? a : b)
#define max(a, b) ((a) > (b) ? a : b)

enum {
	X,
	Y,
	Z
};

enum {
	Orthographic,
	Perspective
};

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;
int MOUSE_X;
int MOUSE_Y;
char WINDOW_NAME[] = "OpenGL Application";
float dt = 0.0f;
float t = 0.0f;
float ref_t = 0.0f;
int projectionMode = Perspective;
float cam_position[3] = {0.0f, 0.0f, 0.0f};
float cam_rotation[3];
float fovy = 60.0;

/* GLUT callbacks */
void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name);
void idle(void);
void reshape(int w, int h);
void motion(int w, int h);
void keyboard(unsigned char key, int x, int y);
void display();

void draw_line(float x0, float y0, float z0, float x1, float y1, float z1);
void draw_rectangle(float x, float y, float w, float h, float angle);
void draw_circle(float x, float y, float radius);

void planet(float x, float y, float z, float distance, float size, float rotationSpeed){
	//"Translation"
	glRotatef(rotationSpeed * t, x, y, z);
	glTranslatef(0.0f, 0.0f, distance);

	//Local rotation
	glRotatef(t, 0.0f, 1.0f, 0.0f);
	
	glutSolidSphere(size, 20, 20);
}


void planet1(){
	glColor3f(1.0f, 0.0f, 0.0f);
	planet(0.0f, 1.0f, 0.0f, 100.0f, 15.0f, -10.0f);
}

void planet2(){
	glColor3f(0.0f, 0.0f, 1.0f);
	planet(0.0f, 1.0f, 0.0f, 200.0f, 10.0f, -10.0f);

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	planet(0.0f, 1.0f, 0.0f, 20.0f, 2.0f, 10.0f);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.25f, 0.25f, 0.25f);
	planet(1.0f, 1.0f, 0.0f, 50.0f, 3.0f, -100.0f);
	glPopMatrix();
}

void sun() {
	glColor3f(1.0f, 1.0f, 0.0f);
	planet(0.0f, 1.0f, 0.0f, 0.0f, 30.0f, 10.0f);

	glPushMatrix();
	planet1();
	glPopMatrix();

	glPushMatrix();
	planet2();
	glPopMatrix();
}

void solarSystem(){
	sun();
}





void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name){
	glutInit(argc, argv);
	glutInitDisplayMode(display_mode);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(window_name);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);

	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);	
}

void idle(void) {
    static float old_t = 0.0f;

    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - ref_t;
    dt = (t - old_t);
    old_t = t;

    gluLookAt(cam_position[X], cam_position[Y], cam_position[Z], cam_rotation[X], cam_rotation[Y], cam_rotation[Z], 0.0f, 1.0f, 0.0f);

    glutPostRedisplay();
}

void reshape(int w, int h){
	printf("reshaping\n");
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();

	float aspect = 1.0f * h / w;

	switch(projectionMode){
		case Orthographic:
			glOrtho(-WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f, (WINDOW_WIDTH * aspect) / 2.0f, (-WINDOW_WIDTH * aspect) / 2.0f, -1000.0f, 1000.0f);
			break;
		case Perspective:
			gluPerspective(fovy, aspect, 0.001, 1000.0f);
	}
}

void motion(int x, int y){
}

void keyboard(unsigned char key, int x, int y){
	if(key == 'r'){
		ref_t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	}
	if(key == 'c'){
		projectionMode = !projectionMode;
		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	draw_line(0.0f, 1000.0f, 0.0f, 0.0f, -1000.0f, 0.0f);

	//planet(0.0f, 0.0f, 0.0f, 100.0f, 50.0f);
	solarSystem();
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

void draw_line(float x0, float y0, float z0, float x1, float y1, float z1){
	glBegin(GL_LINES);
		glVertex3f(x0, y0, z0);
		glVertex3f(x1, y1, z1);
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
	init(&argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH, WINDOW_NAME);

	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}

