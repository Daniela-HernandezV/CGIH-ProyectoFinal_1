#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	mueve = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Entorno virtual interactivo: AVIARIO" , NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	// rotar llantas
	if (key == GLFW_KEY_1)
	{
		theWindow->angulollantas += 10.0;
	}
	if (key == GLFW_KEY_2)
	{
		theWindow->angulollantas -= 10.0;
	}

	// abrir ocerrar cofre
	if (key == GLFW_KEY_P)
	{
		if (theWindow->angulocofre < -75.0)
		{
		}
		else
		{
			theWindow->angulocofre -= 10.0;
		}
	}
	if (key == GLFW_KEY_M)
	{
		if (theWindow->angulocofre > -0.17)
		{
		}
		else
		{
			theWindow->angulocofre += 10.0;
		}
	}

	// para avanzar o retroceder 
	if (key == GLFW_KEY_U)
	{
		theWindow->mueve += 1.0;
		theWindow->arregloluz = 0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->mueve -= 1.0;
		//theWindow->arregloluz = 1;
	}
	// Luz de farol
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		theWindow->prender_luz = -1 * theWindow->prender_luz;
	}

	//VARITA
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		//theWindow->mueve -= 1.0;
		theWindow->prender_luz2 = !theWindow->prender_luz2;
	}



	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS) //PARA EL FARO IZQUIERDO
	{
		theWindow->prenderluz = !theWindow->prenderluz;
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key == GLFW_KEY_V)
	{
		theWindow->posGolf += 1.0f;
		theWindow->rotGolf += 10.0f;
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key == GLFW_KEY_C)
	{
		theWindow->posGolf -= 1.0f;
		theWindow->rotGolf -= 10.0f;
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		if (theWindow->angPsyduck < 3)
			theWindow->angPsyduck += 1.0f;
		else
			theWindow->angPsyduck = 0.0f;
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) //Carrera
	{
		theWindow->controlCarrera = theWindow->controlCarrera * -1;
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		theWindow->controlCamera = theWindow->controlCamera * -1;
	}


	if (key == GLFW_KEY_UP)
	{
		if (theWindow->angPsyduck == 0.0f)
			theWindow->posPsyduckZ += 1.0f;

		else if (theWindow->angPsyduck == 90.0f)
			theWindow->posPsyduckX += 1.0f;

		else if (theWindow->angPsyduck == 180.0f)
			theWindow->posPsyduckZ -= 1.0f;

		else
			theWindow->posPsyduckX -= 1.0f;


		if (theWindow->angPieDer == 30.0f)
		{
			theWindow->angPieDer = 0.0f;
			theWindow->angPieIzq = 30.0f;
		}
		else
		{
			theWindow->angPieIzq = 0.0f;
			theWindow->angPieDer = 30.0f;
		}
	}



	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		theWindow->angPieDer = 0.0f;
		theWindow->angPieIzq = 0.0f;
		if (theWindow->angPsyduck < 270.0f)
			theWindow->angPsyduck += 90.0f;
		else
			theWindow->angPsyduck = 0.0f;
	}



	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		theWindow->angPieDer = 0.0f;
		theWindow->angPieIzq = 0.0f;
		if (theWindow->angPsyduck > 0.0f)
			theWindow->angPsyduck -= 90.0f;
		else
			theWindow->angPsyduck = 270.0f;
	}



	else if (key == GLFW_KEY_DOWN)
	{
		if (theWindow->angPsyduck == 0.0f)
			theWindow->posPsyduckZ -= 1.0f;

		else if (theWindow->angPsyduck == 90.0f)
			theWindow->posPsyduckX -= 1.0f;

		else if (theWindow->angPsyduck == 180.0f)
			theWindow->posPsyduckZ += 1.0f;

		else
			theWindow->posPsyduckX += 1.0f;


		if (theWindow->angPieDer == -30.0f)
		{
			theWindow->angPieDer = 0.0f;
			theWindow->angPieIzq = -30.0f;
		}
		else
		{
			theWindow->angPieIzq = 0.0f;
			theWindow->angPieDer = -30.0f;
		}
	}




	else
	{
		theWindow->angPieDer = 0.0f;
		theWindow->angPieIzq = 0.0f;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}


void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

