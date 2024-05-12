#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);
	}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmueve() { return mueve; }
	GLfloat getangulollantas() { return angulollantas; }
	GLfloat getangulocofre() { return angulocofre; }
	GLfloat getlight() { return arregloluz; }
	GLfloat getPrender() { return prender_luz; }
	GLfloat getPrender2() { return prender_luz2; }
	GLfloat getAnguloPuertaLalo() { return anguloPuertaLalo; }
	GLfloat getPosGolf() { return posGolf; }
	GLfloat getRotGolf() { return rotGolf; }
	GLboolean getprenderluz() { return prenderluz; }	//FAROS DE LUZ DEL CARRO.
	GLboolean getprender_luz2() { return prender_luz2; }	//FAROS DE LUZ DEL CARRO.



	GLfloat getPosPsyduckZ()
	{
		return posPsyduckZ;
	}

	GLfloat getPosPsyduckX()
	{
		return posPsyduckX;
	}



	GLfloat getPosCamaraZ()
	{
		if (angPsyduck == 0.0f)
			return posPsyduckZ - 12.0f;
		else if (angPsyduck == 90.0f)
			return posPsyduckZ;
		else if (angPsyduck == 180.0f)
			return(posPsyduckZ + 12.0f);
		else
			return(posPsyduckZ);
	}


	GLfloat getPosCamaraX()
	{
		if (angPsyduck == 0.0f)
			return posPsyduckX;
		else if (angPsyduck == 90.0f)
			return posPsyduckX - 12.0f;
		else if (angPsyduck == 180.0f)
			return(posPsyduckX);
		else
			return(posPsyduckX + 12.0f);
	}


	GLfloat getAngPsyduck() { return angPsyduck; }


	GLfloat getAngPieDer() { return angPieDer; }
	GLfloat getAngPieIzq() { return angPieIzq; }
	GLint getControlCamera() { return controlCamera; }
	GLint getControlCarrera() { return controlCarrera; }

	
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat rotax, rotay, rotaz;
	GLfloat mueve;
	GLfloat angulollantas = 0.0f;
	GLfloat angulocofre = 0.0f;
	GLfloat anguloPuertaLalo = 0.0f;
	GLfloat posGolf = 0.0f;
	GLfloat rotGolf = 0.0f;

	GLfloat posPsyduckZ = -290.0f;
	GLfloat posPsyduckX = 0.0f;

	GLfloat angPieDer = 0.0f;
	GLfloat angPieIzq = 0.0f;
	GLfloat seleccionPiePsyduck = 1.0f;
	GLfloat angPsyduck = 0.0f;
	GLint arregloluz = 0;
	GLint prender_luz = 1;
	GLint prender_luz2;
	GLboolean prenderluz;

	GLint controlCarrera = 1;

	GLint controlCamera = 1;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

