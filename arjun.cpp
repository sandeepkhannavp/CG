# POLYGON CLIPPING

#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<iostream>
#include<vector>
#include<cmath>
#include <stdlib.h>
using namespace std;

static int slices = 16;
static int stacks = 16;

static int x_length=720;
static int y_length=640;

/* GLUT callback Handlers */
class Point
{
    public:
        double X;
        double Y;

        Point()
        {
            X=0;
            Y=0;
        }
        Point(double x,double y)
        {
            this->X=x;
            this->Y=y;
        }

        void xy(double x,double y)
        {
            this->X=x;
            this->Y=y;
        }
        double x()
        {
            return X;
        }
        double y()
        {
            return Y;
        }
};

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,x_length,0,y_length);
    glClearColor(0,0,0,0);
}

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

void sutherland_hodgeman(Point bl,Point tr,vector<Point> polygon);
int region_code(Point bl,Point tr,Point p,int side);
Point intersection(Point bl,Point tr,Point A,Point B,int side);
void draw_window();
int draw=0;

vector<Point> points;
vector<Point> polygon;
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // clear display window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet( GLUT_WINDOW_WIDTH );
    const double h = glutGet( GLUT_WINDOW_HEIGHT );
    gluOrtho2D(0.0, w, 0.0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3b(1,1,1);
/*
    Point bl(-100,0);
    Point tr(200,200);
    Point A(-50,100);
    Point B(100,100);


    vector<Point> polygon;
    polygon.push_back(Point(0,300));
    polygon.push_back(Point(100,300));
    polygon.push_back(Point(100,100));
    polygon.push_back(Point(0,100));
    polygon.push_back(Point(-50,150));
    polygon.push_back(polygon[0]);
    //polygon.push_back(Point(100,300));
    //polygon.push_back(Point(-200,100));
    //polygon.push_back(Point(50,-100));
    //polygon.push_back(Point(250,100));

    glBegin(GL_LINE_LOOP);
        glColor3f(1,1,1);
        glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);
        glVertex2i(bl.x()+x_length/2,tr.y()+y_length/2);
        glVertex2i(tr.x()+x_length/2,tr.y()+y_length/2);
        glVertex2i(tr.x()+x_length/2,bl.y()+y_length/2);
        glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);

    glEnd();
  */


    if(points.size()>1)
    {
        draw_window();
    }
    glBegin(GL_POINTS);
        glPointSize(3);
        glColor3f(1,0,0);
        for(int i=0;i<polygon.size();i++)
        {
            glVertex2i(polygon[i].x()+x_length/2,polygon[i].y()+y_length/2);
        }
    glEnd();
    if(draw)
    {
        //cout<<"Polygon size : "<<polygon.size()<<endl;
        sutherland_hodgeman(points[0],points[1],polygon);
    }
    glFlush();
}



void sutherland_hodgeman(Point bl,Point tr,vector<Point> polygon)
{
    //draw_window();
    glBegin(GL_LINE_LOOP);
            glColor3f(1,0,0);
            for(int i=0;i<polygon.size();i++)
            {
                glVertex2i(polygon[i].x()+x_length/2,polygon[i].y()+y_length/2);
            }
    glEnd();
    vector<Point> clipped=polygon;
    for(int side=1;side<=4;side++)
    {

        polygon=clipped;
        clipped.clear();
        for(int i=0;i<polygon.size()-1;i++)
        {
            Point A=polygon[i];
            Point B=polygon[i+1];
            int i1=region_code(bl,tr,A,side);
            int i2=region_code(bl,tr,B,side);
            if(i1 && i2)
            {
                clipped.push_back(B);
            }else if(i1 && !i2)
            {
                Point i=intersection(bl,tr,A,B,side);
                clipped.push_back(i);
            }else if(!i1 && i2)
            {
                Point i=intersection(bl,tr,B,A,side);
                clipped.push_back(i);
                clipped.push_back(B);
            }
        }
        clipped.push_back(clipped[0]);
    }
    //cout<<"clipped size : "<<polygon.size()<<endl;
    for(int i=0;i<clipped.size();i++)
    {
        cout<<clipped[i].x()<<" "<<clipped[i].y()<<endl;
    }
    glBegin(GL_LINE_LOOP);
            glColor3f(1,1,1);
            for(int i=0;i<clipped.size();i++)
            {
                glVertex2i(clipped[i].x()+x_length/2,clipped[i].y()+y_length/2);
            }
    glEnd();

}

Point intersection(Point bl,Point tr,Point A,Point B,int side)
{
    double x,y;
    Point i;
        switch(side)
        {
        case 1:
            x=bl.x();
            y=((B.y()-A.y())*(bl.x()-B.x())/(B.x()-A.x()))+B.y();
            i.xy(x,y);
            break;
        case 2:
            x=tr.x();
            y=((B.y()-A.y())*(tr.x()-B.x())/(B.x()-A.x()))+B.y();
            i.xy(x,y);
            break;
        case 3:
            y=bl.y();
            x=(((y-B.y())*(A.x()-B.x()))/(A.y()-B.y()))+B.x();
            i.xy(x,y);
            break;
        case 4:
            y=tr.y();
            x=(((y-B.y())*(A.x()-B.x()))/(A.y()-B.y()))+B.x();
            i.xy(x,y);
            break;
        }
        return i;
}


