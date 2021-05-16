// C++ 
#include <iostream>
#include <thread> 
#include <chrono>
#include <stack>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

// OpenCV 
//#include <opencv2\opencv.hpp>

// OpenGL Extension Wrangler
#include <GL/glew.h> 
#include <GL/wglew.h> //WGLEW = Windows GL Extension Wrangler (change for different platform) 

// GLFW toolkit
#include <GLFW/glfw3.h>

// OpenGL math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <glm/glad.h>
// OpenGL textures
#include <gli/gli.hpp>

//project includes...
#include "globals.h"
#include "init.h"
#include "callbacks.h"
#include "glerror.h" // Check for GL errors

#include "mesh.h"
#include "mesh_init.h"
#include "texture.h"

//mesh
mesh mesh_floor;
mesh mesh_brick;
mesh mesh_brick2;
mesh ceiling;
mesh mesh_undestroyableBrick;
mesh mesh_treasure;
mesh mesh_gun;
std::vector<mesh> meshes_magazine;
int circle_segments = 1'000'000;
mesh mesh_cube;

struct monsterStr{
	mesh monsterMesh;
	int monsterX;
	int monsterY;
};

monsterStr monster;

bool running = false;

std::stack<glm::mat4> stack_modelview;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


Avatar  player = { 10.0f, 0.0f, 0.0f, 0.0f };
Avatar  shot = { 0.0, 0.0, 0.0 };

int brick = 1;

int gun = 1;                          // witch gun is selected 1.. for destroy, 2.. for create
int bullet = 5;							//bullet count 
bool fireBullet = false;				//is fired?
bool nightVisionActive = false;
bool bulletTime = false;

GLfloat color_blue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat color_white[] = { 1.0, 1.0, 1.0, 1.0 };

unsigned int ID;

constexpr auto BLOCK_SIZE = 10.0f;

cv::Mat mapa = cv::Mat(25, 25, CV_8U);   // maze map



// secure access to map
uchar getmap(cv::Mat& map, int x, int y)
{
	if (x < 0)
	{
		std::cerr << "Map: X too small: " << x << std::endl;
		x = 0;
	}

	if (x >= map.cols)
	{
		std::cerr << "Map: X too big: " << x << std::endl;
		x = map.cols - 1;
	}

	if (y < 0)
	{
		std::cerr << "Map: Y too small: " << y << std::endl;
		y = 0;
	}

	if (y >= map.rows)
	{
		std::cerr << "Map: Y too big: " << y << std::endl;
		y = map.rows - 1;
	}

	//at(row,col)!!!
	return map.at<uchar>(y, x);
}

// forward declarations
static void local_init(void);
static void local_init_mesh(void);
static unsigned char collision(Avatar* avatar);
static void setVec3(const std::string &name, float x, float y, float z);
static void setVec3(const std::string &name, const glm::vec3 &value);
static void use();
static void checkCompileErrors(GLuint shader, std::string type);
static void shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath );
void nightVision(void);

void printMap() {
	for (int j = 0; j < mapa.rows; j++) {
		for (int i = 0; i < mapa.cols; i++) {
			std::cout << getmap(mapa, i, j);
		}
		std::cout << std::endl;
	}
}

