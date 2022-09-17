#include <iostream>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <functional>

class Circle
{
public:
	float x_, y_, r_, alpha_;
	Circle(float x = 0.0, float y = 0.0, float r = 10.0) : x_(x), y_(y), alpha_(1.0){};
	void draw();
};

void Circle::draw()
{
	glColor4f(1.0, 1.0, 0.0, alpha_);
	glBegin(GL_LINE_LOOP);
	for (float i = 0.0; i < 2 * 3.14; i += 3.14 / 18)
	{
		glVertex2f(x_ + r_ * std::sin(i), y_ + r_ * std::cos(i));
	}
	glEnd();
};

class Draw
{
	std::vector<Circle> circ_;
	bool down_ = false;

public:
	Draw(){};
	~Draw(){};
	void draw();
	void timer(int value);
	void timer2(int value);
};

void Draw::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::vector<Circle>::iterator itr = circ_.begin();
	while (itr != circ_.end())
	{
		itr->draw();
		if (itr->alpha_ <= 0.05f)
		{
			itr = circ_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA);
	glutSwapBuffers();
}

void Draw::timer(int value)
{
	for (std::vector<Circle>::iterator itr = circ_.begin(); itr != circ_.end(); itr++)
	{
		itr->r_++;
		itr->alpha_ /= 1.05;
	}
	glutPostRedisplay();
	// void (*func)(int) = this->timer;
	// glutTimerFunc(40, func, 0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex3f(-0.6, -0.75, 0.5);
	glColor3f(0, 1, 0);
	glVertex3f(0.6, -0.75, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0.75, 0);
	glEnd();

	glFlush();
}

std::function<void(void)> f(Draw &obj)
{
	return [&]()
	{
		obj.draw();
	};
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(80, 80);
	glutInitWindowSize(400, 300);
	glutCreateWindow("A Simple animation");

	Draw draw;

	// void (*func)(int) = Draw::draw;

	// to_callback<Draw, Draw::draw>(draw);
	// glutDisplayFunc(f);

	//   glutTimerFunc(40, Draw::timer, 0);

	// glutDisplayFunc(display);

	glutMainLoop();
}