int region_code(Point bl,Point tr,Point p,int side)
{
    switch(side)
    {
    case 1:
        return bl.x()<=p.x();
        break;
    case 2:
        return tr.x()>=p.x();
        break;
    case 3:
        return bl.y()<=p.y();
        break;
    case 4:
        return tr.x()>=p.y();
        break;
    }
}
static void key(unsigned char key, int x, int y)
{
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
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}


void draw_window()
{
    Point bl=points[0];
    Point tr=points[1];
    glBegin(GL_LINE_LOOP);
        glColor3f(0,1,0);
        glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);
        glVertex2i(bl.x()+x_length/2,tr.y()+y_length/2);
        glVertex2i(tr.x()+x_length/2,tr.y()+y_length/2);
        glVertex2i(tr.x()+x_length/2,bl.y()+y_length/2);
        glVertex2i(bl.x()+x_length/2,bl.y()+y_length/2);

    glEnd();
    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        draw=0;
        Point p(x-x_length/2,(y_length-y)-y_length/2);
        if(points.size()<2)
        {
            points.push_back(p);
            polygon.clear();
        }else
        {
            polygon.push_back(p);
        }

    }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        polygon.push_back(polygon[0]);
        draw=1;
    }
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

    glutCreateWindow("GLUT Shapes");
    glutMouseFunc(mouse);
    glutDisplayFunc(display);

    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}


#flood fill-boundary fill

#include <iostream>
//#include <stdlib.h>
//#include <cmath>
#include <vector>
#include <windows.h>
#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#define HEIGHT 600
#define WIDTH 800

using namespace std;

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glClearColor(0, 0, 0, 1);
}

static void drawPoint(int x, int y)
{
    glPointSize(2.0f);
    glBegin(GL_POINTS);
        glColor3d(1, 1, 1);
        glVertex2f(x, y);
    glEnd();
    glFlush();
}

bool isSameColor(vector<float>& color1, vector<float>& color2) {
    if(color1[0] == color2[0] && color1[1] == color2[1] && color1[2] == color2[2]) return true;
    else return false;
}

void floodFill(int x, int y, vector<float>& oldColor, vector<float>& newColor) {
    float color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    vector<float> currentColor(begin(color), end(color));
    if(isSameColor(currentColor, oldColor))
    {
        glPointSize(2.0f);
        glBegin(GL_POINTS);
            glColor3d(newColor[0], newColor[1], newColor[2]);
            glVertex2d(x, y);
        glEnd();
        glFlush();
        floodFill(x + 2, y, oldColor, newColor);
        floodFill(x, y + 2, oldColor, newColor);
        floodFill(x - 2, y, oldColor, newColor);
        floodFill(x, y - 2, oldColor, newColor);
    }
}

void boundaryFill(int x, int y, vector<float>& fillColor, vector<float>& borderColor) {
    float color[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    vector<float> interiorColor(begin(color), end(color));

    if(!(isSameColor(interiorColor, borderColor)) && !(isSameColor(interiorColor, fillColor)))
    {
        glBegin(GL_POINTS);
            glColor3d(fillColor[0], fillColor[1], fillColor[2]);
            glVertex2d(x, y);
        glEnd();
        glFlush();
        boundaryFill(x, y + 2, fillColor, borderColor);
        boundaryFill(x - 2, y, fillColor, borderColor);
        boundaryFill(x, y - 2, fillColor, borderColor);
        boundaryFill(x + 2, y, fillColor, borderColor);
    }
}

static void keyboardHandler(unsigned char key, int x, int y) {
    vector<float> oldColor = {0.0f, 0.0f, 0.0f};
    vector<float> newColor = {0.0f, 1.0f, 1.0f};
    vector<float> borderColor = {1.0f, 1.0f, 1.0f};
    y = HEIGHT - y;
    switch(key) {
        case 'F': floodFill(x, y, oldColor, newColor);
                  break;
        case 'B': boundaryFill(x, y, newColor, borderColor);
                  break;
    }
}

static void mouseHandler(int button, int state, int x, int y) {
    static int x1, y1, xw1, yw1, xw2, yw2;
    static vector<pair<int, int>> points;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(x1 = x, y1 = HEIGHT - y);
        points.push_back(make_pair(x1, y1));
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glLineWidth(2.0f);
        glBegin(GL_LINE_LOOP);
            for(auto i: points) {
                glVertex2d(i.first, i.second);
            }
        glEnd();
        glFlush();
        points.clear();
    }
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Polygon Filling Algorithms");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(keyboardHandler);
    glutMainLoop();

    return EXIT_SUCCESS;
}
