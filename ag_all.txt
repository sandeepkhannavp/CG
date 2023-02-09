#include <windows.h>
#include <math.h>

#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

#define WIDTH 640
#define HEIGHT 480

using namespace std;

int xwmin = WIDTH / 2 - 200, xwmax = WIDTH / 2 + 200, ywmin = HEIGHT / 2 - 100, ywmax = HEIGHT / 2 + 100;
int tx1 = WIDTH / 2, ty1 = HEIGHT / 2, tx2 = WIDTH / 2 - 100, ty2 = HEIGHT / 2 - 100, tx3 = WIDTH / 2 + 100, ty3 = HEIGHT / 2 - 100;
const int delta = 5;

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glClearColor(0, 0, 0, 1);
}

static void drawPoint(int x, int y)
{
    glPointSize(2.0f);
    glColor3d(1,1,1);

    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

static void DDA(int button, int state, int xx, int yy) {
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);
            // cout<<"("<<xx<<", "<<yy<<")\n\n";

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);
        }
    }
    if(points.size() == 2) {
        int x1 = points[0][0], y1 = points[0][1], x2 = points[1][0], y2 = points[1][1];
        points.clear();

        int dx = x2 - x1, dy = y2 - y1;
        int steps, k;
        float xInc, yInc;
        float x = x1, y = y1;

        if(abs(dx) > abs(dy)) {
            steps = abs(x);
        } else {
            steps = abs(y);
        }

        xInc = dx / (float)steps;
        yInc = dy / (float)steps;

        drawPoint(x, HEIGHT - y);

        for(k = 0; k < steps; k++) {
            x += xInc;
            y += yInc;
            drawPoint(x, HEIGHT - y);
        }
    }
    glFlush();
}

static void bresenham(int button, int state, int xx, int yy) {
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);
            cout<<"("<<xx<<", "<<yy<<")\n\n";

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);
        }
    }
    if(points.size() == 2) {
        int x1 = points[0][0], y1 = points[0][1], x2 = points[1][0], y2 = points[1][1];
        points.clear();

        int dx, dy, x, y, s1, s2, p;
        bool interchange = false;

        dx = abs(x2 - x1);
        dy = abs(y2 - y1);
        if((x2 - x1) < 0) {
            s1 = -1;
        } else if((x2 - x1) > 0) {
            s1 = 1;
        } else {
            s1 = 0;
        }
        if((y2 - y1) < 0) {
            s2 = -1;
        } else if((y2 - y1) > 0) {
            s2 = 1;
        } else {
        s2 = 0;
        }

        if(dy > dx) {
            int temp = dy;
            dy = dx;
            dx = temp;
            interchange = true;
        }

        p = 2 * dy - dx;
        x = x1;
        y = y1;

        for(int i = 1; i < dx; i++) {
            if(p < 0) {
                if(interchange) {
                    y += s2;
                } else {
                    x += s1;
                }
                p += 2 * dy;
            }
            else {
                y += s2;
                x += s1;
                p += 2 * dy - 2 * dx;
            }
            drawPoint(x, HEIGHT - y);
        }
    }
    glFlush();
}

static void circlePlotPoints(int xc, int yc, int x, int y) {
    drawPoint(xc + x, HEIGHT - (yc + y));
    drawPoint(xc - x, HEIGHT - (yc + y));
    drawPoint(xc + x, HEIGHT - (yc - y));
    drawPoint(xc - x, HEIGHT - (yc - y));
    drawPoint(xc + y, HEIGHT - (yc + x));
    drawPoint(xc - y, HEIGHT - (yc + x));
    drawPoint(xc + y, HEIGHT - (yc - x));
    drawPoint(xc - y, HEIGHT - (yc - x));
    glFlush();
}

