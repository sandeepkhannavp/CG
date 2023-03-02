-bash-4.3$ clear
-bash-4.3$ cat circle.cpp

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

void digital_differential_analyser(Point p1,Point p2);
void bresenham_algorithm(Point p1,Point p2);
void general_bresenham_algorithm(Point p1,Point p2);
void circle_drawing_algorithm(int,int,int);
void plot_octants(int x,int y);


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
    glColor3f(1, 1, 1);
    glPointSize(1.5);
//    glBegin(GL_LINES);
//
//
//        glVertex2i(0,y_length/2);
//        glVertex2i(x_length,y_length/2);
//
//        glVertex2i(x_length/2,0);
//        glVertex2i(x_length/2,y_length);
//
//    glEnd();

    for(int i=0;i<count-1;i+=2)
    {

        Point c=point[i];
        Point r=point[i+1];
        double radius=round(sqrt(pow((r.y()-c.y()),2)+pow((r.x()-c.x()),2)));
        circle_drawing_algorithm(c.x(),c.y(),int(radius));
    }
    //circle_drawing_algorithm(100,-50,100);
    glFlush();


}

void circle_drawing_algorithm(int xc,int yc,int r)
{
    int x,y,p;
    x=0;
    y=r;
    p=1-r;
    cout<<x<<","<<y<<endl;

    glBegin(GL_POINTS);

        glPointSize(5);
        glVertex2i(xc+x_length/2,yc+y_length/2);
        glColor3f(0, 0.5, 0.5);
        //plot_octants(x+x_length/2,y+y_length/2);
        glVertex2i(xc+x+x_length/2,yc+y+y_length/2);
        glVertex2i(xc+y+x_length/2,yc+x+y_length/2);
        glVertex2i(xc-x+x_length/2,yc+y+y_length/2);
        glVertex2i(xc-y+x_length/2,yc+x+y_length/2);
        glVertex2i(xc-y+x_length/2,yc-x+y_length/2);
        glVertex2i(xc-x+x_length/2,yc-y+y_length/2);
        glVertex2i(xc+x+x_length/2,yc-y+y_length/2);
        glVertex2i(xc+y+x_length/2,yc-x+y_length/2);

        while(x<=y)
        {
            if(p<0)
            {
                x+=1;
                p+=2*x+1;

            }else
            {
                x+=1;
                y-=1;
                p+=2*(x-y)+1;
            }
            cout<<x<<","<<y<<endl;
            glVertex2i(xc+x+x_length/2,yc+y+y_length/2);
            glVertex2i(xc+y+x_length/2,yc+x+y_length/2);
            glVertex2i(xc-x+x_length/2,yc+y+y_length/2);
            glVertex2i(xc-y+x_length/2,yc+x+y_length/2);
            glVertex2i(xc-y+x_length/2,yc-x+y_length/2);
            glVertex2i(xc-x+x_length/2,yc-y+y_length/2);
            glVertex2i(xc+x+x_length/2,yc-y+y_length/2);
            glVertex2i(xc+y+x_length/2,yc-x+y_length/2);

        }
    glEnd();


}
void plot_octants(int x,int y)
{
    glBegin(GL_POINTS);
        glColor3f(0.502, 0, 0);
        glVertex2i(x+x_length/2,y+y_length/2);
        glVertex2i(y+x_length/2,x+y_length/2);
        glVertex2i(-x+x_length/2,y+y_length/2);
        glVertex2i(-y+x_length/2,x+y_length/2);
        glVertex2i(-y+x_length/2,-x+y_length/2);
        glVertex2i(-x+x_length/2,-y+y_length/2);
        glVertex2i(x+x_length/2,-y+y_length/2);
        glVertex2i(y+x_length/2,-x+y_length/2);
    glEnd();
}

void digital_differential_analyser(Point p1,Point p2)
{
    int dy,dx,steps,x1,y1;
    double x,y;
    x=p1.x();
    y=p1.y();
    double xincr,yincr;
    dy=p2.y()-p1.y();
    dx=p2.x()-p1.x();
    steps=(dy>dx)?dy:dx;
    xincr=(double)dx/steps;
    yincr=(double)dy/steps;
    cout<<"Steps : "<<steps<<"\ndy : "<<dy<<"\ndx : "<<dx<<"\nxincr : "<<xincr<<"\nyincr : "<<yincr<<endl;

    glBegin(GL_POINTS);
        glPointSize(8);
        glColor3f(0.502, 0, 0);
        glVertex2i(x,y);
        cout<<x<<" "<<y<<"\n";
        for(int i=0;i<steps;i++)
        {
            x=(x+xincr);
            y=(y+yincr);
            x1=round((double)x);
            y1=round((double)y);
            glVertex2i(x1,y1);
            cout<<x1<<" "<<y1<<"\n";
        }
    glEnd();
}

void general_bresenham_algorithm(Point p1,Point p2)
{

    int dy=abs(p2.y()-p1.y());
    int dx=abs(p2.x()-p1.x());

    int s1=(p2.x()==p1.x())?0:(p2.x()-p1.x())/dx;
    int s2=(p2.y()==p1.y())?0:(p2.y()-p1.y())/dy;

    int interchange=0;
    if(dy>dx)
    {
        interchange=1;
        int t=dx;
        dx=dy;
        dy=t;
    }
    int a=2*dy;
    int b=2*dy-2*dx;
    int p=2*dy-dx;
    int x=p1.x();
    int y=p1.y();

    glBegin(GL_POINTS);
        glPointSize(5);
        glColor3f(0.502, 0, 0);
        glVertex2i(x,y);
        while(x!=p2.x() && y!=p2.y())
        {
            if(p<0)
            {
                p+=a;
                if(interchange)
                {
                    y=y+s2;
                }else
                {
                    x=x+s1;
                }

            }else
            {
                p+=b;
                y=y+s2;
                x=x+s1;
            }
            glVertex2i(x,y);
            cout<<x<<" "<<y<<endl;
        }
    glEnd();
}

void bresenham_algorithm(Point p1,Point p2)
{
    int dy=p2.y()-p1.y();
    int dx=p2.x()-p1.x();
    int x,y,x2,y2;
    x2=p2.x();
    y2=p2.y();
    x=p1.x();
    y=p1.y();
    double m=(double)dy/dx;
    int p=2*dy-dx;
    int d1=2*dy-2*dx;
    int d2=2*dy;

    glBegin(GL_POINTS);
    glPointSize(5);
        glColor3f(0.502, 0, 0);
        glVertex2i(x,y);
        while(x!=x2 && y!=y2)
        {
            x++;
            if(p>0)
            {
                y++;
                p=p+d1;

            }else
            {
                p=p+d2;
            }
            cout<<"\nx : "<<x<<"\ny : "<<y<<"\np : "<<p<<endl;

            glVertex2i(x,y);

        }

    glEnd();

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

void mouse(int button, int state, int x, int y)
{
    //static vector<Point> point;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //digital_differential_analyser(Point(5,6),Point(600,400));
        Point p(x-x_length/2,(y_length-y)-y_length/2);
        point.push_back(p);
        //cout<<p.x()<<" "<<p.y()<<"\n";
        count++;
    }
    glutPostRedisplay();
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
    glutDisplayFunc(display);

    init();


    glutMainLoop();

    return EXIT_SUCCESS;
}
-bash-4.3$
