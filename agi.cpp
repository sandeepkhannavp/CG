// AGI - CG

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <bits/stdc++.h>
#include <vector>
#include <windows.h>

#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#define WIDTH 640
#define HEIGHT 480

using namespace std;

float xwmin = WIDTH / 2 - 200, xwmax = WIDTH / 2 + 200, ywmin = HEIGHT / 2 - 100, ywmax = HEIGHT / 2 + 100;
enum Edge {Left, Right, Bottom, Top};

class Point {
    public:
        float x;
        float y;

        Point() {
            x = 0;
            y = 0;
        }

        Point(float xx, float yy) {
            x = xx;
            y = yy;
        }
};

class Colour {
    public:
        int r;
        int g;
        int b;

        Colour() {
            r = 0;
            g = 0;
            b = 0;
        }

        Colour(int rr, int gg, int bb) {
            r = rr;
            g = gg;
            b = bb;
        }

        void display() {
            cout<<"Red:"<<r<<"  Green:"<<g<<"  Blue:"<<b<<"\n";
        }
};

class FColour {
    public:
    float r;
    float g;
    float b;

    FColour() {
        r = 0;
        g = 0;
        b = 0;
    }

    FColour(float rr, float gg, float bb) {
        r = rr;
        g = gg;
        b = bb;
    }

    FColour(float pixel[3]) {
        r = pixel[0];
        g = pixel[1];
        b = pixel[2];
    }

    void display() {
        cout<<"Red:"<<r<<"  Green:"<<g<<"  Blue:"<<b<<"\n";
    }
};

Point wMin(xwmin, ywmin);
Point wMax(xwmax, ywmax);

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glClearColor(0, 0, 0, 1);
}

static void drawPoint(float x, float y)
{
    glPointSize(2.0f);
    glColor3d(1, 1, 1);

    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

bool inside(Point p, Edge e) {
    switch(e) {
        case Left:
            if(p.x < wMin.x) {
                return false;
            }
            break;
        case Right:
            if(p.x > wMax.x) {
                return false;
            }
            break;
        case Bottom:
            if(p.y < wMin.y) {
                return false;
            }
            break;
        case Top:
            if(p.y > wMax.y) {
                return false;
            }
            break;
    }

    return true;
}

bool cross(Point p1, Point p2, Edge e) {
    if(inside(p1, e) == inside(p2, e)) {
        return false;
    } else {
        return true;
    }
}

Point intersect(Point p1, Point p2, Edge e) {
    Point intersection;
    float m;

    if(p1.x != p2.x) {
        m = (p2.y - p1.y) / (p2.x - p1.x);
    }

    switch(e) {
        case Left:
            intersection.x = wMin.x;
            intersection.y = p2.y + (wMin.x - p2.x) * m;
            break;
        case Right:
            intersection.x = wMax.x;
            intersection.y = p2.y + (wMax.x - p2.x) * m;
            break;
        case Bottom:
            intersection.y = wMin.y;
            if(p1.x != p2.x) {
                intersection.x = p2.x + (wMin.y - p2.y) / m;
            } else {
                intersection.x = p2.x;
            }
            break;
        case Top:
            intersection.y = wMax.y;
            if(p1.x != p2.x) {
                intersection.x = p2.x + (wMax.y - p2.y) / m;
            } else {
                intersection.x = p2.x;
            }
            break;
    }

    return intersection;
}

vector<Point> clip(vector<Point> polygon, Edge e, vector<Point> &out) {
    vector<Point> clipped;
    int n = polygon.size();

    for(int i = 0, j = 1; i < n; i++, j = (++j) % n) {
        if(cross(polygon[i], polygon[j], e)) {
            Point intersection = intersect(polygon[i], polygon[j], e);
            if(inside(polygon[i], e)) {      // Inside -> Outside
                clipped.push_back(intersection);
                out.push_back(intersection);
                out.push_back(polygon[j]);
            } else {                         // Outside -> Inside
                clipped.push_back(intersection);
                clipped.push_back(polygon[j]);
                out.push_back(intersection);
            }
        } else {
            if(inside(polygon[i], e)) {     // Inside -> Inside
                clipped.push_back(polygon[j]);
            } else {                        // Outside -> Outside
                out.push_back(polygon[j]);
            }
        }
    }

    return clipped;
}

void drawPolygon(vector<Point> polygon, int r, int g, int b) {
    glColor3d(r, g, b);
    glLineWidth(2);

    int n = polygon.size();

    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < n; i++) {
            glVertex2f(polygon[i].x, HEIGHT - polygon[i].y);
        }
    glEnd();
}