static void midpointCircle(int button, int state, int xx, int yy)
{
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);

            glFlush();
        }
    } else {
        int xc = points[0][0], yc = points[0][1], xr = points[1][0], yr = points[1][1];
        points.clear();

        int r = sqrt((xc - xr) * (xc - xr) + (yc - yr) * (yc - yr));
        int p = 1 - r;
        int x = 0, y = r;

        circlePlotPoints(xc, yc, x, y);

        while(x < y) {
            cout<<"x = "<<x<<"  y = "<<y<<"\n";
            x += 1;
            if(p < 0) {
                p += 2 * x + 1;
            } else {
                y -= 1;
                p += 2 * (x - y) + 1;
            }
            circlePlotPoints(xc, yc, x, y);
        }
    }
}

static bool pointClipping(int x, int y) {
    if(x >= xwmin && x <= xwmax and y >= ywmin && y <= ywmax) {
        return true;
    } else {
        return false;
    }
}

static void getRegionCode(int x, int y, vector<int>& r) {
    if(x - xwmin < 0) {
        r[3] = 1;
    }
    if(xwmax - x < 0) {
        r[2] = 1;
    }
    if(y - ywmin < 0) {
        r[1] = 1;
    }
    if(ywmax - y < 0) {
        r[0] = 1;
    }
}

static bool completelyInside(vector<int> r1, vector<int> r2) {
    if(r1[0] == 0 && r1[1] == 0 && r1[2] == 0 && r1[3] == 0 && r2[0] == 0 && r2[1] == 0 && r2[2] == 0 && r2[3] == 0) {
        return true;
    } else {
        return false;
    }
}

static bool completelyOutside(vector<int> r1, vector<int> r2) {
    if((r1[0] == 1 && r2[0] == 1) || (r1[1] == 1 && r2[1] == 1) || (r1[2] == 1 && r2[2] == 1) || (r1[3] == 1 && r2[3] == 1)) {
        return true;
    } else {
        return false;
    }
}

