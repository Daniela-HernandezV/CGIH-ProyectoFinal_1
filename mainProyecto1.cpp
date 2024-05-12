/*
Proyecto aviario
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


/*TEXTURAS*/
//--------------------------------------------------------------------------------------*//
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
	
							/*DECLARACIÓN DE MODELOS*/
//--------------------------------------------------------------------------------------*//
// Modelos Daniela //
Model Llanta_M;
Model Lampara;
Model Puerta_izquierda;
Model Puerta_derecha;
Model Arco_piedra;
Model Cartel;
Model Elemento;
Model AutoB_base;		// base del auto con textura
Model AutoB_cofre_tex;	// cofre con textura
Model AutoB_LLTI;		// Llanta 1
Model AutoB_LLTD;		// Llanta 3
Model Nadder_Cuerpo;
Model Nadder_AlaI;
Model Nadder_AlaD;

Model Nadder_MusloD;
Model Nadder_MusloI;
Model Nadder_cabeza;
Model Nadder_mandibula;
Model Nadder_PataD;
Model Nadder_PataI;
Model Nadder_cola1;
Model Nadder_cola2;
Model Nadder_cola3;


//Skrill
Model Skrill_cuerpo;
Model Skrill_AlaD;
Model Skrill_AlaI;
Model Skrill_cola;
// Modelos Karla //
Model Jaula_A;
Model Domo;
Model DomoA;
Model Jaula_B;
Model Estatua_A;
Model Lampara_Spot; //Lampara spotlight para la estatua.
Model Keroppi_estatua_M;
Model Gazebo_M;
Model Cartel_M;
Model Arbusto_M;
Model Stand_M;
Model Bocina_M;
Model PuertaP_M;
Model Plataforma_M;
Model Cerca01_M;
Model Cerca02_M;
Model Cerca03_M;
Model Basura_M;
Model Senal_M;
Model PopCorn_M;
Model Cartel2_M;

// Modelos Eduardo Miguel //

Model dodrio;
Model rowlet;
Model banderaPokemon;
Model lamparaLalo;
Model expendedora;
Model paredPuertaLalo;
Model puertaLalo;
Model Eeve;
Model Plant02;
Model Sudowoodo;
Model Zubat;
Model Vigilancia;

//Partes Psybuck
Model psyduckBase;
Model psyduckPieI;
Model psyduckPieD;

//Partes carro de golf
Model carroGolfBase;
Model carroGolfLlanta;

// Modelos Cristopher //
Model banca;
Model Bandera_K;
Model Bandera_C;
Model Bandera_H;
Model Bandera_P;
Model box;
Model metalUp;
Model metal;
Model fruit;
Model normal;
Model chicken;
Model car;
Model stork;
Model vulture;
float movHeliy;
float movHeliyoffset;

//--------------------------------------------------------------------------------------*//

Skybox skybox;

						/*DECLARACIÓN DE MATERIALES*/
//--------------------------------------------------------------------------------------*//
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

							/*DECLARACIÓN DE LUCES*/
