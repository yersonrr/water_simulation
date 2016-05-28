// Cubica

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include <math.h>

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0
#define pi 3.1415

float ctlpoints[21][21][3]; 
GLfloat t=0, L1=1, L2=1, A1=0, A2=0, S1=0, S2=0, D1X=1, D1Y=1, D2X=0, D2Y=0;
int wave = 1;
GLfloat variableKnots[25] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
							 1.0, 2.0, 1.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0,
							 0.0, 0.0, 0.0, 0.0, 0.0};
GLUnurbsObj *theNurb;

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);

}

void init_surface() {
	// Create matrix
	for (int i = 0; i <21; i++) {
		for (int j = 0; j < 21; j++) {
			// Position in X
			ctlpoints[i][j][0] = i - 10;
			// Position in Y
			ctlpoints[i][j][1] = 0;
			// Position in Z
			ctlpoints[i][j][2] = j - 10;
		}
	}
}

void function_waves(int value){
	float W1 = 2*pi/L1;
	float W2 = 2*pi/L2;
	float O1 = S1 * W1;
	float O2 = S2 * W2;
	
	t += 0.1;
	for (int i = 0; i <21; i++) {
		for (int j = 0; j < 21; j++) {
			ctlpoints[i][j][1] = A1 * sin((D1X*ctlpoints[i][j][0] +  D1Y*ctlpoints[i][j][2])*W1 + t * O1)
					   + A2 * sin((D2X*ctlpoints[i][j][0] +  D2Y*ctlpoints[i][j][2])*W2 + t * O2);
		}
	}

	glutTimerFunc(10,function_waves,1);
    glutPostRedisplay();
}

void init(){

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_AUTO_NORMAL);
   glEnable(GL_NORMALIZE);

   init_surface();

   theNurb = gluNewNurbsRenderer();
   gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 15.0);
   gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

   glutTimerFunc(10,function_waves,1);
   t = 0.0;
}



void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;
  };

  if(wave == 1){
	if (key == 'a') L1 -= 0.1;
    else if (key == 'z') L1 += 0.1;
    else if (key == 's') A1 -= 0.1;
    else if (key == 'x') A1 += 0.1;
    else if (key == 'd') S1 -= 0.1;
    else if (key == 'c') S1 += 0.1;
    else if (key == 'f') D1X -= 0.1;
    else if (key == 'v') D1X += 0.1;
	else if (key == 'g') D1Y -= 0.1;
	else if (key == 'b') D1Y += 0.1;
  } else {
	if (key == '1') wave = 1;
	else if (key == '2') wave = 2;
	else if (key == 'a') L2 -= 0.1;
    else if (key == 'z') L2 += 0.1;
    else if (key == 's') A2 -= 0.1;
    else if (key == 'x') A2 += 0.1;
    else if (key == 'd') S2 -= 0.1;
    else if (key == 'c') S2 += 0.1;
    else if (key == 'f') D2X -= 0.1;
	else if (key == 'v') D2X += 0.1;
	else if (key == 'g') D2Y -= 0.1;
	else if (key == 'b') D2Y += 0.1;
  }
}



void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// Luz y material

	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.9, 1.0 };
	GLfloat mat_specular[] = { 0.8, 0.8, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 60.0 };
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   


	
	// Render Grid 
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glPushMatrix();
	glRotatef(90,1.0,0.0,0.0);
    glColor3f( 0.0, 0.7, 0.7 );
    glBegin( GL_LINES );
    zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
    for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	{
	xLocal = DEF_floorGridScale * loopX;
	glVertex3f( xLocal, -zExtent, 0.0 );
	glVertex3f( xLocal, zExtent,  0.0 );
	}
    xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	{
	zLocal = DEF_floorGridScale * loopZ;
	glVertex3f( -xExtent, zLocal, 0.0 );
	glVertex3f(  xExtent, zLocal, 0.0 );
	}
    glEnd();
	ejesCoordenada();
    glPopMatrix();
	glEnable(GL_LIGHTING);
	// Fin Grid
	


	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	
	GLfloat* variablePuntosControl = &ctlpoints[0][0][0]; 

	glPushMatrix();

	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 
                   25, variableKnots, 25, variableKnots,
                   21 * 3, 3, variablePuntosControl, 
                   4, 4, GL_MAP2_VERTEX_3); 
	/*
		No cambien los numeros de la funcion, solo deben de poner los nombres de las variables correspondiente.
	*/
	gluEndSurface(theNurb);	
	glPopMatrix();
	
	
	/* Muestra los puntos de control */
	
	int i,j;
	glPointSize(5.0);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POINTS);

	for (i = 0; i <21; i++) {
		for (j = 0; j < 21; j++) {
	        glVertex3f(ctlpoints[i][j][0], 	ctlpoints[i][j][1], ctlpoints[i][j][2]);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
		

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glutSwapBuffers();
}


int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Nurbs Proyecto - Ola");

	init ();
	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(Keyboard);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	

	glutMainLoop();
	return 1;

}