//---------------------------------------------------------------------
// Random map gen
//---------------------------------------------------------------------
void genLabyrinth(void) {
	int i, j;
	cv::Point2i start, end;

	// C++ random numbers
	std::random_device r; // Seed with a real random value, if available
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_height(1, mapa.rows - 2); // uniform distribution between int..int
	std::uniform_int_distribution<int> uniform_width(1, mapa.cols - 2);
	std::uniform_int_distribution<int> uniform_block(0, 15);

	//inner maze 
	for (j = 0; j < mapa.rows; j++) {

		for (i = 0; i < mapa.cols; i++) {

			// první a poslední øádek/sloupec je nerozbitný
			if (j == 0 || j == mapa.rows - 1 || i == 0 || i == mapa.cols - 1) {
				mapa.at<uchar>(cv::Point(i, j)) = '*';
				continue;
			}

			// tady bude pøíšera
			if (j == 1 && i == 1) {
				mapa.at<uchar>(cv::Point(i, j)) = 'P';
				monster.monsterX = 1;
				monster.monsterY = 1;
				continue;
			}

			switch (uniform_block(e1))
			{
			case 0:
				mapa.at<uchar>(cv::Point(i, j)) = '1'; // høáè mùže tuto kostku rozbít
				break;
			default:
				mapa.at<uchar>(cv::Point(i, j)) = '.';
				break;
			}
		}
	}

	//gen start inside maze (excluding walls)
	do {
		start.x = uniform_width(e1);
		start.y = uniform_height(e1);
	} while (getmap(mapa, start.x, start.y) == '1'); //check wall

	//gen end different from start, inside maze (excluding outer walls) 
	do {
		end.x = uniform_width(e1);
		end.y = uniform_height(e1);
	} while (start == end); //check overlap
	mapa.at<uchar>(cv::Point(end.x, end.y)) = 'e';

	std::cout << "Start: " << start << std::endl;
	std::cout << "End: " << end << std::endl;

	printMap();

	//set player position
	player.posX = (-start.x * BLOCK_SIZE) - BLOCK_SIZE / 2.0f;
	player.posY = (-start.y * BLOCK_SIZE) - BLOCK_SIZE / 2.0f;
}

//---------------------------------------------------------------------
// pøidání/odebrání kostky 
//---------------------------------------------------------------------
void changeCube(Avatar* avatar, bool addOnly)
{

	int kvadrantX, kvadrantY;
	int playerX, playerY;


	kvadrantX = abs(ceilf(avatar->posX / BLOCK_SIZE));
	kvadrantY = abs(ceilf(avatar->posY / BLOCK_SIZE));

	playerX = abs(ceilf(player.posX / BLOCK_SIZE));
	playerY = abs(ceilf(player.posY / BLOCK_SIZE));

	std::cout << kvadrantX << "  " << kvadrantY << ";" << std::endl;
	std::cout << playerX << "  " << playerY << ";" << std::endl;

	if (kvadrantX == playerX && abs(kvadrantY - playerY) < 4) {
		if (kvadrantY < playerY) {
			if (gun == 1 && !addOnly && bullet < 5) {
				mapa.at<uchar>(cv::Point(kvadrantX, kvadrantY)) = '.';
				bullet = bullet + 1;
			}
			else if (gun == 2 && bullet > 0) {
				mapa.at<uchar>(cv::Point(kvadrantX, kvadrantY + 1)) = (char)(brick + '0');
				bullet = bullet - 1;
			}
		}
		else {
			if (gun == 1 && !addOnly && bullet < 5) {
				mapa.at<uchar>(cv::Point(kvadrantX, kvadrantY)) = '.';
				bullet = bullet + 1;
			}
			else if (gun == 2 && bullet > 0) {
				mapa.at<uchar>(cv::Point(kvadrantX, kvadrantY - 1)) = (char)(brick + '0');
				bullet = bullet - 1;
			}
		}
	}

	else if (kvadrantY == playerY && abs(kvadrantX - playerX) < 4) {
		if (kvadrantX < playerX) {
			if (gun == 1 && !addOnly && bullet < 5) {
				mapa.at<uchar>(cv::Point(kvadrantX, kvadrantY)) = '.';
				bullet = bullet + 1;
			}
			else if (gun == 2 && bullet > 0) {
				mapa.at<uchar>(cv::Point(kvadrantX + 1, kvadrantY)) = (char)(brick + '0');
				bullet = bullet - 1;
			}
		}
		else {
			if (gun == 1 && !addOnly && bullet < 5) {
				mapa.at<uchar>(cv::Point(kvadrantX, kvadrantY)) = '.';
				bullet = bullet + 1;
			}
			else if (gun == 2 && bullet > 0) {
				mapa.at<uchar>(cv::Point(kvadrantX - 1, kvadrantY)) = (char)(brick + '0');
				bullet = bullet - 1;
			}
		}
	}

	printMap();
}

