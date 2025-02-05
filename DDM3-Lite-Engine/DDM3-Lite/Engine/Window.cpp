// Window.cpp

// Header include
#include "Window.h"

// File includes
#include "Managers/ConfigManager.h"

// Static library includes
#include <functional>
#include <iostream>

DDM3::Window::Window()
{
	// Set the width of the window
	m_Window.Width = ConfigManager::GetInstance().GetInt("WindowWidth");
	// Set the height of the window
	m_Window.Height = ConfigManager::GetInstance().GetInt("WindowHeight");

	// Initialize the window
	InitWindow();
}

DDM3::Window::~Window()
{
	// Destroy the window
	glfwDestroyWindow(m_Window.pWindow);

	// Terminate glfw
	glfwTerminate();
}

void DDM3::Window::SetFrameBufferResized(bool value)
{
	// Set the value of frameBufferResized to the requested value
	m_Window.FrameBufferResized = value;
}

void DDM3::Window::AddCallback(void* object, std::function<void(int, const char**)> function)
{
	if (!m_DropFileCallbacks.contains(object))
	{
		m_DropFileCallbacks[object] = function;
	}
}

void DDM3::Window::RemoveCallback(void* object)
{
	m_DropFileCallbacks.erase(object);
}

void DDM3::Window::InitWindow()
{
	// Initialize glfw
	glfwInit();

	//Tell GLFW not to create an OpenGL context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Get a reference to the configmanager for later use
	auto& configManager{ ConfigManager::GetInstance() };

	// Get info ffrom config manager
	bool fullScreen{ configManager.GetBool("FullScreen") };
	int monitorIndex{ configManager.GetInt("Monitor") };
	bool maximized{ configManager.GetBool("Maximized") };

	// Get a list of all the available monitors
	int count{};
	GLFWmonitor** monitors{ glfwGetMonitors(&count) };

	// Create monitor variable and initialize with nullptr
	// If fullscreen is enabled, this will hold the correct monitor
	// If fullscreen is not enabled, this will stay nullptr
	GLFWmonitor* monitor{ nullptr };

	if (fullScreen)
	{
		// If the monitor index is available, set monitor, if not, get primary monitor
		if (monitorIndex < count)
		{
			monitor = monitors[monitorIndex];
		}
		else
		{
			monitor = glfwGetPrimaryMonitor();
		}
	}
	else if (maximized)
	{
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	}

	//Initialize the window
	m_Window.pWindow = glfwCreateWindow(m_Window.Width, m_Window.Height, configManager.GetString("WindowName").c_str(),
		monitor, nullptr);


	// If fullscreen is disabled, the monitor index is available and isn't 0, calculate position for the window on the requested monitor
	if (!fullScreen && monitorIndex != 0 && monitorIndex < count)
	{
		glfwRestoreWindow(m_Window.pWindow);

		int x, y, w, h;

		// Get the current monitor
		auto currentMonitor{ monitors[monitorIndex] };

		// Get all the measurements of the monitor
		glfwGetMonitorWorkarea(currentMonitor, &x, &y, &w, &h);

		// Calculate new position for the screen
		int newX{ x + (w / 2) - (m_Window.Width / 2) };
		int newY{ y + (h / 2) - (m_Window.Width / 2) };

		// Get refreshrate
		auto refreshRate{ glfwGetVideoMode(currentMonitor)->refreshRate };

		// Set the new window position
		glfwSetWindowMonitor(m_Window.pWindow, nullptr, newX, newY, m_Window.Width, m_Window.Height, refreshRate);
	}

	// If the window isn't fullscreen, should be maximized and isn't yet, maximize it
	if (!fullScreen && maximized && !glfwGetWindowAttrib(m_Window.pWindow, GLFW_MAXIMIZED))
	{
		glfwMaximizeWindow(m_Window.pWindow);
	}

	// Set window user pointer and callbacks
	glfwSetWindowUserPointer(m_Window.pWindow, this);
	glfwSetFramebufferSizeCallback(m_Window.pWindow, FramebufferResizeCallback);
	glfwSetWindowMaximizeCallback(m_Window.pWindow, MaximizeWindowCallback);
	glfwSetDropCallback(m_Window.pWindow, DropFileCallback);
}

void DDM3::Window::FramebufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
	// Get a reference to the window struct
	auto& window = GetInstance().GetWindowStruct();

	// Update values of global window variable after resizing
	window.FrameBufferResized = true;
	window.pWindow = pWindow;
	window.Width = width;
	window.Height = height;
}

void DDM3::Window::MaximizeWindowCallback(GLFWwindow* /*pWindow*/, int /*maximized*/)
{
	 
}


void DDM3::Window::DropFileCallback(GLFWwindow* window, int count, const char** paths)
{
	Window::GetInstance().CallDropFileCallbacks(count, paths);
}

void DDM3::Window::CallDropFileCallbacks(int count, const char** paths)
{
	for (auto& callback : m_DropFileCallbacks)
	{
		callback.second(count, paths);
	}
}