void drawPolygon(vector<Point> polygon, FColour c) {
    glColor3f(c.r, c.g, c.b);

    int n = polygon.size();

    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < n; i++) {
            glVertex2f(polygon[i].x, HEIGHT - polygon[i].y);
        }
    glEnd();
}

static void sutherlandHodgeman(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        vector<Point> clipped;
        vector<Point> out;

        clipped = clip(polygon, Left, out);
        clipped = clip(clipped, Right, out);
        clipped = clip(clipped, Bottom, out);
        clipped = clip(clipped, Top, out);

        // drawPolygon(out, 1, 0, 0);
        drawPolygon(clipped, 0, 1, 0);

        glFlush();
        polygon.clear();
    }
}

bool sameColour(FColour c1, FColour c2) {
    if(fabs(c1.r - c2.r) < 0.05 && fabs(c1.g - c2.g) < 0.05 && fabs(c1.b - c2.b) < 0.05) {
        return true;
    } else {
        return false;
    }
}

void bFill(int x, int y, FColour fillColour, FColour boundary) {
    // cout<<"x - "<<x<<"\ty - "<<y<<"\n";
    float pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);

    FColour current(pixel);

    if(!sameColour(current, boundary) && !sameColour(current, fillColour)) {
        glBegin(GL_POINTS);
            glColor3f(fillColour.r, fillColour.g, fillColour.b);
            glVertex2d(x, y);
        glEnd();
        glFlush();

        bFill(x + 2, y, fillColour, boundary);
        bFill(x - 2, y, fillColour, boundary);
        bFill(x, y + 2, fillColour, boundary);
        bFill(x, y - 2, fillColour, boundary);
    }
}

static void boundaryFill(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        Point interior(xx, yy);
        FColour boundary(1.0f, 0.0f, 1.0f);
        FColour fillColour(0.0f, 1.0f, 1.0f);

        drawPolygon(polygon, boundary);

        bFill(interior.x, HEIGHT - interior.y, fillColour, boundary);
        cout<<"Finished filling!\n";

        glFlush();
        polygon.clear();
    }
}

void flood(int x, int y, FColour fillColour, FColour oldColour) {
    // cout<<"x - "<<x<<"\ty - "<<y<<"\n";
    float pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);

    FColour current(pixel);

    if(sameColour(current, oldColour)) {
        glBegin(GL_POINTS);
            glColor3f(fillColour.r, fillColour.g, fillColour.b);
            glVertex2d(x, y);
        glEnd();
        glFlush();

        flood(x + 2, y, fillColour, oldColour);
        flood(x - 2, y, fillColour, oldColour);
        flood(x, y + 2, fillColour, oldColour);
        flood(x, y - 2, fillColour, oldColour);
    }
}

static void floodFill(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        Point interior(xx, yy);
        FColour oldColour(0.0f, 0.0f, 0.0f);
        FColour fillColour(0.0f, 1.0f, 1.0f);

        drawPolygon(polygon, fillColour);

        flood(interior.x, HEIGHT - interior.y, fillColour, oldColour);
        cout<<"Finished filling!\n";

        glFlush();
        polygon.clear();
    }
}