//---------------------------------------------------------------------
// position & draw bullet 
//---------------------------------------------------------------------
void shotMoveView(Avatar* shot)
{
	glPushMatrix();

	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0, 4.9, 0.0);
	glTranslatef(-shot->posX, 0.0, -shot->posY);
	glRotatef(shot->move_h_angle, 0.0f, 1.0f, 0.0f);

	glScalef(0.3f, 0.3f, 0.3f);
	mesh_draw(mesh_gun);     //strelba zmensenou pistoli :-D (to nikdo nevidi, tak co...)

	glPopMatrix();
}

//---------------------------------------------------------------------
// Avatar view and orientation
//---------------------------------------------------------------------
void avatarMoveView(Avatar* avatar)
{
	constexpr auto MOVE_SCENE_DOWN = -5.0;            // make x-y plane visible;
	constexpr auto SCENE_SHIFT = -1.0;                // move scene for rotation;

	glTranslatef(0.0, MOVE_SCENE_DOWN, 0.0);        // move down - make z-plane visible
	glRotatef(90.0, 1.0, 0.0, 0.0);                 // change axis
	glTranslatef(0.0, SCENE_SHIFT, 0.0);            // step back for rotation
	glRotatef(avatar->cam_h_angle, 0.0, 0.0, 1.0);      // rotate player
	glTranslatef(avatar->posX, avatar->posY, 0.0);  // move player
}

//---------------------------------------------------------------------
// switch Gun
//---------------------------------------------------------------------
void switchGun()
{
	if (gun == 1) {
		gun = 2;
	}
	else
	{
		gun = 1;
	}
}

void changeBrick() {
	brick++;
	if (brick > 2)
		brick = 1;
}

//---------------------------------------------------------------------
// Collision detection with walls
//---------------------------------------------------------------------	
static unsigned char collision(Avatar* avatar)
{
	int kvadrantX, kvadrantY;

	//avatar quadrant
	kvadrantX = ceilf(avatar->posX / BLOCK_SIZE);
	kvadrantY = ceilf(avatar->posY / BLOCK_SIZE);

	return getmap(mapa, abs(kvadrantX), abs(kvadrantY));
}

