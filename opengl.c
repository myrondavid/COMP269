#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

#define min(a, b) ((a) < (b) ? a : b)
#define max(a, b) ((a) > (b) ? a : b)
#define radians(a) (a * M_PI / 180.0f)

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
float cam_position[3] = {0.0f, 1.73f, 5.0f};
float cam_rotation[3] = {0.0f, 0.0f, 0.0f};
float fovy = 60.0f;
int walk[3];
int rotate[3];
float move_speed = 10.0f;
float rotation_speed = 180.0f;
float sensibility[2] = {10.0f, 10.0f};

/* GLUT callbacks */
void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name);
void idle(void);
void reshape(int w, int h);
void motion(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);
void display();

void draw_line(float x0, float y0, float z0, float x1, float y1, float z1);
void draw_rectangle(float x, float y, float w, float h, float angle);
void draw_circle(float x, float y, float radius);
void draw_floor(float grid_size, int n);
void draw_axis();

void planet(float x, float y, float z, float distance, float size, float rotationSpeed){
	//"Translation"
	glRotatef(rotationSpeed * t, x, y, z);
	
	glTranslatef(0.0f, 0.0f, distance);
	//Local rotation
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	
	glutSolidSphere(size, 200, 200);
}


void planet1(){
	glColor3f(1.0f, 0.0f, 0.0f);
	planet(0.0f, 1.0f, 0.0f, 100.0f, 15.0f, 1.0f);
}

void planet2(){
	glColor3f(0.0f, 0.0f, 1.0f);
	planet(0.0f, 1.0f, 0.0f, 200.0f, 10.0f, 1.0f);

	glPushMatrix();
	glColor3f(0.5f, 0.5f, 0.5f);
	planet(0.0f, 1.0f, 0.0f, 20.0f, 2.0f, 1.0f);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.25f, 0.25f, 0.25f);
	planet(1.0f, 0.0f, 0.0f, 50.0f, 3.0f, -1.0f);
	glPopMatrix();
}

void sun() {
	glColor3f(1.0f, 1.0f, 0.0f);
	planet(0.0f, 1.0f, 0.0f, 0.0f, 30.0f, 1.0f);

	//RedPlanet
	glPushMatrix();
	planet1();
	glPopMatrix();

	//BluPlanet
	glPushMatrix();
	planet2();
	glPopMatrix();
}

void solarSystem(){
	sun();
}

void house(){
	GLfloat colors[][3] = {
		{1.0, 1.0, 1.0},
		{1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{0.0, 0.0, 1.0},
		{1.0, 1.0, 0.0},
		{1.0, 0.0, 1.0},
		{0.0, 1.0, 1.0},
		{0.5, 0.1, 0.1},
		{0.5, 0.1, 0.1},
		{0.5, 0.1, 0.1},
		{0.5, 0.1, 0.1},
		{0.5, 0.1, 0.1},
		{0.5, 0.1, 0.1},
		{0.5, 0.1, 0.1},
		{0.4, 0.2, 0.1},
		{0.4, 0.1, 0.2},
		{0.5, 0.1, 0.1}
	};
	GLfloat vertices[][3] = {
		{0.0f, 0.0f, 3.0f},
		{3.0f, 0.0f, 3.0f},
		{3.0f, 2.0f, 3.0f},
		{2.0f, 3.0f, 3.0f},
		{0.0f, 3.0f, 3.0f},
		{0.0f, 3.0f, 0.0f},
		{3.0f, 3.0f, 0.0f},
		{3.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{3.0f, 3.0f, 2.0f},
		{1.5f, 5.0f, 1.5f}
	};
	int faces[][5] = {
		{0, 1, 2, 3, 4},
		{1, 7, 6, 9, 2},
		{7, 8, 5, 6},
		{8, 0, 4, 5},
		{0, 1, 7, 8},
		{2, 9, 3},
		{4, 3, 9, 6, 5},
		{10, 5, 4},
		{10, 4, 3},
		{10, 3, 9},
		{10, 9, 6},
		{10, 6, 5}
	};
	int count[] = {5, 5, 4, 4, 4, 3, 5, 3, 3, 3, 3, 3};
	int n_faces = sizeof(faces)/ (5 * sizeof(int));

	int i, j;
	for(i = 0;i < n_faces;i++){
		glColor3f(colors[i][X], colors[i][Y], colors[i][Z]);
		glBegin(GL_POLYGON);
		for(j = 0;j < count[i];j++){
			glVertex3f(vertices[faces[i][j]][X], vertices[faces[i][j]][Y], vertices[faces[i][j]][Z]);
		}
		glEnd();
	}
}

void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name){
	glutInit(argc, argv);
	glutInitDisplayMode(display_mode);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(window_name);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);

	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);	
}

