#include <windows.h>
#include<cmath>
#ifdef __APPLE__
#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif
#include<iostream>
#include<vector>

#include <stdlib.h>
using namespace std;
static int slices = 16;
static int stacks = 16;

static int x_length=720;
static int y_length=640;

/* GLUT callback Handlers */
void drawHouse();

class Point
{
    public:
        int X;
        int Y;

        Point()
        {
            X=0;
            Y=0;
        }
        Point(int x,int y)
        {
            this->X=x;
            this->Y=y;
        }

        void xy(int x,int y)
        {
            this->X=x;
            this->Y=y;
        }
        int x()
        {
            return X;
        }
        int y()
        {
            return Y;
        }
};

void drawPolygon(vector<Point> p);
void translate(int xi,int yi);
void rotate(double degree);
void scale(int x,int y);


static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,x_length,0,y_length);
    glClearColor(0,0,0,0);
}

//
//Point p1(100,100);
//Point p2(200,200);

vector<Point> point;
vector<vector<Point> > polygon;
int count=0;

void display()
{
//    glClearColor(0, 0, 0.502, 1);
    glClear(GL_COLOR_BUFFER_BIT); // clear display window

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet( GLUT_WINDOW_WIDTH );
    const double h = glutGet( GLUT_WINDOW_HEIGHT );
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(0, 0.502, 1);
    glPointSize(1.5);
    glBegin(GL_LINES);


        glVertex2i(0,y_length/2);
        glVertex2i(x_length,y_length/2);

        glVertex2i(x_length/2,0);
        glVertex2i(x_length/2,y_length);

    glEnd();
    for(int i=0;i<polygon.size();i++)
    {
        drawPolygon(polygon[i]);
        cout<<"Tree\n";
    }
    //rotate(M_PI_2);
    glFlush();


}

static void key(unsigned char key, int x, int y)
{
    int x1,y1;
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
        case 't' :
            cout<<"Enter translation values for x and y: ";
            cin>>x1>>y1;
            translate(x1,y1);
            cout<<"key t is pressed \n"<<x;
            break;
        case 'r':
            cout<<"Enter degree for rotation : ";
            double d;
            cin>>d;
            Point p=point[point.size()-1];
            x1=p.x();
            y1=p.y();
            translate(-x1,-y1);
            rotate(d);
            translate(x1,y1);
            break;

    }

    glutPostRedisplay();
}

void scale(int x,int y)
{
    for(int i=0;i<polygon.size();i++)
    {
        vector<Point> p1=polygon[i];
        for(int i=0;i<p1.size();i++)
        {
            p1[i].xy(p1[i].x()*x,p1[i].y()*y);
        }
        polygon[i]=p1;
    }

}

void rotate(double degree)
{
    double sint=sin(degree*3.14/180);
    double cost=cos(degree*3.14/180);
    cout<<cost<<" "<<sint<<endl;

    for(int i=0;i<polygon.size();i++)
    {
        vector<Point> p1=polygon[i];

        for(int i=0;i<p1.size();i++)
        {
            p1[i].xy(cost*p1[i].x()-sint*p1[i].y(),sint*p1[i].x()+cost*p1[i].y());
        }
        polygon[i]=p1;

    }


}



void translate(int xi,int yi)
{
    for(int i=0;i<polygon.size();i++)
    {
        vector<Point> p1=polygon[i];
        for(int i=0;i<p1.size();i++)
        {
            p1[i].xy(p1[i].x()+xi,p1[i].y()+yi);
        }
        polygon[i]=p1;
    }


    //glutPostRedisplay();
}
void keyboard(int key,int x,int y)
{
    switch(key)
    {

        case GLUT_KEY_LEFT:
            cout<<"LEFT\n";
            translate(-20,0);
            break;
        case GLUT_KEY_RIGHT:
            translate(20,0);
            break;
        case GLUT_KEY_DOWN:
            translate(0,-20);
            break;
        case GLUT_KEY_UP:
            translate(0,20);
            break;

    }
    glutPostRedisplay();

}
void mouse(int button, int state, int x, int y)
{
    //static vector<Point> point;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //digital_differential_analyser(Point(5,6),Point(600,400));
        Point p(x-x_length/2,(y_length-y)-y_length/2);
        point.push_back(p);
        cout<<p.x()<<" "<<p.y()<<"\n";
        count++;
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        vector<Point> p;
        for(int i=0;i<point.size();i++)
        {
            p.push_back(point[i]);
        }
        polygon.push_back(p);
        point.clear();
    }
    glutPostRedisplay();
    glFlush();
}

void drawPolygon(vector<Point> p)
{
    glColor3f(0.502, 0, 0);
    glBegin(GL_POLYGON);
        for(int i=0;i<p.size();i++)
        {
            cout<<p[i].x()<<","<<p[i].y()<<endl;
            glVertex2i(p[i].x()+x_length/2,p[i].y()+y_length/2);
        }
    glEnd();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(x_length,y_length);
    glutInitWindowPosition(250,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("<WINDOW_TITLE>");
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(keyboard);
    glutDisplayFunc(display);

    init();


    glutMainLoop();

    return EXIT_SUCCESS;
}
