






// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Simple_shaiders.h"
#include "Simple_camera.h"
#include "Model.h"

#include <iostream>
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Window dimensions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

const GLuint SCR_WIDTH = 1200, SCR_HEIGHT = 800;

//camera parametres
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = SCR_WIDTH / 2.0f;
GLfloat lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool flashlight = false;
bool carlights = false;
// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width12, height12;
	glfwGetFramebufferSize(window, &width12, &height12);
	glViewport(0, 0, width12, height12);


	

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

	// load models
	// -----------
	//-Model ourModel("models/20christmas-tree/20christmas-tree.3DS");
	//Model ourModel("models/backpack/backpack.obj");
	Model ourModel("models/Pine/Pine.3ds");

	//Model ourModel("models/Pine4/realistictrees2.3ds");
	Model Terrain("models/SnowTerrain/SnowTerrain.obj");
	Model Star("models/Star/20facestar.obj");
	Model Gift1("models/gift3/Box christmas present N221218.3ds");

	Model Hummer("models/hummer/truck.3ds");

	Model Backpack("models/rukzak/Backpack.3ds");

	Model Table("models/table/Table Trestle N080519.3ds");
	Model Chair("models/chair/Chair IKEA Siaro N031120.3ds");
	//Shader TestShaider("VShaider_coordinates.txt", "FShader_textures.txt");
	// Set up vertex data (and buffer(s)) and attribute pointers
	// We add a new set of vertices to form a second triangle (a total of 6 vertices); the vertex attribute configuration remains the same (still one 3-float position vector per vertex)
	
	 // build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("6.multiple_lights.txt", "6.multiple_lights_frag.txt");
	Shader lightCubeShader("6.light_cube_vert.txt", "6.light_cube_frag.txt");
	
	

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.5f,  0.55f,  2.5f),
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

	glm::vec3 treepositions[] = {

		glm::vec3(0.0f, -0.05f, 0.0f),
		glm::vec3(-3.0f, 0.0f, -5.0f),
		glm::vec3(-2.0f, 0.0f, -3.0f),
		glm::vec3(1.0f, 0.0f, -4.0f),
		glm::vec3(4.0f, 0.0f, 0.0f),
		glm::vec3(5.0f, 0.0f, 3.0f),
		glm::vec3(2.0f, 0.0f, 6.0f),
		glm::vec3(-3.0f, 0.0f, 5.0f),
		glm::vec3(-4.5f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 7.0f)


	};

	float  tresssize[] = {
		1.0, 0.8, 0.6, 0.7, 0.85, 0.5, 0.9, 0.64, 0.72, 0.24
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.05f,  3.1f, 0.0f)
	};
	GLuint VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	GLuint lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
	   // -----
		processInput(window);
		//key_callback(window);
		// render
	   // ------
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		//ourShader.use();
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		// view/projection transformations
		/*glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);*/


		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.8f, 0.8f, 0.8f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1000.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.7);
		lightingShader.setFloat("pointLights[3].quadratic", 1.8);
		// spotLight
		if (flashlight) {
			lightingShader.setVec3("spotLight[0].position", camera.Position);
			lightingShader.setVec3("spotLight[0].direction", camera.Front);
			lightingShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight[0].constant", 1.0f);
			lightingShader.setFloat("spotLight[0].linear", 0.09);
			lightingShader.setFloat("spotLight[0].quadratic", 0.032);
			lightingShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		else {
			lightingShader.setVec3("spotLight[0].position", camera.Position);
			lightingShader.setVec3("spotLight[0].direction", camera.Front);
			lightingShader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight[0].constant", 100.0f);
			lightingShader.setFloat("spotLight[0].linear", 0.09);
			lightingShader.setFloat("spotLight[0].quadratic", 0.032);
			lightingShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		if (carlights) {
			//flashlights
	//-2.96945 0.71335 1.20027
	//-3.40898 0.706157 1.89029
	//-0.836223 0.71846 3.2851
	//-0.457139 0.808149 2.61684

			lightingShader.setVec3("spotLight[1].position", glm::vec3(-2.96945f, 0.71335f, 1.20027f));
			lightingShader.setVec3("spotLight[1].direction", glm::vec3(-0.457139f, 0.808149f, 2.61684f) - glm::vec3(-2.96945f, 0.71335f, 1.20027f));
			lightingShader.setVec3("spotLight[1].ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight[1].diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight[1].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight[1].constant", 1.0f);
			lightingShader.setFloat("spotLight[1].linear", 0.09);
			lightingShader.setFloat("spotLight[1].quadratic", 0.032);
			lightingShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));
			

			lightingShader.setVec3("spotLight[2].position", glm::vec3(-3.40898f, 0.706157f, 1.89029f));
			lightingShader.setVec3("spotLight[2].direction", glm::vec3(-0.836223f, 0.71846f, 3.2851f) - glm::vec3(-3.40898f, 0.706157f, 1.89029f));
			lightingShader.setVec3("spotLight[2].ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight[2].diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight[2].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight[2].constant", 1.0f);
			lightingShader.setFloat("spotLight[2].linear", 0.09);
			lightingShader.setFloat("spotLight[2].quadratic", 0.032);
			lightingShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		else {
			lightingShader.setVec3("spotLight[1].position", glm::vec3(-2.96945f, 0.71335f, 1.20027f));
			lightingShader.setVec3("spotLight[1].direction", glm::vec3(-0.457139f, 0.808149f, 2.61684f) - glm::vec3(-2.96945f, 0.71335f, 1.20027f));
			lightingShader.setVec3("spotLight[1].ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight[1].diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight[1].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight[1].constant", 100.0f);
			lightingShader.setFloat("spotLight[1].linear", 0.09);
			lightingShader.setFloat("spotLight[1].quadratic", 0.032);
			lightingShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));


			lightingShader.setVec3("spotLight[2].position", glm::vec3(-3.40898f, 0.706157f, 1.89029f));
			lightingShader.setVec3("spotLight[2].direction", glm::vec3(-0.836223f, 0.71846f, 3.2851f) - glm::vec3(-3.40898f, 0.706157f, 1.89029f));
			lightingShader.setVec3("spotLight[2].ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight[2].diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight[2].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight[2].constant", 100.0f);
			lightingShader.setFloat("spotLight[2].linear", 0.09);
			lightingShader.setFloat("spotLight[2].quadratic", 0.032);
			lightingShader.setFloat("spotLight[2].cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight[2].outerCutOff", glm::cos(glm::radians(15.0f)));
		}
		

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);


		//rendering tree
		for (int i = 0; i < 10; i++) {
			glm::mat4 Treemodel = glm::mat4(1.0);
			Treemodel = glm::translate(Treemodel, treepositions[i]); // translate it down so it's at the center of the scene
			//Treemodel = glm::rotate(Treemodel, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
			//Treemodel = glm::rotate(Treemodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
			Treemodel = glm::scale(Treemodel, glm::vec3(tresssize[i]));	// it's a bit too big for our scene, so scale it down
			lightingShader.setMat4("model", Treemodel);
			ourModel.Draw(lightingShader);
		}
		


		//rendering terrain
		glm::mat4 Terrainmodel = glm::mat4(1.0f);
		Terrainmodel = glm::translate(Terrainmodel, glm::vec3(0.0f, -0.3f, 0.0f));
		//Terrainmodel = glm::rotate(Terrainmodel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		lightingShader.setMat4("model", Terrainmodel);
		Terrain.Draw(lightingShader);
		

		//rendering star on the tree
		glm::mat4 Starmodel = glm::mat4(1.0f);
		Starmodel = glm::translate(Starmodel, glm::vec3(0.04f, 3.05f, 0.0f)); // translate it down so it's at the center of the scene
		Starmodel = glm::rotate(Starmodel, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		//Treemodel = glm::rotate(Treemodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		Starmodel = glm::scale(Starmodel, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", Starmodel);
		Star.Draw(lightingShader);

		//rendering gifts
		glm::mat4 Giftmodel = glm::mat4(1.0f);
		Giftmodel = glm::translate(Giftmodel, glm::vec3(0.4f, 0.01f, 0.0f)); // translate it down so it's at the center of the scene
		//Giftmodel = glm::rotate(Giftmodel, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		//Treemodel = glm::rotate(Treemodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		Giftmodel = glm::scale(Giftmodel, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", Giftmodel);
		Gift1.Draw(lightingShader);

		//rendering hummer
		glm::mat4 Carmodel = glm::mat4(1.0f);
		Carmodel = glm::translate(Carmodel, glm::vec3(-5.0f, 0.0f, 0.5f));
		Carmodel = glm::rotate(Carmodel, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		Carmodel = glm::rotate(Carmodel, glm::radians(60.0f), glm::vec3(0.0, 0.0, 1.0));
		Carmodel = glm::rotate(Carmodel, glm::radians(4.0f), glm::vec3(1.0, 0.0, 0.0));
		Carmodel = glm::scale(Carmodel, glm::vec3(0.001f, 0.001f, 0.001f));
		lightingShader.setMat4("model", Carmodel);
		Hummer.Draw(lightingShader);

		//rendering backpack		
		glm::mat4 Backpackmodel = glm::mat4(1.0f);
		Backpackmodel = glm::translate(Backpackmodel, glm::vec3(0.5f, 0.03f, 3.35f)); // translate it down so it's at the center of the scene
		Backpackmodel = glm::rotate(Backpackmodel, glm::radians(-10.0f), glm::vec3(1.0, 0.0, 0.0));
		//Treemodel = glm::rotate(Treemodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		Backpackmodel = glm::scale(Backpackmodel, glm::vec3(0.0002f, 0.0002f, 0.0002f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", Backpackmodel);
		Backpack.Draw(lightingShader);

		 //rendering table	
		/*-3.1543 0.738803 -3.37536
-3.79332 0.777978 -2.87564
-3.20599 0.780862 -3.22534
////
-3.93065 0.737064 -3.05234
-3.41046 0.836388 -3.16502
-3.47636 0.774649 -2.87956
-4.00798 0.504885 -3.83931
-4.36947 0.48382 -1.69315

*/
		glm::mat4 Tablemodel = glm::mat4(1.0f);
		Tablemodel = glm::translate(Tablemodel, glm::vec3(-3.5f, 0.05f, -3.0f));
		//Tablemodel = glm::rotate(Tablemodel, glm::radians(-10.0f), glm::vec3(1.0, 0.0, 0.0));
		//Treemodel = glm::rotate(Treemodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		Tablemodel = glm::scale(Tablemodel, glm::vec3(0.008f, 0.008f, 0.008f));	
		lightingShader.setMat4("model", Tablemodel);
		Table.Draw(lightingShader);

		//rendering chairs
		glm::mat4 Chairmodel = glm::mat4(1.0f);
		Chairmodel = glm::translate(Chairmodel, glm::vec3(-4.0f, 0.05f, -3.8f)); 
		Chairmodel = glm::rotate(Chairmodel, glm::radians(200.0f), glm::vec3(0.0, 1.0, 0.0));
		//Chairmodel = glm::rotate(Chairmodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		Chairmodel = glm::scale(Chairmodel, glm::vec3(0.001f, 0.001f, 0.001f));
		lightingShader.setMat4("model", Chairmodel);
		Chair.Draw(lightingShader);
		Chairmodel = glm::mat4(1.0f);
		Chairmodel = glm::translate(Chairmodel, glm::vec3(-4.3f, 0.03f, -2.0f));
		Chairmodel = glm::rotate(Chairmodel, glm::radians(-15.0f), glm::vec3(0.0, 1.0, 0.0));
		//Chairmodel = glm::rotate(Chairmodel, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		Chairmodel = glm::scale(Chairmodel, glm::vec3(0.001f, 0.001f, 0.001f));
		lightingShader.setMat4("model", Chairmodel);
		Chair.Draw(lightingShader);


		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		// render containers
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 1; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object(s)
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			if (i == 0)
				continue;
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			//-3.26042 0.71498 1.97557
			//-2.83158 0.728036 1.25836
			if (i == 1) {
				if (!carlights) {
					continue;
				}
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-3.36144f, 0.737122f, 1.92532f));
				model = glm::scale(model, glm::vec3(0.1f)); 
				model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
				lightCubeShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			if (i == 2) {
				if (!carlights) {
					continue;
				}
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(-2.87342f, 0.708605f, 1.19694f));
				model = glm::scale(model, glm::vec3(0.1f));
				model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
				lightCubeShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			if (i == 3) {
				model = glm::scale(model, glm::vec3(0.1f));
			}
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	//glDeleteVertexArrays(1, VAO);
	//glDeleteBuffers(1, VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (flashlight) {
			flashlight = false;
		}
		else {
			flashlight = true;
		}
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (carlights) {
			carlights = false;
		}
		else {
			carlights = true;
		}
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
	/*if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		moverx -= 0.1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		moverx += 0.1;
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		movery += 0.1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		movery -= 0.1;*/
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	/*if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		if (flashlight) {
			flashlight = false;
		}
		else {
			flashlight = true;
		}
	}*/
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}