void idle(void) {
    static float old_t = 0.0f;

    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - ref_t;
    dt = (t - old_t);
    old_t = t;

    
    cam_position[Z] -= move_speed * (walk[Z] * sin(radians(cam_rotation[Y]) + M_PI_2) + walk[X] * sin(radians(cam_rotation[Y]))) * dt;
    cam_position[X] += move_speed * (walk[Z] * cos(radians(cam_rotation[Y]) + M_PI_2) + walk[X] * cos(radians(cam_rotation[Y]))) * dt;

	
    //gluLookAt(cam_position[X], cam_position[Y], cam_position[Z], cam_rotation[X], cam_rotation[Y], cam_rotation[Z], 0.0f, 1.0f, 0.0f);

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
			gluPerspective(fovy, 1.0/aspect, 0.001, 1000.0f);
	}
}

void motion(int x, int y){
	//cam_rotation[X] += dt;
	static int last[2];
	cam_rotation[Y] -= (x - last[X]) * sensibility[X] * dt;
	cam_rotation[X] -= (y - last[Y]) * sensibility[Y] * dt;

	last[X] = x;
	last[Y] = y;
}

void keyboard(unsigned char key, int x, int y){
	if(key == 'r'){
		ref_t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	}
	else if(key == 'c'){
	    	projectionMode = !projectionMode;
	    	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
	else if (key == 'w'){
 	    	walk[Z] = 1;	
	}
	else if (key == 's'){
		walk[Z] = -1;	
	}
	else if (key == 'd'){
 	    	walk[X] = 1;	
	}
	else if (key == 'a'){
		walk[X] = -1;	
	}
}

void keyboard_up(unsigned char key, int x, int y){
	if(key == 'w'){
	    walk[Z] = 0;
	}
	else if(key == 's'){
	    walk[Z] = 0;
	}
	else if(key == 'd'){
	    walk[X] = 0;
	}
	else if(key == 'a'){
	    walk[X] = 0;
	}
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(-cam_rotation[X], 1.0, 0.0, 0.0);
	glRotatef(-cam_rotation[Y], 0.0, 1.0, 0.0);

	glTranslatef(-cam_position[X], -cam_position[Y], -cam_position[Z]);

	draw_floor(1.0f, 50);
	draw_axis();
	
	sun();

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

void draw_floor(float grid_size, int n){
	float width = 0.75f;
	glColor3f(width, width, width);
	glLineWidth(width);
	int i, j; 
	for(i = 1;i < n;i++){
		draw_line(i * grid_size, 0.0f, n * grid_size, i * grid_size, 0.0f, -n * grid_size);
		draw_line(-i * grid_size, 0.0f, n * grid_size, -i * grid_size, 0.0f, -n * grid_size);
		draw_line(n * grid_size, 0.0f, i * grid_size, -n * grid_size, 0.0f, i * grid_size);
		draw_line(n * grid_size, 0.0f, -i * grid_size, -n * grid_size, 0.0f, -i * grid_size);
	}
}

void draw_axis(){
	float width = 1.5f;
	glLineWidth(width);
	glColor3f(1.0f, 0.0f, 0.0f);
	draw_line(0.0f, 0.0f, 0.0f, 100, 0.0f, 0.0f);	
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_line(0.0f, 0.0f, 0.0f, 0.0f, 100, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	draw_line(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 100);
}

int main(int argc, char** argv){
	init(&argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT, GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH, WINDOW_NAME);

	glutIdleFunc(idle); 
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}
