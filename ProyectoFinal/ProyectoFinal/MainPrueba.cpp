//Proyecto Final Computacion grafica
//Semestre 2022-2
//Integrantes:
/*
  Diego Henok Camarena
  Luis Fernando Jaime Farfan
  Karla Rodriguez Jimenez
*/



#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 2.0f, -45.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float rotAlas = 0.0f;
float rotPendulo = 0.0f;
//Mov dino 1
float movPajaro = 0.0f;
int direccionGiro = 0;
int escala = 1;
float rotPajaro = 0.0f;
float posPaloReloj = 0.0f;
bool animPajaro1 = true;
bool animPajaro2 = false;
bool animAlas1 = true;
bool animAlas2 = false;
//Mov dino 2
int paso = 0;
float movPajaroX = 0.0f;//mov en X
float movPajaroY = 75.0f;// mov en Y
float movPajaroZ = 0.0f;//mov en Z
float rotPajaro2 = 0.0f;//Direccion del pajaro
bool animPajaro1_2 = true;//Inicia animacion
bool animPajaro2_2 = false;//detiene animacion
bool bajaPajaro = false;//Detiene mov en X
bool brincaPajaro = false;//Detiene mov en Y
bool desciendePajaro = false;
bool ventanaPajaro = false;
bool establePajaro = false;
bool rotaPajaro = false;
bool rotaPajaro_2 = false;
bool regresaPajaro = false;
bool aterrizaPajaro = false;
bool animAlas1_2 = true;
bool animAlas2_2 = false;
//------------------------//
//Animacion dino 3
float giro = 0.0f;
float giro1 = 0.0f;
float giroRueda = 0.0f;
float inc = 1.0f;
bool animPatas1 = true;
bool animPatas2 = false;
bool acelera = true;
bool desacelera = false;
bool mueveBrazos = false;
//------------------------//
bool animPendulo1 = true;
bool animPendulo2 = false;
float movCamera = 0.0f;
float tiempo;
bool animFuego1 = true;
bool animFuego2 = false;
float rotFuego = 0.0f;
float escFuego = 0.0f;
//------------------------//
//Dino 4
bool cabezaArriba = true;
bool cabezaAbajo = false;
bool cabezaDerecha = false;
bool cabezaizquierda = false;
float rotCabeza = 0.0f; // arr-ab
float rotCabeza2 = 0.0f;// i-d
bool mueveColaD = true;
bool mueveColaI = false;
float rotCola = 0.0f; // i-d
bool bebeAgua = false;
float delay4 = 0.0f;
int pasos4 = 0;

bool abre = false;
bool cierra = false;
float rotPuerta = 0.0f;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -45.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRuedas = 0;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRuedas;
	float rotInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;




void saveFrame(void)
{

	printf("posx %f\n", posX);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	
	KeyFrame[FrameIndex].rotRuedas = rotRuedas;
	

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRuedas = KeyFrame[0].rotRuedas;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	
	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRuedas - KeyFrame[playIndex].rotRuedas) / i_max_steps;

}