//---------------------------------------------------------------------
// 3D draw
//---------------------------------------------------------------------
void DrawAll(void)
{
	int i, j;
	static double old_time = 0.0;
	static double old_frame_time = 0.0;
	static int frame_cnt = 0;
	double current_time;

	current_time = glfwGetTime();

	// move bullet
	if ((fireBullet == true))
	{
		while (true) {
			old_time = current_time;

			Avatar a = avatarMoveForward(shot);
			char cub = collision(&a);

			if (cub == '1' || cub == '2') {
				fireBullet = false;
				changeCube(&a, false);
				break;
			}

			else if (cub == '*') {
				fireBullet = false;
				changeCube(&a, true);
				break;
			}

			else if (cub == 'e') {
				fireBullet = false;
				genLabyrinth();
				return;
				break;
			}
			else {
				shot = a;
			}

			if (!fireBullet)
			{
				globals.camera = &player;
				bulletTime = false;
			}
		}
	}


	//write FPS
	if (current_time - old_frame_time > 1.0)
	{
		old_frame_time = current_time;
		std::cout << "FPS: " << frame_cnt << "\r";
		frame_cnt = 0;
	}
	frame_cnt++;

	//light on/off
	if (nightVisionActive == true)
	{
		glEnable(GL_LIGHT2);
	}
	else {
		glDisable(GL_LIGHT2);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	avatarMoveView(globals.camera);

	//bullet fired
	if (fireBullet == true) {
		shotMoveView(&shot);
	}

	//draw gun
	mesh_gun = gen_mesh_gun(gun);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);
	if (!bulletTime) mesh_draw(mesh_gun);
	mesh_draw(meshes_magazine[bullet]);
	glPopMatrix();

	//draw ground
	mesh_draw(mesh_floor);
	// draw inner walls and ceiling ...
	for (j = 0; j < mapa.rows; j++)
	{
		for (i = 0; i < mapa.cols; i++)
		{
			switch (getmap(mapa, i, j))
			{
			case '.': // volno
				break;
			case '*': // nerozbitné
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, 0.0f);
				mesh_draw(mesh_undestroyableBrick);
				glPopMatrix();

				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, -10.0f);
				mesh_draw(mesh_undestroyableBrick);
				glPopMatrix();
				break;
			case 'X': // hráè
				break;
			case '1':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, 0.0f);
				mesh_draw(mesh_brick);
				glPopMatrix();
				break;
			case 'P':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, 0.0f);
				mesh_draw(monster.monsterMesh);
				glPopMatrix();
				break;
			case 'e':
				/*glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, -10.0f);
				mesh_draw_arrays(mesh_cube);
				glPopMatrix();*/
				break;
			default:
				break;
			}

			// strop
			/*glPushMatrix();
			glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, -20.0f);
			mesh_draw(ceiling);
			glPopMatrix();*/
		}
	}


	for (j = 0; j < mapa.rows; j++)
	{
		for (i = 0; i < mapa.cols; i++)
		{
			glPushMatrix();
			glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, -20.0f);
			//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color_blue);
			mesh_draw_arrays(mesh_cube);
			glPopMatrix();
			//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color_white);
		}
	}
	
	// vykreslení prùhledných kostièek
	for (j = 0; j < mapa.rows; j++)
	{
		for (i = 0; i < mapa.cols; i++)
		{
			switch (getmap(mapa, i, j))
			{
			case '2':
				glPushMatrix();
				glTranslatef(i * BLOCK_SIZE, j * BLOCK_SIZE, 0.0f);
				mesh_draw(mesh_brick2);
				glPopMatrix();
				break;
			default:
				break;
			}
		}
	}

}

void monsterMove(void) {

	char c;
	int oldX;
	int oldY;

	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		int playerX = abs(ceilf(player.posX / BLOCK_SIZE));
		int playerY = abs(ceilf(player.posY / BLOCK_SIZE));
		oldX = monster.monsterX;
		oldY = monster.monsterY;

		if (playerX > monster.monsterX) {
			c = getmap(mapa, monster.monsterX + 1, monster.monsterY);
			monster.monsterX++;
		}
		else if (playerX < monster.monsterX) {
			c = getmap(mapa, monster.monsterX - 1, monster.monsterY);
			monster.monsterX--;
		}

		else if (playerY > monster.monsterY) {
			c = getmap(mapa, monster.monsterX, monster.monsterY + 1);
			monster.monsterY++;
		}

		else if (playerY < monster.monsterY) {
			c = getmap(mapa, monster.monsterX, monster.monsterY - 1);
			monster.monsterY--;
		}

		if (c != '2') {
			mapa.at<uchar>(cv::Point(oldX, oldY)) = c;
			mapa.at<uchar>(cv::Point(monster.monsterX, monster.monsterY)) = 'P';
			//printMap();
		}
		else {
			monster.monsterX = oldX;
			monster.monsterY = oldY;
		}

		// restart pokud je pøíšerka na stejné pozici jako hráè
		if (playerX == monster.monsterX && playerY == monster.monsterY) {
			genLabyrinth();
		}
	}

}