static void cohenSutherland(int button, int state, int xx, int yy)
{
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);
            // cout<<"("<<xx<<", "<<yy<<")\n\n";

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);

            glFlush();
        }
    } else {
        int x1 = points[0][0], y1 = points[0][1], x2 = points[1][0], y2 = points[1][1];
        points.clear();

        /*
        glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
        glEnd();
        */

        // Setting region codes for the end points
        vector<int> r1(4, 0);
        getRegionCode(x1, y1 ,r1);
        vector<int> r2(4, 0);
        getRegionCode(x2, y2, r2);

        // Check if the line is completely inside
        if(r1[0] == 0 && r1[1] == 0 && r1[2] == 0 && r1[3] == 0 && r2[0] == 0 && r2[1] == 0 && r2[2] == 0 && r2[3] == 0) {
            glColor3d(0, 1, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
            return;
        }

        // Check if line is completely outside
        if((r1[0] == 1 && r2[0] == 1) || (r1[1] == 1 && r2[1] == 1) || (r1[2] == 1 && r2[2] == 1) || (r1[3] == 1 && r2[3] == 1)) {
            glColor3d(1, 0, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
            return;
        }

        float m = (y2 - y1) / (float)(x2 - x1);
        float c = y1 - m * x1;
        // Line might pass through the clip window
        while(!completelyInside(r1, r2) && !completelyOutside(r1, r2)) {
            /*
            cout<<"("<<x1<<", "<<y1<<")\t("<<x2<<", "<<y2<<")\n";
            drawPoint(x1, HEIGHT - y1);
            drawPoint(x2, HEIGHT - y2);
            glFlush();
            */
            // If P1 is outside the clip window
            if(!pointClipping(x1, y1)) {
                // Outside left boundary
                int xOld = x1, yOld = y1;
                if(r1[3] == 1) {
                    x1 = xwmin;
                    y1 = m * x1 + c;
                    r1[3] = 0;
                } else if(r1[2] == 1) {
                    x1 = xwmax;
                    y1 = m * x1 + c;
                    r1[2] = 0;
                } else if(r1[1] == 1) {
                    y1 = ywmin;
                    x1 = (y1 - c) / m;
                    r1[1] = 0;
                } else if(r1[0] == 1) {
                    y1 = ywmax;
                    x1 = (y1 - c) / m;
                    r1[0] = 0;
                }

                glColor3d(1, 0, 0);
                glBegin(GL_LINES);
                    glVertex2f(x1, HEIGHT - y1);
                    glVertex2f(xOld, HEIGHT - yOld);
                glEnd();
                glFlush();
            }
            // If P2 is outside the clip window
            else if(!pointClipping(x2, y2)) {
                int xOld = x2, yOld = y2;
                // Outside left boundary
                if(r2[3] == 1) {
                    x2 = xwmin;
                    y2 = m * x2 + c;
                    r2[3] = 0;
                } else if(r2[2] == 1) {
                    x2 = xwmax;
                    y2 = m * x2 + c;
                    r2[2] = 0;
                } else if(r2[1] == 1) {
                    y2 = ywmin;
                    x2 = (y2 - c) / m;
                    r2[1] = 0;
                } else if(r2[0] == 1) {
                    y2 = ywmax;
                    x2 = (y2 - c) / m;
                    r2[0] = 0;
                }

                glColor3d(1, 0, 0);
                glBegin(GL_LINES);
                    glVertex2f(xOld, HEIGHT - yOld);
                    glVertex2f(x2, HEIGHT - y2);
                glEnd();
                glFlush();
            }
        }
        if(completelyInside(r1, r2)) {
            glColor3d(0, 1, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
        } else {
            glColor3d(1, 0, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
            // cout<<"The line is completely outside the clip window\n";
        }
    }

    return;
}

static void liangBarsky(int button, int state, int xx, int yy)
{
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);

            glFlush();
        }
    } else {
        int x1 = points[0][0], y1 = points[0][1], x2 = points[1][0], y2 = points[1][1];
        points.clear();

        // cout<<"\n\n("<<x1<<", "<<y1<<")  ("<<x2<<", "<<y2<<")\n";
        // cout<<"u1 = 0  u2 = 1"<<"\n";

        int deltaX = x2 - x1, deltaY = y2 - y1;
        vector<int> p{-deltaX, deltaX, -deltaY, deltaY};
        vector<int> q{x1 - xwmin, xwmax - x1, y1 - ywmin, ywmax - y1};
        float u1 = 0, u2 = 1;
        vector<float> r1;
        vector<float> r2;

        for(int i = 0; i < 4; i++) {
            if(p[i] == 0) {
                if(q[i] < 0) {
                    // The line is completely outside the boundary
                    glColor3d(1, 0, 0);
                    glBegin(GL_LINES);
                        glVertex2f(x1, HEIGHT - y1);
                        glVertex2f(x2, HEIGHT - y2);
                    glEnd();
                    glFlush();
                    return;
                }
            } else if(p[i] < 0) {
                r1.push_back(q[i] / (float)p[i]);
            } else {
                r2.push_back(q[i] / (float)p[i]);
            }
        }

        if(*max_element(r1.begin(), r1.end()) > u1) {
            u1 = *max_element(r1.begin(), r1.end());
        }
        if(*min_element(r2.begin(), r2.end()) < u2) {
            u2 = *min_element(r2.begin(), r2.end());
        }

        int x1_ = x1 + deltaX * u1, y1_ = y1 + deltaY * u1;
        int x2_ = x1 + deltaX * u2, y2_ = y1 + deltaY * u2;

        if(u1 > u2) {
            // The line is completely outside the boundary
            glColor3d(1, 0, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
        } else {
            glColor3d(0, 1, 0);
            glBegin(GL_LINES);
                glVertex2f(x1_, HEIGHT - y1_);
                glVertex2f(x2_, HEIGHT - y2_);
            glEnd();

            glColor3d(1, 0, 0);
            /*
            if(x1 < x1_) {
                glBegin(GL_LINES);
                    glVertex2f(x1, HEIGHT - y1);
                    glVertex2f(x1_, HEIGHT - y1_);
                glEnd();
            } else {
                glBegin(GL_LINES);
                    glVertex2f(x1_, HEIGHT - y1_);
                    glVertex2f(x1, HEIGHT - y1);
                glEnd();
            }

            if(x2 < x2_) {
                glBegin(GL_LINES);
                    glVertex2f(x2, HEIGHT - y2);
                    glVertex2f(x2_, HEIGHT - y2_);
                glEnd();
            } else {
                glBegin(GL_LINES);
                    glVertex2f(x2_, HEIGHT - y2_);
                    glVertex2f(x2, HEIGHT - y2);
                glEnd();
            }
            */
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x1_, HEIGHT - y1_);
            glEnd();

            glBegin(GL_LINES);
                glVertex2f(x2, HEIGHT - y2);
                glVertex2f(x2_, HEIGHT - y2_);
            glEnd();

            glFlush();
        }
    }

    return;
}

