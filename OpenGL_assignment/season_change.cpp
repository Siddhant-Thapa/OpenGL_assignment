#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.14159265358979323846

#define NUM_SNOW 100
#define NUM_STARS 50

int season = 0;           // 0 = Summer, 1 = Winter
int isNight = 0;          // 0 = Day, 1 = Night
float snowX[NUM_SNOW];
float snowY[NUM_SNOW];
float starX[NUM_STARS];
float starY[NUM_STARS];
float birdX = -5.0f;

void initSnow() {
    for (int i = 0; i < NUM_SNOW; i++) {
        snowX[i] = (rand() % 100 / 10.0f) - 5.0f;     //random co-ordinate
        snowY[i] = (rand() % 100 / 10.0f);
    }
}

void initStars() {
    for (int i = 0; i < NUM_STARS; i++) {
        starX[i] = (rand() % 100 / 10.0f) - 5.0f;
        starY[i] = (rand() % 100 / 10.0f);
    }
}

void drawTrunk() {
    glColor3f(0.36f, 0.25f, 0.20f); // darker brown shade
    glPushMatrix();                 //save current state
    glTranslatef(0, -1.0f, 0);      // trunk base down along y -axis
    glScalef(0.4f, 1.2f, 0.4f);     // long trunk(width ,tall ,depth
    glutSolidCube(1.0f);
    glPopMatrix();                    // restore saved state
}



void drawTreeTop() {
    if (season == 0) {
        glColor3f(0.2f, 0.8f, 0.2f);  // Summer green
    }
    else {
        glColor3f(0.9f, 0.9f, 0.9f);  // Winter white
    }

    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);   // lowered further to remove visible gap
    glutSolidSphere(0.8, 24, 24);    // realistic size
    glPopMatrix();

    // Flowers in Summer
    if (season == 0) {
        glColor3f(1.0f, 0.5f, 0.8f);  // pink flowers
        for (float a = 0; a < 2 * M_PI; a += M_PI / 6) {
            glPushMatrix();
            glRotatef(a * 180 / M_PI, 0, 1, 0);
            glTranslatef(0.6f, 0.6f, 0.0f);  // aligned with new top center
            glutSolidSphere(0.08, 8, 8);
            glPopMatrix();
        }
    }
}


void drawBird() {
    glPushMatrix();
    glTranslatef(birdX, 1.5f, 0.0f);

    // Bird body
    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidSphere(0.1, 12, 12);
    // 2 triangles for wings
    // Left wing
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.1f, 0.0f, 0.0f);
    glVertex3f(-0.3f, 0.1f, 0.0f);
    glVertex3f(-0.3f, -0.1f, 0.0f);
    glEnd();

    // Right wing
    glBegin(GL_TRIANGLES);
    glVertex3f(0.1f, 0.0f, 0.0f);
    glVertex3f(0.3f, 0.1f, 0.0f);
    glVertex3f(0.3f, -0.1f, 0.0f);
    glEnd();

    glPopMatrix();
}

void drawSnow() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_SNOW; i++) {
        glVertex3f(snowX[i], snowY[i], 0.0f);
    }
    glEnd();
}

void drawStars() {
    glDisable(GL_LIGHTING);  // stars are self-lit
    glPointSize(2.0f);     //size to 2 pixels
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; i++) {
        glVertex3f(starX[i], starY[i], -1.0f);  // fixed Z-depth for visibility
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void display() {
    // Set the background color (sky color) based on the season
    if (isNight) {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);  // dark sky for Night
    }
    else if (season == 0) {
        glClearColor(0.53f, 0.81f, 0.98f, 1.0f);  // light blue sky for Summer
    }
    else {
        glClearColor(0.8f, 0.9f, 1.0f, 1.0f);      // pale blue-white sky for Winter
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 2, 5,       // camera placed 2 units up in y axis , 5 unit back on z axis
        0, 0, 0,       // looking at
        0, 1, 0);      // y axis is up

    drawTrunk();
    drawTreeTop();

    if (season == 0 && !isNight) {
        drawBird();
    }
    else {
        drawSnow();
    }

    if (isNight) {
        drawStars();
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 's' || key == 'S') {
        season = 0;
    }
    else if (key == 'w' || key == 'W') {
        season = 1;
    }
    else if (key == 'n' || key == 'N') {
        isNight = 1;
    }
    else if (key == 'd' || key == 'D') {
        isNight = 0;
    }
    glutPostRedisplay(); // re-renders display function
}

void idle() {
    if (season == 1) {
        for (int i = 0; i < NUM_SNOW; i++) {
            snowY[i] -= 0.01f;              // per frame
            if (snowY[i] < -1.0f) snowY[i] = 2.0f;
        }
    }
    else if (!isNight) {
        birdX += 0.02f;                    // per frame
        if (birdX > 5.0f) birdX = -5.0f;
    }
    glutPostRedisplay();
}

void initScene() {
    srand(0);           // fixes my random number to same values
    initSnow();
    initStars();

    glEnable(GL_DEPTH_TEST); //ensure object render properly
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     //first default light
    glEnable(GL_COLOR_MATERIAL); //initialize all glcolor3f() used before

    GLfloat diff[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //diffues light color 80% white light
    GLfloat pos[] = { 2.0f, 2.0f, 2.0f, 1.0f };   //position of light in 3d space
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);   //assigns light pos in world coordinate
}

void reshape(int w, int h) {     //auto called when window resized
    glViewport(0, 0, w, h);    //drawable area in the window (0,0) -bottom, left corner
    glMatrixMode(GL_PROJECTION);  //camera setup 
    glLoadIdentity();            //reset to indetity -blank state
    gluPerspective(60, (float)w / h, 1.0, 100.0); //makes object farther away look small(natural to human eye) 60- field of view, aspect ratio- ootherwise stretched object
    glMatrixMode(GL_MODELVIEW); // actual drawing (rotations and translations)
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Summer and Winter Seasons");

    initScene();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);          // creating window and setting up call back functions
    glutMainLoop();

    return 0;
}