void nightVision(void) {
	int timeNight = 5;
	while (running) {
		if (nightVisionActive) {
			if (timeNight <= 0) {
				nightVisionActive = false;
				timeNight = 5;
			}
			else {
				timeNight--;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

//---------------------------------------------------------------------
// Mouse pressed?
//---------------------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			if (!fireBullet) {
				fireBullet = true;
				shot.move_h_angle = shot.cam_h_angle = player.move_h_angle;
				shot.posX = player.posX;
				shot.posY = player.posY;
			}
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		/*if (action == GLFW_PRESS) {
			if (bullet != 5) bullet = 5;
		}*/
	}
}

//---------------------------------------------------------------------
// Mouse moved?
//---------------------------------------------------------------------
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static int first = 1;
	static double old_x;
	if (first) {
		old_x = xpos;
		first = 0;
	}
	else {
		*globals.camera = avatarRotate(*globals.camera, glm::radians(-xpos + old_x), glm::radians(0.0f), glm::radians(0.0f));
	}

	//glfwSetCursorPos(globals.window, 0, 0);
}

//---------------------------------------------------------------------
// MAIN
//---------------------------------------------------------------------
int main(int argc, char** argv)
{
	// Call all initialization.
	init_glfw();
	init_glew();
	gl_print_info();

	local_init();
	local_init_mesh();

	running = true;
	std::thread MonsterThread(monsterMove);
	std::thread NightThread(nightVision);

	//
	//shader("2.2.basic_lighting.vs", "2.2.basic_lighting.fs", nullptr);

	// Run until exit is requested.
	while (!glfwWindowShouldClose(globals.window))
	{
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Use ModelView matrix for following trasformations (translate,rotate,scale)
		glMatrixMode(GL_MODELVIEW);
		// Clear all tranformations
		glLoadIdentity();

		DrawAll();

		/*glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(45.0f, (float)800 / (float)600, 0.1f, 100.0f);
		setMat4("model", projection);
		use();
		setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		setVec3("lightPos", lightPos);*/

		// Swap front and back buffers 
		// Calls glFlush() inside
		glfwSwapBuffers(globals.window);

		// Check for errors in current frame
		gl_check_error();

		// Poll for and process events
		glfwPollEvents();
	}

	running = false;
	MonsterThread.join();
	finalize(EXIT_SUCCESS);
}

void fullScreen() {
	const GLFWvidmode* pWindowMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	globals.fullScreen = !globals.fullScreen;

	if (globals.fullScreen) {
		glfwSetWindowMonitor(globals.window, glfwGetPrimaryMonitor(), 0, 0, pWindowMode->width, pWindowMode->height, GLFW_DONT_CARE);
	}
	else {
		glfwSetWindowMonitor(globals.window, NULL, 0, 0, 800, 400, GLFW_DONT_CARE);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Avatar a = player;

	if ((action == GLFW_PRESS) || (action == GLFW_REPEAT))
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE: //fallthrough
			break;
		case GLFW_KEY_I:
			fullScreen();
			break;
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_UP: //fallthrough
		case GLFW_KEY_W:
			a = avatarMoveForward(a);
			break;
		case GLFW_KEY_DOWN: //fallthrough
		case GLFW_KEY_S:
			a = avatarMoveBackward(a);
			break;
		case GLFW_KEY_E:
			switchGun();
			break;
		case GLFW_KEY_LEFT: //fallthrough
		case GLFW_KEY_A:
			a = avatarMoveLeft(a);
			break;
		case GLFW_KEY_RIGHT: //fallthrough
		case GLFW_KEY_D:
			a = avatarMoveRight(a);
			break;
		case GLFW_KEY_PAGE_UP: //fallthrough
		case GLFW_KEY_R:
			a = avatarMoveUp(a);
			break;
		case GLFW_KEY_PAGE_DOWN: //fallthrough
		case GLFW_KEY_F:
			changeBrick();
			break;
		case GLFW_KEY_KP_ADD:
			a.movement_speed += 1.0f;
			std::cout << "Speed: " << a.movement_speed << std::endl;
			break;
		case GLFW_KEY_KP_SUBTRACT:
			if (a.movement_speed > 1.0f)
				a.movement_speed -= 1.0f;
			std::cout << "Speed: " << a.movement_speed << std::endl;
			break;
		case GLFW_KEY_L:
			if (bullet > 0) {
				nightVisionActive = !nightVisionActive;
				std::cout << "Light: " << nightVision << std::endl;
				bullet--;
			}
			break;
		case GLFW_KEY_B:
			if (fireBullet)	bulletTime = !bulletTime;
			if (bulletTime)
				globals.camera = &shot;
			else
				globals.camera = &player;
			break;
		default:
			break;
		}

		if (collision(&a) == '.') {
			player = a;
		}
	}
}

