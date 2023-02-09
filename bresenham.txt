#include <windows.h>
#include<cmath>
#include <GL/glut.h>
#include<iostream>
#include<vector>
#include <stdlib.h>
using namespace std;

static int x_length=720;
static int y_length=640;

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

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,x_length,0,y_length);
    glClearColor(0,0,0,0);
}


vector<Point> point;
int count=0;




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
        glPointSize(3.5);
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
        glPointSize(0.5);
        glColor3f(0, 0.5, 0.5);
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
    glPointSize(3.5);
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


void display()
{

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const double w = glutGet( GLUT_WINDOW_WIDTH );
    const double h = glutGet( GLUT_WINDOW_HEIGHT );
    gluOrtho2D(0.0, w, 0.0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for(int i=0;i<count-1;i+=2)
    {
        //digital_differential_analyser(point[i],point[i+1]);
        //bresenham_algorithm(point[i],point[i+1]);
        general_bresenham_algorithm(point[i],point[i+1]);
    }

    glColor3f(0.502,0,0);
    glPointSize(3);
    cout<<"size : "<<point.size()<<"\n";

    glFlush();

}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        Point p(x,(y_length-y));
        point.push_back(p);
        count++;
    }
    glutPostRedisplay();
}

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