/*
static vector<vector<int>> drawObject(int button, int state, int xx, int yy) {
    static vector<vector<int> > points;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        vector<int> point;
        point.push_back(xx);
        point.push_back(yy);
        points.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(3);

        glBegin(GL_LINE_LOOP);
            for(int i = 0; i < points.size(); i++) {
                glVertex2i(points[i][0], points[i][1]);
            }
        glEnd();

        glFlush();
    }

    return points;
}
*/

static vector<vector<int>> multiply(vector<vector<int>> a, vector<vector<int>> b) {
    int rows = a[0].size();
    int columns = b.size();
    int cols = a.size();

    vector<vector<int>> c(rows, vector<int> (columns, 0));

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            for(int k = 0; k < cols; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return c;
}

static void drawTriangle() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
        glVertex2i(tx1, ty1);
        glVertex2i(tx2, ty2);
        glVertex2i(tx3, ty3);
    glEnd();

    glFlush();
}

static void transformations(int key_, int xx, int yy) {
    vector<vector<int>> object {
        {tx1, tx2, tx3},
        {ty1, ty2, ty3},
        {1, 1, 1}
    };
    vector<vector<int>> transformMatrix {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 1}
    };
    switch(key_) {
        case GLUT_KEY_UP:
            // ty1 += delta;
            // ty2 += delta;
            // ty3 += delta;
            transformMatrix[0][0] = 1;
            transformMatrix[1][1] = 1;
            transformMatrix[1][2] = delta;

            vector<vector<int>> result;
            result = multiply(transformMatrix, object);

            tx1 = result[0][0];
            tx2 = result[0][1];
            tx3 = result[0][2];
            ty1 = result[1][0];
            ty2 = result[1][1];
            ty3 = result[1][2];

            drawTriangle();
            break;
    }
}

/*
1. DDA Line
2. Bresenham Line
3. Mid-Point Circle
4. Cohen-Sutherland Clipping
5. Liang-Barsky Clipping
6. Transformations
*/
static void keyboardHandler(unsigned char key, int xx, int yy)
{
    switch(key) {
    case '1':
        cout<<"DDA Line Algorithm\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glFlush();
        glutMouseFunc(DDA);
        break;

    case '2':
        cout<<"Bresenham Line Algorithm\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glFlush();
        glutMouseFunc(bresenham);
        break;

    case '3':
        cout<<"Mid - Point Circle Algorithm\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glFlush();
        glutMouseFunc(midpointCircle);
        break;

    case '4':
        cout<<"Cohen - Sutherland Line Clipping\n";

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

        glutMouseFunc(cohenSutherland);
        break;
    case '5':
        cout<<"Liang - Barsky Line Clipping\n";

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
        glutMouseFunc(liangBarsky);
        break;

    case '6':
        cout<<"Transformations\n";

        drawTriangle();

        glutSpecialFunc(transformations);

        break;

    default:
        break;
    }
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    cout<<"1. DDA Line\n2. Bresenham Line\n3. Mid-Point Circle\n4. Cohen-Sutherland Clipping\n5. Liang-Barsky Clipping\n6. Transformations\n\n";
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WIDTH / 4, HEIGHT / 4);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("CA1");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();

    return EXIT_SUCCESS;
}


