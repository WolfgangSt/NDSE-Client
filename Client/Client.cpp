#include <cstring>
#include <NDSE_SDK.h>
#include "../VRAM-Reactor/source/VRAM_Reactor.h" 
#include "../KEYPAD-Actor/source/KEYPAD_Actor.h"

////////////////////////////////////////////////////////////////////////////////

#include <boost/thread.hpp>
#include <iostream>
#include <GL/glut.h>

#ifdef WIN32
#include <windows.h>
#else
#include <GL/glx.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// GFX


int width = 256;
int height = 192*2;
boost::mutex ready7, ready9;

void display()
{
	Redisplay(width, height);
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void resize(int w, int h)
{
	width = w;
	height = h;
}

void keyboard(unsigned char key, bool down)
{
	switch (key)
	{
	case '1': return TriggerKey(Keys::START, down);
	case '2': return TriggerKey(Keys::SELECT, down);
	case 'q': return TriggerKey(Keys::BUTTON_L, down);
	case 'w': return TriggerKey(Keys::BUTTON_R, down);
	case 'a': return TriggerKey(Keys::BUTTON_A, down);
	case 's': return TriggerKey(Keys::BUTTON_B, down);
	case 'y': return TriggerKey(Keys::BUTTON_X, down);
	case 'x': return TriggerKey(Keys::BUTTON_Y, down);
	}
}

void keyboard2(int key, bool down)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: return TriggerKey(Keys::LEFT, down);
	case GLUT_KEY_RIGHT: return TriggerKey(Keys::RIGHT, down);
	case GLUT_KEY_UP: return TriggerKey(Keys::UP, down);
	case GLUT_KEY_DOWN: return TriggerKey(Keys::DOWN, down);
	}
}

void keydown(unsigned char key, int x, int y)
{
	keyboard(key, true);
}

void keyup(unsigned char key, int x, int y)
{
	keyboard(key, false);
}

void keydown2(int key, int x, int y)
{
	keyboard2(key, true);
}

void keyup2(int key, int x, int y)
{
	keyboard2(key, false);
}

////////////////////////////////////////////////////////////////////////////////
// ARM7

void arm7_cb(Fiber *f) // add code to handle CPU halts here
{
	std::cout << "ARM7 halted!" << std::endl;
	ready7.unlock();
} 

////////////////////////////////////////////////////////////////////////////////
// ARM9

void arm9_cb(Fiber *f) // add code to handle CPU halts here
{
	std::cout << "ARM9 halted!" << std::endl;
	ready9.unlock();
} 

////////////////////////////////////////////////////////////////////////////////
// INIT

void* STDCALL get_extension(const char *name)
{
#ifdef WIN32
	return wglGetProcAddress(name);
#else
	return (void*)glXGetProcAddress((const GLubyte*)name);
#endif
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("NDSE");
	glutDisplayFunc(display);
	glutKeyboardFunc(keydown);
	glutSpecialFunc(keydown2);
	glutKeyboardUpFunc(keyup);
	glutSpecialUpFunc(keyup2);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);


	ready7.lock();
	ready9.lock();

	Init();

	ARM7_Init(arm7_cb);
	ARM9_Init(arm9_cb);
	VideoInit2(get_extension);
	InputInit2();

	// wait for both CPUs ready 
	ready7.lock();
	ready9.lock();

	load_result loadres;
	std::cout << "Loading rom" << std::endl;
	if (!UTIL_LoadFile("../fire.nds", &loadres, LH_UNKNOWN))
	{
		std::cout << "Could not open rom" << std::endl;
		exit(-1);
	}
	std::cout << "ARM7 entry: " << std::hex << loadres.arm7_entry << std::endl;
	std::cout << "ARM9 entry: " << std::hex << loadres.arm9_entry << std::endl;
	ARM7_SetPC(loadres.arm7_entry);
	ARM9_SetPC(loadres.arm9_entry);	

	std::cout << "Running now" << std::endl;
	ARM7_Continue();
	ARM9_Continue();

	
	glutMainLoop();
	return 0;
}