static void local_init_mesh(void)
{
	genLabyrinth();	// create labyrinth


	// aby to bylo prùhledné
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mesh_floor = gen_mesh_floor(mapa, BLOCK_SIZE);
	mesh_brick = gen_mesh_brick(BLOCK_SIZE);
	ceiling = gen_mesh_ceiling(BLOCK_SIZE);

	mesh_treasure = mesh_brick;
	mesh_treasure.tex_id = textureInit("resources/treasure.bmp", false, false);

	mesh_undestroyableBrick = gen_mesh_brick(BLOCK_SIZE);
	mesh_undestroyableBrick.tex_id = textureInit("resources/wall.bmp", false, false);


	mesh_brick2 = mesh_brick;
	mesh_brick2.tex_id = textureInit("resources/blending_transparent_window.png", false, true);

	monster.monsterMesh = mesh_brick;
	monster.monsterMesh.tex_id = textureInit("resources/monster.png", false, false);

	mesh_cube = gen_mesh_ceiling(BLOCK_SIZE);
	if (!loadOBJ(mesh_cube, "resources/model.obj"))
	{
		std::cerr << "loadOBJ failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	gen_mesh_magazines(meshes_magazine);
	{
		GLuint t = textureInit("resources/bullet.bmp", false, false);
		for (auto &i : meshes_magazine)
		{
			i.tex_id = t;
			i.texture_used = true;
		}
	}

	player.mouse_sensitivity = 10.0f;
	player.movement_speed = 2.0f;
	player.lock_cam_move_angles = true;

	globals.camera = &player;

	shot.movement_speed = 0.5f;
	shot.mouse_sensitivity = 10.0f;
	shot.lock_cam_move_angles = false;
}

static void local_init(void)
{
	//
	// OpenGL settings
	// 

	glClearColor(0.2f, 0.2f, 0.4f, 0.0f);               // color-buffer clear colour
	glEnable(GL_CULL_FACE);  // disable draw of back face
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);                        // set Gouraud shading

	glEnable(GL_DEPTH_TEST);                        // enable depth test  
	glPolygonMode(GL_FRONT, GL_FILL);       // enable polygon fill
	ShowCursor(false);

	GLfloat light_position[] = { 0.0f, 0.0f, 45.0f, 1.0f };
	GLfloat light_direction[] = { 0.0, 0.0, -1.0 };
	GLfloat light_color[] = { 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_POSITION, light_position);			// light setup 
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 1.5f);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_color);

	GLfloat light1_ambient[] = { 1.2, 0.2, 1.2, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 0.0, 45.0, 1.0 };
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	//glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	//glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 50.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.5);

	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT1);
}

void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void fbsize_callback(GLFWwindow* window, int width, int height)
{
	// check for limit case (prevent division by 0)
	if (height == 0)
		height = 1;

	float ratio = (float)width / (float)height;

	globals.width = width;
	globals.height = height;

	glMatrixMode(GL_PROJECTION);				// set projection matrix for following transformations

	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(45.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		ratio,			     // Aspect Ratio. Depends on the size of your window.
		0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		20000.0f              // Far clipping plane. Keep as little as possible.
	);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glViewport(0, 0, width, height);			// set visible area

	std::cout << "WxH: " << width << " " << height << std::endl;
}

void use()
{
	glUseProgram(ID);
}

void setMat4(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void setVec3(const std::string &name, const glm::vec3 &value)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void setVec3(const std::string &name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);

}
// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}