
////////////////////////////////////////////////////////////////////////////////
// This is temporary
#include "../../NDSE-Core/Core/NDSE.h" 
#include "../../../NDSE-bak/Plugins/VRAM_Reactor/VRAM_Reactor.h" 
#include "../../../NDSE-bak/Plugins/KEYPAD_Actor/KEYPAD_Actor.h" 

#ifdef _DEBUG
#pragma comment(lib, "../../NDSE-Core/VC/Debug/NDSEd.lib")
#pragma comment(lib, "../../../NDSE-bak/VC/Debug/VRAM_Reactord.lib")
#pragma comment(lib, "../../../NDSE-bak/VC/Debug/KEYPAD_Actord.lib")
#else
#pragma comment(lib, "../../NDSE-Core/VC/Release/NDSE.lib")
#pragma comment(lib, "../../../NDSE-bak/VC/Release/VRAM_Reactor.lib")
#pragma comment(lib, "../../../NDSE-bak/VC/Release/KEYPAD_Actor.lib")
#endif

////////////////////////////////////////////////////////////////////////////////

#include <boost/thread.hpp>
#include <iostream>
#include <gl/glut.h>

////////////////////////////////////////////////////////////////////////////////
// GFX


int width = 256;
int height = 192*2;

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

void arm7_cb(Fiber *f) {} // add code to handle CPU halts here

////////////////////////////////////////////////////////////////////////////////
// ARM9

void arm9_cb(Fiber *f) {} // add code to handle CPU halts here

////////////////////////////////////////////////////////////////////////////////
// INIT

void* STDCALL get_extension(const char *name)
{
	return wglGetProcAddress(name);
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

	Init();

	ARM7_Init(arm7_cb);
	ARM9_Init(arm9_cb);

	util::load_result loadres;
	UTIL_LoadFile("..\\fire.nds", &loadres, util::LH_UNKNOWN);

	ARM7_SetPC(loadres.arm7_entry);
	ARM9_SetPC(loadres.arm9_entry);
	ARM7_Continue();
	ARM9_Continue();
	

	VideoInit2(get_extension);
	InputInit2();
	glutMainLoop();
	return 0;
}