//--------------------------------------------------------------------------------------*//
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
PointLight pointLights3[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];
SpotLight spotLights3[MAX_SPOT_LIGHTS];
SpotLight spotLights4[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {		//Desde donde lo voy a ilumnar
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	-1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		-1.0f, 1.0f,		0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, -1.0f, -1.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);	//calcula normal por promedio de vertives

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	/*TEXTURAS*/
//--------------------------------------------------------------------------------------*//
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	//--------------------------------------------------------------------------------------*//


									/*MODELOS DANIELA*/
	//--------------------------------------------------------------------------------------*//



	//Nadder
	Nadder_Cuerpo = Model();
	Nadder_Cuerpo.LoadModel("Models/Daniela/Nadder_cuerpo_tex.obj");
	Nadder_AlaD = Model();
	Nadder_AlaD.LoadModel("Models/Daniela/Nadder_ala_der.obj");
	Nadder_AlaI = Model();		// Llanta trasera izquierda
	Nadder_AlaI.LoadModel("Models/Daniela/Nadder_ala_izq.obj");
	Nadder_MusloD = Model();		// Llanta trasera derecha
	Nadder_MusloD.LoadModel("Models/Daniela/Nadder_muslo_der.obj");
	Nadder_MusloI = Model();		// Llanta trasera derecha
	Nadder_MusloI.LoadModel("Models/Daniela/Nadder_muslo_izq.obj");
	Nadder_cabeza = Model();		// Llanta trasera derecha
	Nadder_cabeza.LoadModel("Models/Daniela/Nadder_cabeza_tex.obj");
	Nadder_mandibula = Model();		// Llanta trasera derecha
	Nadder_mandibula.LoadModel("Models/Daniela/Nadder_mandibula.obj");
	Nadder_PataD = Model();		// Llanta trasera derecha
	Nadder_PataD.LoadModel("Models/Daniela/Nadder_pata_izq.obj");
	Nadder_PataI = Model();		// Llanta trasera derecha
	Nadder_PataI.LoadModel("Models/Daniela/Nadder_pata_der.obj");
	Nadder_cola1 = Model();		// Llanta trasera derecha
	Nadder_cola1.LoadModel("Models/Daniela/Nadder_cola1.obj");
	Nadder_cola2 = Model();		// Llanta trasera derecha
	Nadder_cola2.LoadModel("Models/Daniela/Nadder_cola2.obj");
	Nadder_cola3 = Model();		// Llanta trasera derecha
	Nadder_cola3.LoadModel("Models/Daniela/Nadder_cola3.obj");


	//Skrill
	Skrill_cuerpo = Model();
	Skrill_cuerpo.LoadModel("Models/Daniela/Skrill_cuerpo.obj");
	Skrill_AlaD = Model();
	Skrill_AlaD.LoadModel("Models/Daniela/Skrill_alaD.obj");
	Skrill_AlaI = Model();		// Llanta trasera izquierda
	Skrill_AlaI.LoadModel("Models/Daniela/Skrill_alaI.obj");
	Skrill_cola = Model();		// Llanta trasera derecha
	Skrill_cola.LoadModel("Models/Daniela/Skrill_cola.obj");

									/*MODELOS KARLA*/
	//--------------------------------------------------------------------------------------*//
	Jaula_A = Model();
	Jaula_A.LoadModel("Models/Karla/JaulaA_M.obj");
	Domo = Model();
	Domo.LoadModel("Models/Karla/Domo_M.obj");
	DomoA = Model();
	DomoA.LoadModel("Models/Karla/DomoB_M.obj");
	Estatua_A = Model();
	Estatua_A.LoadModel("Models/Karla/Kuromi_estatua_M.obj");
	Lampara_Spot = Model();
	Lampara_Spot.LoadModel("Models/Karla/LamparaS_M.obj");
	Keroppi_estatua_M = Model();
	Keroppi_estatua_M.LoadModel("Models/Karla/Keroppi_estatua_M.obj");
	Gazebo_M = Model();
	Gazebo_M.LoadModel("Models/Karla/Gazebo_M.obj");
	Cartel_M = Model();
	Cartel_M.LoadModel("Models/Karla/Cartel_M.obj");
	Arbusto_M = Model();
	Arbusto_M.LoadModel("Models/Karla/Arbusto_M.obj");
	Stand_M = Model();
	Stand_M.LoadModel("Models/Karla/Stand_M.obj");
	Bocina_M = Model();
	Bocina_M.LoadModel("Models/Karla/Bocina_M.obj");
	PuertaP_M = Model();
	PuertaP_M.LoadModel("Models/Karla/PuertaP_M.obj");
	Plataforma_M = Model();
	Plataforma_M.LoadModel("Models/Karla/Auditorio_M.obj");
	Cerca01_M = Model();
	Cerca01_M.LoadModel("Models/Karla/Cerca01_M.obj");
	Cerca02_M = Model();
	Cerca02_M.LoadModel("Models/Karla/Cerca02_M.obj");
	Cerca03_M = Model();
	Cerca03_M.LoadModel("Models/Karla/Señal_M.obj");
	Senal_M = Model();
	Senal_M.LoadModel("Models/Karla/Señal_M.obj");
	Basura_M = Model();
	Basura_M.LoadModel("Models/Karla/Basura_M.obj");
	PopCorn_M = Model();
	PopCorn_M.LoadModel("Models/Karla/PopCorn.obj");
	Cartel2_M = Model();
	Cartel2_M.LoadModel("Models/Karla/Cartel2_M.obj");
								/*MODELOS EDUARDO*/
	//--------------------------------------------------------------------------------------*//

	dodrio = Model();
	dodrio.LoadModel("Models/Eduardo/dodrio.obj");
	rowlet = Model();
	rowlet.LoadModel("Models/Eduardo/rowlet.obj");
	banderaPokemon = Model();
	banderaPokemon.LoadModel("Models/Eduardo/Flag.obj");
	lamparaLalo = Model();
	lamparaLalo.LoadModel("Models/Eduardo/lamp.obj");
	paredPuertaLalo = Model();
	paredPuertaLalo.LoadModel("Models/Eduardo/PAREDES.obj");
	puertaLalo = Model();
	puertaLalo.LoadModel("Models/Eduardo/Puerta.obj");
	Eeve = Model();
	Eeve.LoadModel("Models/Eduardo/Eeve.obj");
	Plant02 = Model();
	Plant02.LoadModel("Models/Eduardo/Plant02.obj");
	Sudowoodo = Model();
	Sudowoodo.LoadModel("Models/Eduardo/Sudowoodo.obj");	
	Zubat = Model();
	Zubat.LoadModel("Models/Eduardo/Zubat.obj");
	Vigilancia = Model();
	Vigilancia.LoadModel("Models/Eduardo/cabina.obj");


	//Partes Psybuck
	psyduckBase = Model();
	psyduckBase.LoadModel("Models/Eduardo/Psyduck_cuerpo.obj");
	psyduckPieI = Model();
	psyduckPieI.LoadModel("Models/Eduardo/Psyduck_pie_izquierdo.obj");
	psyduckPieD = Model();
	psyduckPieD.LoadModel("Models/Eduardo/Psyduck_pie_derecho.obj");

	//Partes carro de golf
	carroGolfBase = Model();
	carroGolfBase.LoadModel("Models/Carro_golf_base.obj");


	carroGolfLlanta = Model();
	carroGolfLlanta.LoadModel("Models/Carro_golf_llanta.obj");

	/*MODELOS CRISTOPHER*/
//--------------------------------------------------------------------------------------*//
	banca = Model();
	banca.LoadModel("Models/Christopher/banca.obj");
	Bandera_C = Model();
	Bandera_C.LoadModel("Models/Christopher/crash.obj");
	Bandera_K = Model();
	Bandera_K.LoadModel("Models/Christopher/kuromi.obj");
	Bandera_H = Model();
	Bandera_H.LoadModel("Models/Christopher/dragon.obj");
	Bandera_P = Model();
	Bandera_P.LoadModel("Models/Christopher/pokemon.obj");
	box = Model();
	box.LoadModel("Models/Christopher/ModelsCrash/box.obj");
	metalUp = Model();
	metalUp.LoadModel("Models/Christopher/ModelsCrash/metalUp.obj");
	fruit = Model();
	fruit.LoadModel("Models/Christopher/ModelsCrash/fruit.obj");
	normal = Model();
	normal.LoadModel("Models/Christopher/ModelsCrash/normal.obj");
	chicken = Model();
	chicken.LoadModel("Models/Christopher/ModelsCrash/chicken.obj");
	car = Model();
	car.LoadModel("Models/Christopher/ModelsCrash/CrashStangFEShape.obj");
	stork = Model();
	stork.LoadModel("Models/Christopher/ModelsCrash/Stork.obj");
	metal = Model();
	metal.LoadModel("Models/Christopher/ModelsCrash/metal.obj");
	vulture = Model();
	vulture.LoadModel("Models/Christopher/ModelsCrash/ave.obj");

	//--------------------------------------------------------------------------------------*//

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_up.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/escenario_dia_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	//--------------------------------------------------------------------------------------*//




									/*LUCES*/
	//--------------------------------------------------------------------------------------*//
		//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);		// a donde apunta

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int pointLightCount2 = 0;
	unsigned int pointLightCount3 = 0;

	//Declaración de primer luz puntual

	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,		// color de la luz // R,G,B
		0.0f, 3.0f,										// ambIntensidad, diffIntensidad
		-5.5f, 22.0f, 0.5f,								// Posicion // X,Y,Z
		0.1f, 0.2f, 0.1f);								// con, lin, exp
	pointLightCount++;

	pointLights2[0] = PointLight(1.0f, 0.0f, 0.0f,		// color de la luz // R,G,B
		0.0f, 3.0f,										// ambIntensidad, diffIntensidad
		10.0f, 4.0f, 1.0f,								// Posicion // X,Y,Z
		0.3f, 0.2f, 0.1f);								// con, lin, exp
	pointLightCount2++;

	unsigned int spotLightCount = 0;
	unsigned int spotLightCount3 = 0;

	//linterna principal
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,		// color de la luz // R,G,B
		0.0f, 2.0f,									// ambIntensidad, diffIntensidad
		0.0f, 0.0f, 0.0f,								// Posicion // X,Y,Z
		0.0f, -1.0f, 0.0f,								// direccion
		1.0f, 0.0f, 0.0f,								// con, lin, exp
		5.0f);											// angulo de apertura del cono de la linterna
	spotLightCount++;

	//Luz del faro para la estatua de kuromi 1
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 5.0f,
		-65.0f, 5.0f, -200.0f,
		-1.0f, 0.5f, 0.0f,
		0.5f, 0.1f, 0.001f,
		50.0f);
	spotLightCount++;

	//Luz del faro para la estatua de kuromi 2
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 5.0f,
		-65.0f, 5.0f, -200.0f,
		-10.0f, 0.5f, 0.0f,
		0.5f, 0.1f, 0.001f,
		50.0f);;
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	movHeliyoffset = 12.0f;
	////Loop mientras no se cierra la ventana

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Movimiento animación
		if (movHeliyoffset > 360.0f)
			movHeliyoffset = 0.0f;

		movHeliy += movHeliyoffset;

		//Fin movimiento animacion

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;

		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(26.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//--------------------------------------------------------------------------------------*//
		//
		//
		//	 
		//
		//--------------------------------------------------------------------------------------*//

										/*MODELOS DANIELA*/
		//--------------------------------------------------------------------------------------*//
		//Skril cuerpo 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 20.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmueve()));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_cuerpo.RenderModel();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		// Skrill ala D
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getangulollantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_AlaD.RenderModel();
		model = modelaux;
		// Skrill ala I
		modelaux = model;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getangulollantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_AlaI.RenderModel();
		model = modelaux;
		// Skrill cola
		modelaux = model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -0.8f));
		model = glm::rotate(model, glm::radians(mainWindow.getangulollantas()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Skrill_cola.RenderModel();
		//--------------------------------------------------------------------------------------*//


											/*MODELOS KARLA*/
		//--------------------------------------------------------------------------------------*//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




		//PopCorn
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, -230.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PopCorn_M.RenderModel();

		glDisable(GL_BLEND);

		//Plataforma
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 100.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Plataforma_M.RenderModel();

		//ESTATUTA KEROPPI
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 100.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Keroppi_estatua_M.RenderModel();

		//LAMPARAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 2.0f, 85.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara_Spot.RenderModel();

		//LAMPARAS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 2.0f, 85.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lampara_Spot.RenderModel();

		//ESTATUA KUROMI
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-122.0f, -1.0f, -180.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Estatua_A.RenderModel();

		//GAZEBO 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -1.0f, -200.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -200.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -1.0f, -150.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//GAZEBO 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -150.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gazebo_M.RenderModel();

		//CARTEL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-122.0f, -1.0f, -125.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cartel_M.RenderModel();

		//Stan de comida
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.0f, -220.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Stand_M.RenderModel();

		//Expendedora
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.0f, -180.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		expendedora.RenderModel();

		//Bocina
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, -150.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bocina_M.RenderModel();

		//Arbusto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arbusto_M.RenderModel();

		//Puerta Principal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PuertaP_M.RenderModel();



		//Señal con mapa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 1.0f, 35.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Senal_M.RenderModel();

		//Bote de Basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -1.0f, -150.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Basura_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -1.0f, -200.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Basura_M.RenderModel();

		//Cercas
		// 
		//Cercas del enfrente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(142.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(204.0f, -2.0f, -275.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-142.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-204.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		//Cercas de la parte trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(186.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-124.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-186.0f, -2.0f, 290.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		//Cercas del lado izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -245.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -180.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -118.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, -55.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 7.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 69.3f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 131.6f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 194.4f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-235.0f, -2.0f, 257.2f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();



		//Cercas del lado derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, -275.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -245.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca02_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -180.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -118.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, -55.5f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 7.0f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 69.3f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 131.6f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 194.4f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(235.0f, -2.0f, 257.2f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cerca01_M.RenderModel();


		//--------------------------------------------------------------------------------------*//


											/*MODELOS EDUARDO*/
		//--------------------------------------------------------------------------------------*//

		/*ANIMALES POKEMON*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, -200.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dodrio.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, 10.0f, -200.0f));
		model = glm::scale(model, glm::vec3(9.0f, 9.0f, 9.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rowlet.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 250.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(124.0f, -1.0f, 220.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-62.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-124.0f, -1.0f, 220.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f, 150.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sudowoodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -1.0f, 250.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Plant02.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 25.0f, -220.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Zubat.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-65.0f, 6.0f, -53.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Eeve.RenderModel();

		/*PERSONAJE PSYDUCK*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -290.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getPosPsyduck()));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckBase.RenderModel();

		modelaux = model;
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieIzq()), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckPieI.RenderModel();
		model = modelaux;

		modelaux = model;
		model = glm::rotate(model, glm::radians(mainWindow.getAngPieDer()), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		psyduckPieD.RenderModel();
		model = modelaux;

		//--------------------------------------------------------------------------------------*//


									/*MODELOS CRISTOPHER*/
		//--------------------------------------------------------------------------------------*//
		//BANCAS DEL DOMO G Y F
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -1.0f, -20.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		//BANCAS DEL DOMO F Y E
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(105.0f, -1.0f, -140.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -125 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		//BANCAS DEL DOMO GRANDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 295 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -1.0f, -95.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 260 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -1.0f, -95.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, 250 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		banca.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 240 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		normal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 4.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 70 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		normal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 9.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		normal.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fruit.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, 20.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fruit.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, 90.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		chicken.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -1.0f, 80.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		stork.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 5.0f, 70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vulture.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f,0.1f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		car.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0, -1.0f, -100.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		metalUp.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 7.0f + 30*abs(sin(glm::radians(movHeliy * 0.1f))), -100.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		metal.RenderModel();

		///////////////////////////////////////////////////////////////////////////////////
											/*DOMOS*/
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//JAULA A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -0.5f, 95.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jaula_A.RenderModel();

		//DOMO GRANDE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		DomoA.RenderModel();
		//BANDERA DRAGON
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, -80.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_H.RenderModel();

		//DOMO G
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, 65.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -55 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Domo.RenderModel();
		//BANDERA CRASH
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -1.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_C.RenderModel();

		//DOMO F
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -1.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Domo.RenderModel();
		//BANDERA KUROMI
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -1.0f, -90.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_K.RenderModel();

		//DOMO E
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, -1.0f, -200.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Domo.RenderModel();
		//BANDERA POKEMON
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, -1.0f, -220.0f));
		model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bandera_P.RenderModel();

		//Caseta de vigilancia
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, -1.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Vigilancia.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, -25.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cartel2_M.RenderModel();


		glDisable(GL_BLEND);


		//--------------------------------------------------------------------------------------*//


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