int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final: Computacion Grafica e Interaccion Humano-Computadora", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");
	Shader Anim2("Shaders/anim2.vs", "Shaders/anim2.frag");
	//--------------------------------CARGA DE MODELOS--------------------------------//
	//--------------------------------FACHADA--------------------------------//
	Model Fachada((char*)"Models/Modelos/Diego/Fachada_Picapiedras/fachadaPicapiedras.obj");
	Model PuertaFachada((char*)"Models/Modelos/Diego/Fachada_Picapiedras/PuertaFachadaPicapiedras.obj");
	//--------------------------------1. Lavatrastes--------------------------------//
	Model LavatrastesCuerpo((char*)"Models/Modelos/Diego/Lavatrastes_Picapiedras/lavatrastesCuerpo.obj");
	Model LavatrastesPD((char*)"Models/Modelos/Lavatrastes_Picapiedras/lavatrastesPuertaD.obj");
	Model LavatrastesPI((char*)"Models/Modelos/Lavatrastes_Picapiedras/lavatrastesPuertaI.obj");
	//--------------------------------2. Lampara--------------------------------//
	//Model LamparaCabeza((char*)"Models/Modelos/Lampara_Picapiedras/cabeza_lampara.obj");
	Model Lampara((char*)"Models/Modelos/Lampara_Picapiedras/lamparaPicapiedras.obj");
	//--------------------------------3. Reloj--------------------------------//
	Model relojCaja((char*)"Models/Modelos/Diego/Reloj_Picapiedras/relojCaja.obj");
	Model relojPendulo((char*)"Models/Modelos/Reloj_Picapiedras/relojPendulo.obj");
	Model relojCuerpoPajaro((char*)"Models/Modelos/Diego/Reloj_Picapiedras/relojCuerpoPajaro.obj");
	Model relojAlaIzqPajaro((char*)"Models/Modelos/Diego/Reloj_Picapiedras/relojAlaIzqPajaro.obj");
	Model relojAlaDerPajaro((char*)"Models/Modelos/Diego/Reloj_Picapiedras/relojAlaDerPajaro.obj");
	//--------------------------------4. Estufa--------------------------------//
	 Model hornoPicapiedras((char*)"Models/Modelos/Horno_Picapiedras/HornoPicapiedras.obj");
	 Model FuegoHornoPicapiedras((char*)"Models/Modelos/Horno_Picapiedras/FuegoHornoPicapiedras.obj");
	 //--------------------------------5. Cazuela--------------------------------//
	 Model cazuelaPicapiedras((char*)"Models/Modelos/Cazuela_Picapiedras/Cazuela_Picapiedras.obj");
	 Model aguaCazuelaPicapiedras((char*)"Models/Modelos/Diego/Cazuela_Picapiedras/AguaCazuela_Picapiedras.obj");
	 //--------------------------------6. Proyector--------------------------------//
	 Model proyectorPicapiedras((char*)"Models/Modelos/Proyector_Picapiedras/proyectorPicapiedras.obj");
	 Model ruedaProyectorPicapiedras((char*)"Models/Modelos/Diego/Proyector_Picapiedras/RuedaProyectorPicapiedras.obj");
	 Model PajaroProyectorPicapiedras((char*)"Models/Modelos/Diego/Proyector_Picapiedras/CuerpoPajaroProyectorPicapiedras.obj");
	 Model PataDPajaroProyectorPicapiedras((char*)"Models/Modelos/Diego/Proyector_Picapiedras/PiernaDerPajaroProyectorPicapiedras.obj");
	 Model PataIPajaroProyectorPicapiedras((char*)"Models/Modelos/Diego/Proyector_Picapiedras/PiernaIzqPajaroProyectorPicapiedras.obj");
	 Model BrazoDerecho((char*)"Models/Modelos/Diego/Proyector_Picapiedras/brazoDer.obj");
	 Model BrazoIzquierdo((char*)"Models/Modelos/Diego/Proyector_Picapiedras/brazoIzq.obj");
	 
	 //--------------------------------7. Telefono--------------------------------//
	 Model telefonoPicapiedras((char*)"Models/Modelos/Telefono_Picapiedras/telefono_picapiedras.obj");
	 Model PajaroTelefonoPicapiedras((char*)"Models/Modelos/Diego/Telefono_Picapiedras/pajaro_tel_picapiedras.obj");
	 Model AlaDerPajarotelefonoPicapiedras((char*)"Models/Modelos/Diego/Telefono_Picapiedras/alaDer_pajaro_tel.obj");
	 Model AlaIzqPajarotelefonoPicapiedras((char*)"Models/Modelos/Diego/Telefono_Picapiedras/alaIzq_pajaro_tel.obj");
	 //----------Dino 4------------//
	 Model cuerpoDino4((char*)"Models/Modelos/Diego/dino4/cuerpoDino.obj");
	 Model cabezaDino4((char*)"Models/Modelos/Diego/dino4/cabezaDino.obj");
	 Model colaDino4((char*)"Models/Modelos/Diego/dino4/colaDino.obj");
	 
	 //--------------------------------Carro Picapiedra--------------------------------//
	 Model Carro((char*)"Models/Modelos/Karla/carro/carro.obj");
	 /*Model Carro((char*)"Models/Modelos/Karla/carro/carroSR.obj");
	 Model Ruedas((char*)"Models/Modelos/Karla/carro/Ruedas.obj");*/


	//Objeto traslucido
	Model objTras("Models/Cubo/Cube01.obj");

	// Build and compile our shader program

	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRuedas = 0;
		KeyFrame[i].rotInc = 0;
	}

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/front.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/right.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp

		//Carga de modelo
		//--------------------------FACHADA--------------------------//
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(20.0f, 20.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Fachada.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		PuertaFachada.Draw(lightingShader);
		//--------------------------LAVATRASTES--------------------------//
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model,glm::vec3(posX,posY,posZ));
		model = glm::translate(model, glm::vec3(210.0f, -30.0f, 400.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LavatrastesCuerpo.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-210.0f, -30.0f, 85.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LavatrastesPD.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-210.0f, -30.0f, 85.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		LavatrastesPI.Draw(lightingShader);
		//--------------------------LAMPARA--------------------------//
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(120, 0, 40));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-160.0f, -30.0f, -170.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.Draw(lightingShader);
		//--------------------------RELOJ--------------------------//
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(50.0f, 460.0f, -300.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		relojCaja.Draw(lightingShader);	
		
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(rotPendulo), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		relojPendulo.Draw(lightingShader);
		//--------------------------ESTUFA--------------------------//
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(-60.0f, -20.0f, 100.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		hornoPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(rotFuego), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f, escFuego, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		FuegoHornoPicapiedras.Draw(lightingShader);
		//--------------------------Cazuela--------------------------//
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(500.0f, -32.0f, -100.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		aguaCazuelaPicapiedras.Draw(lightingShader);
		//--------------------------Proyector--------------------------//		
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(470.0f, -20.0f, -100.0f));
		model = glm::rotate(model, glm::radians(giroRueda), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.7f, 1.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		ruedaProyectorPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(470.0f, 65.0f, -90.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		PajaroProyectorPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(giro), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		PataDPajaroProyectorPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(giro1), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		PataIPajaroProyectorPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(470.0f, 65.0f, -152.0f)); 
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -62.0f));
		model = glm::rotate(model, glm::radians(giro1), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		BrazoDerecho.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giro1), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		BrazoIzquierdo.Draw(lightingShader);
		//--------------------------Telefono--------------------------//
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(300.0f, 180.0f, -550.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		telefonoPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(movPajaroX, movPajaroY, movPajaroZ));
		model = glm::rotate(model, glm::radians(rotPajaro2), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		PajaroTelefonoPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(3*rotAlas), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		AlaDerPajarotelefonoPicapiedras.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(3*rotAlas), glm::vec3(1.0f, 0.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		AlaIzqPajarotelefonoPicapiedras.Draw(lightingShader);
		// Dino //
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(500.0f, 32.0f, -430.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		cuerpoDino4.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(500.0f, 32.0f, -435.0f));
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotCabeza2), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		cabezaDino4.Draw(lightingShader);
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(model, glm::vec3(500.0f, 32.0f, -380.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -180.0f));
		model = glm::rotate(model, glm::radians(rotCola), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		colaDino4.Draw(lightingShader);
		// Carro Picapiedra //
		view = camera.GetViewMatrix();
		model = glm::mat4(4);
		model = glm::translate(model, glm::vec3(-95.0f, 22.0f, -45.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Carro.Draw(lightingShader);
		
		/*view = camera.GetViewMatrix();
		model = glm::mat4(4);
		model = glm::translate(model, glm::vec3(-95.0f, 22.0f, -45.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ruedas.Draw(lightingShader);*/


		//-----------------ANIMACIONES-------------// 		

		Anim2.Use();
		model = glm::mat4(4);
		tiempo = glfwGetTime();
		modelLoc = glGetUniformLocation(Anim2.Program, "model");
		viewLoc = glGetUniformLocation(Anim2.Program, "view");
		projLoc = glGetUniformLocation(Anim2.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//tmp = model = glm::translate(model, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::translate(model, glm::vec3(90.0f, 470.0f, -300.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, (escala) * 1.0f));
		model = glm::translate(model, glm::vec3(movPajaro, 0, posPaloReloj));
		model = glm::rotate(model, glm::radians(rotPajaro), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim2.Program, "time"), tiempo);
		relojCuerpoPajaro.Draw(Anim2);
		
		view = camera.GetViewMatrix();
		model = glm::rotate(model, glm::radians(rotAlas), glm::vec3(0.0f, 0.0f, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		relojAlaIzqPajaro.Draw(lightingShader);

		view = camera.GetViewMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		relojAlaDerPajaro.Draw(lightingShader);
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void animacion()
{

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;

				rotRuedas += KeyFrame[playIndex].rotInc;

				i_curr_steps++;
			}

		}
	}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		
		movCamera = 0.01f;//Manda una velocidad de 0.01 a la camara automatica

	}

	if (keys[GLFW_KEY_2])
	{
			posX += 1.0f;
			
	}	

	if (keys[GLFW_KEY_3])
	{			
			posX -= 1.0f;
		
	}

	if (keys[GLFW_KEY_4])
	{
		posY += 1.0f;

	}
	if (keys[GLFW_KEY_5])
	{
		posY -= 1.0f;

	}

	//Mov Pendulo pajaro Reloj
	if (animPendulo1) //animación hacia la derecha
	{
		rotPendulo += 0.2;
	}
	if (animPendulo2)// animación hacia la izquierda
	{
		rotPendulo -= 0.2;
	}
	if (rotPendulo >= 45) { //se mueve hasta 45° hacia la derecha
		animPendulo1 = false;//Cuando llega a 45, termina anim1 (derecha)
		animPendulo2 = true;//Cuando llega a 45, inicia anim2 (izquierda)
	}
	if (rotPendulo <= -45) { //se mueve hasta 45° a la izquierda
		animPendulo1 = true;//Cuando llega a 45 hacia la izquierda, inicia anim1 
		animPendulo2 = false;//Cuando llega a 45 hacia la izquierda, termina anim2 
	}

	//Mov pajaro Reloj
	if (animPajaro1) //animación hacia la derecha
	{
		movPajaro += 0.005;
	}
	if (animPajaro2)// animación hacia la izquierda
	{
		if (direccionGiro == 0) {
			posPaloReloj = 0.0f;
			rotPajaro += 0.15;
			escala = 1;
		}
		if (direccionGiro == 1) {
			posPaloReloj = 520.0f;
			escala = -1;
			rotPajaro += 0.2;
		}
	}
	if (movPajaro >= 8) { 
		animPajaro1 = false;
		animPajaro2 = true;
		
	}
	if (rotPajaro >= 360 || rotPajaro <= -360 ) {
		animPajaro1 = true;
		animPajaro2 = false;
		movPajaro = 0.0;
		rotPajaro = 0.0;
		direccionGiro = (rand()) % 2;
	}

	//----------------Animacion fuego---------------------//
	rotFuego += 0.2;
	if (animFuego1) //animación hacia la derecha
	{
		escFuego += 0.004;
	}
	if (animFuego2)// animación hacia la izquierda
	{
		escFuego -= 0.004;
	}
	if (escFuego >= 1.5) { //se mueve hasta 45° hacia la derecha
		animFuego1 = false;//Cuando llega a 45, termina anim1 (derecha)
		animFuego2 = true;//Cuando llega a 45, inicia anim2 (izquierda)
	}
	if (escFuego <= 0.5) { //se mueve hasta 45° a la izquierda
		animFuego1 = true;//Cuando llega a 45 hacia la izquierda, inicia anim1 
		animFuego2 = false;//Cuando llega a 45 hacia la izquierda, termina anim2 
	}

	//Mov alas pajaro Reloj
	if (animAlas1) //animación hacia la derecha
	{
		rotAlas += 0.4;
	}
	if (animAlas2)// animación hacia la izquierda
	{
		rotAlas -= 0.4;
	}
	if (rotAlas >= 30) { //se mueve hasta 45° hacia la derecha
		animAlas1 = false;//Cuando llega a 45, termina anim1 (derecha)
		animAlas2 = true;//Cuando llega a 45, inicia anim2 (izquierda)
	}
	if (rotAlas <= -30) { //se mueve hasta 45° a la izquierda
		animAlas1 = true;//Cuando llega a 45 hacia la izquierda, inicia anim1 
		animAlas2 = false;//Cuando llega a 45 hacia la izquierda, termina anim2 
	
	}
	//Inicia animacion compleja pajaro2

	if (animPajaro1_2 ) {
		bajaPajaro = true;		
		paso = 1;
	}
	if (bajaPajaro ) {
		movPajaroZ += 0.18 * 3;
		movPajaroY -= 0.1 * 3;
		
	}
	if (brincaPajaro ) {
		movPajaroZ += 0.35 * 2;
		movPajaroY += 0.15 * 2;
		
	}
	if (desciendePajaro ) {
		movPajaroZ += 0.2;
		movPajaroY -= 0.15;
		
	}
	if (ventanaPajaro ) {
		movPajaroZ += 0.09;
		movPajaroY += 0.1;
	}
	if (rotaPajaro) {
		rotPajaro2 += 0.5;
	}
	if (rotaPajaro_2) {
		rotPajaro2 -= 0.5;
	}
	if (regresaPajaro) {
		movPajaroZ -= 4;
		movPajaroY += 1;
	}
	if (aterrizaPajaro) {
		movPajaroZ -= 4.3;
		movPajaroY -= 1.5;
	}
	if (movPajaroY <= -130 && paso ==1) {
		bajaPajaro = false;
		brincaPajaro = true;
		animPajaro1_2 = false;
		paso = 2;
	}
	if (movPajaroY >= 95 && paso ==2 ) {
		brincaPajaro = false;
		bajaPajaro = false;
		desciendePajaro = true;
		animPajaro1_2 = false;
		paso = 3;
	}
	
	if (movPajaroY <= 47 && paso == 3) {
		desciendePajaro = false;
		rotaPajaro = true;
		paso = 4;
	}
	if (rotPajaro2 >= 180 && paso == 4) {
		rotaPajaro = false;
		regresaPajaro = true;
		paso = 5;
	}
	if (movPajaroY >= 200 && paso == 5) {
		regresaPajaro = false;
		aterrizaPajaro = true;
		paso = 6;
	}
	if (movPajaroY <= 75 && paso == 6) {
		aterrizaPajaro = false;
		rotaPajaro_2 = true;
		paso = 7;
	}
	if (rotPajaro2 <= 0 && paso == 7) {
		
		rotaPajaro_2 = false;
		animPajaro1_2 = true;
	}
	//Termina animacion compleja pajaro2
	
	//Inicia animacion compleja pajaro3
	if (acelera) {
		giroRueda -= (0.2)*(inc)*(inc);
		if (animPatas1) //animación hacia la derecha
		{
			giro += (0.1) * (inc) * (inc);
			giro1 -= (0.2) * (inc) * (inc);

		}
		if (animPatas2)// animación hacia la izquierda
		{
			giro -= (0.1) * (inc) * (inc);
			giro1 += (0.2) * (inc) * (inc);

		}
		if (giro >= 30) { //se mueve hasta 45° hacia la derecha
			animPatas1 = false;//Cuando llega a 45, termina anim1 (derecha)
			animPatas2 = true;//Cuando llega a 45, inicia anim2 (izquierda)
		}
		if (giro <= -30) { //se mueve hasta 45° a la izquierda
			animPatas1 = true;//Cuando llega a 45 hacia la izquierda, inicia anim1 
			animPatas2 = false;//Cuando llega a 45 hacia la izquierda, termina anim2 
		}
		inc += 0.001;
	}
	if (desacelera) {
		giroRueda -= (0.2) * (inc) * (inc);
		inc -= 0.001;
		if (animPatas1) //animación hacia la derecha
		{
			giro += (0.1) * (inc) * (inc);
			giro1 -= (0.2) * (inc) * (inc);

		}
		if (animPatas2)// animación hacia la izquierda
		{
			giro -= (0.1) * (inc) * (inc);
			giro1 += (0.2) * (inc) * (inc);

		}
		if (giro >= 30) { //se mueve hasta 45° hacia la derecha
			animPatas1 = false;//Cuando llega a 45, termina anim1 (derecha)
			animPatas2 = true;//Cuando llega a 45, inicia anim2 (izquierda)
		}
		if (giro <= -30) { //se mueve hasta 45° a la izquierda
			animPatas1 = true;//Cuando llega a 45 hacia la izquierda, inicia anim1 
			animPatas2 = false;//Cuando llega a 45 hacia la izquierda, termina anim2 
		}
	}
	if (inc >= 5) {
		acelera = false;
		desacelera = true;
	}
	if (inc <= 0) {
		desacelera = false;
		acelera = true;
	}

	if (keys[GLFW_KEY_O])
	{
		cierra = false;
		abre = true;
	}

	// Anim dino 4 //
	if (cabezaArriba) {	
		rotCabeza += 0.2;
	}
	if (cabezaAbajo) {
		rotCabeza -= 0.2;
	}
	if (cabezaizquierda) {
		rotCabeza2 -= 0.3;
	}
	if (cabezaDerecha) {
		rotCabeza2 += 0.3;
	}
	if (bebeAgua) {
		delay4 += 0.01;
	}
	if (rotCabeza >= 30 ) {
		cabezaArriba = false;
		cabezaAbajo = true;
		pasos4 = 1;
	}
	if (rotCabeza <= -50 && pasos4 == 1) {
		bebeAgua = true;
		cabezaAbajo = false;
		pasos4 = 2;
	}
	if (delay4 >= 20 && pasos4 == 2) {
		bebeAgua = false;
		cabezaArriba = true;
		delay4 = 0;
		pasos4 = 3;
	}
	if (rotCabeza >= 0 && pasos4 == 3) {
		cabezaArriba = false;
		bebeAgua = true;
		pasos4 = 4;
	}
	if (delay4 >= 20 && pasos4 == 4) {
		bebeAgua = false;
		cabezaDerecha = true;
		delay4 = 0;
		pasos4 = 5;
	}
	if (rotCabeza2 >= 30 && pasos4 == 5) {
		cabezaDerecha = false;
		cabezaizquierda = true;
		pasos4 = 6;
	}
	if (rotCabeza2 <= -30 && pasos4 == 6) {
		cabezaizquierda = false;
		bebeAgua = true;
		pasos4 = 7;
	}
	if (delay4 >= 20 && pasos4 == 7) {
		cabezaArriba = true;
		pasos4 = 0;
	}
	if (mueveColaD) {
		rotCola += 0.3;
	}
	if (mueveColaI) {
		rotCola -= 0.3;
	}
	if (rotCola >= 30) {
		mueveColaD = false;
		mueveColaI = true;
	}
	if (rotCola <= -30) {
		mueveColaD = true;
		mueveColaI = false;
	}
	if (keys[GLFW_KEY_C])
	{
		abre = false;
		cierra = true;
	}

	if (abre) {
		if (rotPuerta >= -90.0f)
			rotPuerta -= 0.05f;
	}

	if (cierra) {
		if (rotPuerta <= 0.0f)
			rotPuerta += 0.05f;
	}
	

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, 50*deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, 50 * deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, 50 * deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, 50 * deltaTime);
	}
}