#include "Joystick.h"
#include <string>
#include <sstream>
#include "../GR_cross_definitions.h"
#include <GLFW/glfw3.h>

gr::Joystick::Joystick()
{
}

bool gr::Joystick::IsConnected(JoystickPlayers n)
{
	return glfwJoystickPresent((int)n);
}

float gr::Joystick::GetAxesValue(JoystickAxes x)
{
	int count;
	const float* axes = glfwGetJoystickAxes((int)m_selected, &count);
	return axes[(int)x];
}

bool gr::Joystick::IsButtonPressed(JoystickButtons b)
{
	int count;
	const unsigned char* buttons = glfwGetJoystickButtons((int)m_selected, &count);
	return GLFW_PRESS == buttons[(int)b];
}