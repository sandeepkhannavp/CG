-bash-4.3$ clear
-bash-4.3$ cat liam_bresenham.cpp
#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <stdlib.h>
using namespace std;

#define WIDTH 640
#define HEIGHT 480

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;
float xmax, ymax, xmin, ymin;

class Point
{
    public:
        float x, y;
        Point() {
            x = 0;
            y = 0;
        }
        Point(float x, float y) {
            this->x = x;
            this->y = y;
        }
        void change(float x, float y) {
            this->x = x;
            this->y = y;
        }
        float X() {
            return x;
        }
        float Y() {
            return y;
        }
        void print() {
            cout << "(" << x << ", " << y << ")";
        }
};

vector<Point> points;
int c;

void dda(Point p1, Point p2) {
    float dx, dy, steps, xin, yin, x, y;
    dx = p2.X() - p1.X();
    dy = p2.Y() - p1.Y();
    steps = abs(dx)>abs(dy)?abs(dx):abs(dy);

    xin = dx/steps;
    yin = dy/steps;
    x = p1.X();
    y = p1.Y();
    glBegin(GL_POINTS);
        glColor3f(0, 0, 1);
        glVertex2f(x, y);
    glEnd();
    for (int i=1; i<steps; i++) {
        x += xin;
        y += yin;
        glBegin(GL_POINTS);
            glColor3f(0, 0, 1);
            glVertex2f(x, y);
        glEnd();

    }
    return;
}

void generalBresenham(Point p1,Point p2)
{

    int dy = abs(p2.Y()-p1.Y());
    int dx = abs(p2.X()-p1.X());

    int s1=(p2.X()==p1.X())?0:(p2.X()-p1.X())/dx;
    int s2=(p2.Y()==p1.Y())?0:(p2.Y()-p1.Y())/dy;

    int interchange = 0;
    if(dy>dx)
    {
        interchange = 1;
        int t=dx;
        dx = dy;
        dy = t;
    }
    int a = 2*dy;
    int b = 2*dy - 2*dx;
    int p = 2*dy - dx;
    int x = p1.X();
    int y = p1.Y();

    glBegin(GL_POINTS);
        glPointSize(1);
        glColor3f(0, 0, 1);
        glVertex2i(x,y);
        while(x!=p2.X() && y!=p2.Y())
        {
            if(p<0)
            {
                p += a;
                if(interchange)
                {
                    y = y + s2;
                }else
                {
                    x = x + s1;
                }

            }else
            {
                p += b;
                y = y + s2;
                x = x + s1;
            }
            glVertex2i(x,y);
        }
    glEnd();
}

int computeCode(Point p1) {
    int code = INSIDE;
    if (p1.X()<xmin)
        code |= LEFT;
    if (p1.X()>xmax)
        code |= RIGHT;
    if (p1.Y()<ymin)
        code |= BOTTOM;
    if (p1.Y()>ymax)
        code |= TOP;
    return code;
}

void cohenSutherland(Point p1, Point p2) {
    int code1 = computeCode(p1);
    int code2 = computeCode(p2);
    bool accept = false;

    while(true) {
        if (code1==0&&code2==0) {
            accept = true;
            break;
        }
        else if (code1 & code2)
            break;
        else {
            int codeOut;
            float x, y;

            if (code1!=0)
                codeOut = code1;
            else
                codeOut = code2;

            if (codeOut&TOP) {
                x = p1.X() + (p2.X()-p1.X())*(ymax-p1.Y())/(p2.Y()-p1.Y());
                y = ymax;
            }
            if (codeOut&BOTTOM) {
                x = p1.X() + (p2.X()-p1.X())*(ymin-p1.Y())/(p2.Y()-p1.Y());
                y = ymin;
            }
            if (codeOut&LEFT) {
                y = p1.Y() + (p2.Y()-p1.Y())*(xmin-p1.X())/(p2.X()-p1.X());
                x = xmin;
            }
            if (codeOut&RIGHT) {
                y = p1.Y() + (p2.Y()-p1.Y())*(xmax-p1.X())/(p2.X()-p1.X());
                x = xmax;
            }

            if (codeOut==code1) {
                p1.x = x;
                p1.y = y;
                code1 = computeCode(p1);
            }
            else {
                p2.x = x;
                p2.y = y;
                code2 = computeCode(p2);
            }
        }
    }
    if (accept) {
        glBegin(GL_LINES);
            glPointSize(1);
            glColor3f(1, 0, 0);
            glVertex2f(p1.X(), p1.Y());
            glVertex2f(p2.X(), p2.Y());
        glEnd();
    }
}

bool test(float p, float q, float &t1, float &t2) {
    float t = q / p;
    if (p<0) {
        if (t>t2) return false;
        if (t>t1) t1 = t;
    }
    else if (p>0) {
        if (t<t1) return false;
        if (t<t2) t2 = t;
    }
    else
        return q<0;
    return true;
}

void liangBarsky(Point p1, Point p2) {
    float t1 = 0, t2 = 1;
    if (test(-(p2.X()-p1.X()), p1.X()-xmin, t1, t2)) {
        if (test(p2.X()-p1.X(), xmax-p1.X(),t1, t2)) {
            if (test(-(p2.Y()-p1.Y()), p1.Y()-ymin, t1, t2)) {
                if (test(p2.Y()-p1.Y(), ymax-p1.Y(), t1, t2)) {
                    if (t2<1) {
                        p2.x = p1.X() + t2*(p2.X()-p1.X());
                        p2.y = p1.Y() + t2*(p2.Y()-p1.Y());
                    }
                    if (t1>0) {
                        p1.x = p1.X() + t1*(p2.X()-p1.X());
                        p1.y = p1.Y() + t1*(p2.Y()-p1.Y());
                    }
                    glBegin(GL_LINES);
                        glColor3f(1, 0, 0);
                        glVertex2f(p1.X(), p1.Y());
                        glVertex2f(p2.X(), p2.Y());
                    glEnd();
                }
            }
        }
    }
}

void init() {
    glClearColor(1, 1, 1, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i=0; i<c-1; i++) {
        Point p1 = points[i];
        Point p2 = points[i+1];
        glBegin(GL_LINE_LOOP);
            glColor3f(0, 0, 0);
            glVertex2f(p1.X(), p1.Y());
            glVertex2f(p2.X(), p1.Y());
            glVertex2f(p2.X(), p2.Y());
            glVertex2f(p1.X(), p2.Y());
        glEnd();
        xmin = p1.X();
        ymin = p2.Y();
        xmax = p2.X();
        ymax = p1.Y();
        if (i==0)
            break;
    }
    for (int i=2; i<c-1; i++) {
        Point p1 = points[i];
        Point p2 = points[i+1];
        glBegin(GL_LINES);
            glColor3f(0, 0, 1);
            glVertex2f(p1.X(), p1.Y());
            glVertex2f(p2.X(), p2.Y());
        glEnd();
        //dda(p1, p2)
        //generalBresenham(p1, p2);
        liangBarsky(p1, p2);
        i++;
    }
    glFlush();
}

void mouse(int button, int action, int xmouse, int ymouse) {
    if (button==GLUT_LEFT_BUTTON && action==GLUT_DOWN) {
        Point p(xmouse, HEIGHT-ymouse);
        points.push_back(p);
        c++;
    }
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Practice");
    glutMouseFunc(mouse);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

-bash-4.3$
