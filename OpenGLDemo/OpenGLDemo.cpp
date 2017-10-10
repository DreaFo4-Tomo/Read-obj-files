#include "OpenGLDemo.h"

// GLUT header
#include <stdlib.h>
#include "OpenGL\glut.h"// OpenGL GLUT Library Header

// Open file dialog
#include "LoadFileDlg.h"

// The GLM code for loading and displying OBJ mesh file
#include "glm.h"

// The trackball code for rotating the model
#include "trackball.h"

// The size of the GLUT window
int window_width  = 800;
int window_height = 600;

// The OBJ model
GLMmodel* pModel = NULL;

// The current modelview matrix
double pModelViewMatrix[16];

// If mouse left button is pressed
bool bLeftBntDown = false;

// Old position of the mouse
int OldX = 0;
int OldY = 0;

int flag = 1;
double upx=0.0,upy=1,upz=0.0;
double eyex=1,eyey=1,eyez=1;
int id,id1,id2;
float RGB[3]={1.0,0,1.0};
double nears=0.01f,fars=200.0f;
void mymenu1(int value1)//Color changing.
{
	glDisable(GL_LIGHTING);
    if(value1 == 1)
    {
        glClearColor(0.0,0.0,0.0,0.0);
        RGB[0]=1.0;
        RGB[1]=0.0;
        RGB[2]=0.0;
    }
    if(value1 == 2)
    {
        glClearColor(0.0,0.0,0.0,0.0);
        RGB[0]=0.0;
        RGB[1]=1.0;
        RGB[2]=0.0;
    }
    if(value1 == 3)
    {
        glClearColor(0.0,0.0,0.0,0.0);
        RGB[0]=0.0;
        RGB[1]=0.0;
        RGB[2]=1.0;
    }
	glutPostRedisplay();
}

void mymenu2(int value1)
{
    if(value1 == 1)
    {
        glFrontFace(GL_CW);
    }
    if(value1 == 2)
    {
       glFrontFace(GL_CCW);
    }
	glutPostRedisplay();
}



void mymenu(int value)//Set the main menu.
{
    if(value == 1)
        exit(0);
	else if(value == 2)
	{
		upx=0.0;
		upy=1;
		upz=0.0;
		eyex=1;
		eyey=1;
		eyez=1;
		nears = 0.01f;
		fars = 200.0f;
	}
	else if(value == 3){
		glEnable( GL_LIGHTING );
	}

}

int main(int argc, char* argv[])
{
	// Initialize the GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Problem 1");
	init();

	id1 = glutCreateMenu(mymenu1);
    glutAddMenuEntry("Change_Red_Color",1);
    glutAddMenuEntry("Change_Green_Color",2);
    glutAddMenuEntry("Change_Blue_Color",3);

    id2 = glutCreateMenu(mymenu2);
    glutAddMenuEntry("CW",1);
    glutAddMenuEntry("CCW",2);

    id = glutCreateMenu(mymenu);
    glutAddSubMenu("Change colors",id1);
	glutAddSubMenu("Change polygon vertices orientation",id2);
	glutAddMenuEntry("Reset the camera",2);
	glutAddMenuEntry("Open the lights",3);
	glutAddMenuEntry("Exit",1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
	// Set the callback function
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

/// Initialize the OpenGL
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, nears, fars);

	glClearColor(0, 0, 0, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

  	//Perform the backface culling to reduce the number of primitives actually drawn.
	glEnable(GL_CULL_FACE);

    // Setup other misc features.
    glEnable( GL_LIGHTING );
    glEnable( GL_NORMALIZE );
    glShadeModel( GL_SMOOTH );

    // Setup lighting model.
	GLfloat light_model_ambient[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_direction[] = {0.0f, 0.0f, 10.0f, 0.0f};
	GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable( GL_LIGHT0 );
	
	// Init the dlg of the open file
	PopFileInitialize( NULL );
}

/// Display the Object
void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated( 0.0, 0.0, -5.0 );
	glMultMatrixd( pModelViewMatrix );

	gluLookAt(eyex,eyey,eyez,0,0,0,upx,upy,upz);//Change the orientation and position of the camera.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, nears, fars);
	glColor3f(RGB[0],RGB[1],RGB[2]);
	if ( pModel )
	{
		glmDraw( pModel, GLM_FLAT ,flag);
	}
	glutSwapBuffers();
}


/// Reshape the Window
void reshape(int w, int h)
{
	// Update the window's width and height
	window_width  = w;
	window_height = h;

	// Reset the viewport
	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)window_width/(float)window_height, nears, fars);

	glutPostRedisplay();
}