static void yx(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        drawPolygon(polygon, 1, 1, 1);
        glFlush();

        FColour background(0.0f, 0.0f, 0.0f);

        for(int y = 0; y <= HEIGHT; y++) {      // For each scan line
            vector<int> intersections;
            int n = polygon.size();

            for(int i = 0, j = 1; i < n; i++, j = (++j) % n) {      // For each edge
                /*
                if(polygon[i].x != polygon[j].x) {
                    int x = scanLineIntersect(y, polygon[i], polygon[j]);
                    intersections.push_back(x);
                }
                */

                int y1 = polygon[i].y;
                int y2 = polygon[j].y;

                if(y1 < y2) {
                    if(y >= y1 && y < y2) {
                        int x = (y - y1) * (polygon[j].x - polygon[i].x) / (y2 - y1) + polygon[i].x;
                        intersections.push_back(x);
                    }
                } else {
                    if (y >= y2 && y < y1) {
                        int x = (y - y1) * (polygon[j].x - polygon[i].x) / (y2 - y1) + polygon[i].x;
                        intersections.push_back(x);
                    }
                }
            }

            sort(intersections.begin(), intersections.end());
            int m = intersections.size();

            glColor3f(0, 1, 0);
            for(int i = 0; i < m; i += 2) {
                int x1 = intersections[i];
                int x2 = intersections[i + 1];
                glBegin(GL_LINES);
                    glVertex2i(x1, HEIGHT - y);
                    glVertex2i(x2, HEIGHT - y);
                glEnd();

                glFlush();
            }
        }

        cout<<"Finished filling!\n";

        glFlush();
        polygon.clear();
    }
}

static void y_x(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        drawPolygon(polygon, 1, 1, 1);
        glFlush();

        FColour background(0.0f, 0.0f, 0.0f);

        for(int y = 0; y <= HEIGHT; y++) {      // For each scan line
            vector<int> intersections;
            int n = polygon.size();

            for(int i = 0, j = 1; i < n; i++, j = (++j) % n) {      // For each edge
                int y1 = polygon[i].y;
                int y2 = polygon[j].y;

                if(y1 < y2) {
                    if(y >= y1 && y < y2) {
                        int x = (y - y1) * (polygon[j].x - polygon[i].x) / (y2 - y1) + polygon[i].x;
                        intersections.push_back(x);
                    }
                } else {
                    if (y >= y2 && y < y1) {
                        int x = (y - y1) * (polygon[j].x - polygon[i].x) / (y2 - y1) + polygon[i].x;
                        intersections.push_back(x);
                    }
                }
            }

            sort(intersections.begin(), intersections.end());
            int m = intersections.size();

            glColor3f(0, 1, 0);
            for(int i = 0; i < m; i += 2) {
                int x1 = intersections[i];
                int x2 = intersections[i + 1];
                glBegin(GL_LINES);
                    glVertex2i(x1, HEIGHT - y);
                    glVertex2i(x2, HEIGHT - y);
                glEnd();

                glFlush();
            }
        }

        cout<<"Finished filling!\n";

        glFlush();
        polygon.clear();
    }
}

static void keyboardHandler(unsigned char key, int xx, int yy) {
    switch(key) {
        case '1':
            cout<<"Sutherland - Hodgeman Algorithm\n";

            // Draw a fresh clip window
            glClear(GL_COLOR_BUFFER_BIT);

            glColor3f(1.0, 1.0, 1.0);
            glLineWidth(3);

            glBegin(GL_LINE_LOOP);
                glVertex2i(xwmin, ywmin);
                glVertex2i(xwmax, ywmin);
                glVertex2i(xwmax, ywmax);
                glVertex2i(xwmin, ywmax);
            glEnd();

            glFlush();

            glutMouseFunc(sutherlandHodgeman);
            break;

        case '2':
            cout<<"Boundary Fill Algorithm\n";

            glLineWidth(2);
            glPointSize(1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();

            glutMouseFunc(boundaryFill);
            break;

        case '3':
            cout<<"Flood Fill Algorithm\n";

            glLineWidth(2);
            glPointSize(1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();

            glutMouseFunc(floodFill);
            break;

        case '4':
            cout<<"YX Algorithm\n";

            glLineWidth(1);
            glPointSize(1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();

            glutMouseFunc(yx);
            break;

        case '5':
            cout<<"Y-X Algorithm\n";

            glLineWidth(1);
            glPointSize(1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();

            glutMouseFunc(y_x);
            break;
    }
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    cout << "1. Polygon Clipping\n2. Boundary Fill\n3. Flood Fill\n4. YX Algorithm\n5. Y-X Algorithm\n\n";
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WIDTH / 4, HEIGHT / 4);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("CA 2");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();

    return EXIT_SUCCESS;
}
