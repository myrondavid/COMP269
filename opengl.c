#include <GL/glut.h>

int WINDOW_WIDTH = 640;
int WINDOW_HEIGHT = 480;
int MOUSE_X;
int MOUSE_Y;
char WINDOW_NAME[] = "OpenGL Application";
float dt = 0.0f;
float t = 0.0f;

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


void draw_rectangle(float x, float y, float w, float h, float angle);

void init(int* argc, char** argv, int window_width, int window_height, int display_mode, const char* window_name){
	glutInit(argc, argv);
	glutInitDisplayMode(display_mode);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow(window_name);

	//reshape(WINDOW_WIDTH, WINDOW_HEIGHT);	
}

void idle(void) {
    static float old_t = 0.0f;

    t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    dt = (t - old_t);
    old_t = t;

    glutPostRedisplay();
}

void reshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	
	glLoadIdentity();
	glOrtho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
}

void motion(int x, int y){
	MOUSE_X = x * WINDOW_WIDTH / WINDOW_HEIGHT;
	MOUSE_Y = y * WINDOW_WIDTH / WINDOW_HEIGHT;
}

void keyboard(unsigned char key, int x, int y){

}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);

	draw_rectangle(MOUSE_X, MOUSE_Y, 200.0f, 200.0f, 180 * t);
 	
 	glutSwapBuffers();
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