/// Keyboard Messenge
void keyboard(unsigned char key, int x, int y)
{
	// The obj file will be loaded
	char FileName[128] = "";
	char TitleName[128]= "";

	// Center of the model
	float modelCenter[] = {0.0f, 0.0f, 0.0f};

	switch ( key )
	{
	case 'o':
	case 'O':
		PopFileOpenDlg( NULL, FileName, TitleName );

		// If there is a obj model has been loaded, destroy it
		if ( pModel )
		{
			glmDelete( pModel );
			pModel = NULL;
		}

		// Load the new obj model
		pModel = glmReadOBJ( FileName );

		// Generate normal for the model
		glmFacetNormals( pModel );

		// Scale the model to fit the screen
		glmUnitize( pModel, modelCenter );

		// Init the modelview matrix as an identity matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );

		break;

	case '+':
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pModelViewMatrix );
		glScaled( 1.05, 1.05, 1.05 );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
		break;

	case '-':
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pModelViewMatrix );
		glScaled( 0.95, 0.95, 0.95 );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );
		break;

	//Render an object using points.
	case 'z':
	case 'Z':
		flag = 2;
		break;

	//Render an object using solid polygon.
	case 'c':
	case 'C':
		flag = 1;
		break;

	 case'w':
            eyex=eyex-0.1;
            break;

     case'W':
            eyex=eyex+0.1;
            break;

     case'a':
            eyey=eyey-0.1;
            break;

     case'A':
            eyey=eyey+0.1;
            break;

     case'd':
            eyez=eyez-0.1;
            break;

     case'D':
            eyez=eyez+0.1;
            break;

     case'q':
            upx=upx-0.5;
            break;

     case'Q':
            upx=upx+0.5;
            break;

     case'e':
            upy=upy-0.5;
            break;

     case'E':
            upy=upy+0.5;
            break;

     case's':
            upz=upz-0.5;
            break;

     case'S':
            upz=upz+0.5;
            break;

	 case'f':
            fars=fars-10;
            break;

     case'F':
            fars=fars+10;
            break;

	 case'n':
            nears=nears-0.1f;
            break;

     case'N':
            nears=nears+0.1f;
            break;
	default:
		break;
	}

	glutPostRedisplay();
}

/// Mouse Messenge
void mouse(int button, int state, int x, int y)
{
	if ( pModel )
	{
		if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
		{
			OldX = x;
			OldY = y;
			bLeftBntDown = true;
		}
		else if ( state==GLUT_DOWN && button==GLUT_LEFT_BUTTON )
		{
			bLeftBntDown = false;
		}
	}
}

/// Motion Function
void motion(int x, int y)
{
	if ( bLeftBntDown && pModel )
	{
		float fOldX =  2.0f*OldX/(float)window_width  - 1.0f;
		float fOldY = -2.0f*OldY/(float)window_height + 1.0f;
		float fNewX =  2.0f*x/(float)window_width  - 1.0f;
		float fNewY = -2.0f*y/(float)window_height + 1.0f;

		double pMatrix[16];
		trackball_opengl_matrix( pMatrix, fOldX, fOldY, fNewX, fNewY);

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glLoadMatrixd( pMatrix );
		glMultMatrixd( pModelViewMatrix );
		glGetDoublev( GL_MODELVIEW_MATRIX, pModelViewMatrix );

		OldX = x;
		OldY = y;
		glutPostRedisplay();
	}
}

/// Idle function
void idle(void)
{
	glutPostRedisplay();
}
