/*************************************
 * PROGRAMMER: Kononov Nickolay
 * GROUP: 131
 * LAST UPDATE: 27.05.2019
 *************************************/
#define _CRT_SECURE_NO_WARNINGS // fopen unsafe function
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")


#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


const char *vertex_program =
	"#version 330 core\n"
	"uniform mat4 proj;\n"
	"uniform vec3 pos;\n"
	"uniform vec4 rot;\n"
	"out vec2 vert_tex_coords;\n"
	"vec3 qmulv(vec4 a, vec3 b) {\n"
	" return b + 2.0 * cross(a.xyz, cross(a.xyz, b) + a.w * b);\n"
	"}\n"
	"vec4 faces[6] = vec4[6](\n"
	" vec4(-sqrt(0.5), 0.0, 0.0, sqrt(0.5)),\n"
	" vec4(sqrt(0.5), 0.0, 0.0, sqrt(0.5)),\n"
	" vec4(0.0, 0.0, 0.0, 1.0),\n"
	" vec4(0.0, -sqrt(0.5), 0.0, sqrt(0.5)),\n"
	" vec4(0.0, 1.0, 0.0, 0.0),\n"
	" vec4(0.0, sqrt(0.5), 0.0, sqrt(0.5))\n"
	");\n"
	"vec2 vertices[6] = vec2[6](\n"
	" vec2(-1.0, 1.0),\n"
	" vec2(-1.0, -1.0),\n"
	" vec2(1.0, 1.0),\n"
	" vec2(-1.0, -1.0),\n"
	" vec2(1.0, -1.0),\n"
	" vec2(1.0, 1.0)\n"
	");\n"
	"void main() {\n"
	" int fi = gl_VertexID / 6;\n"
	" int vi = gl_VertexID % 6;\n"
	" vec3 p = qmulv(rot, pos + qmulv(faces[fi], vec3(vertices[vi], -1.0) * 0.5));\n"
	" vert_tex_coords = vertices[vi];\n"
	" gl_Position = proj * vec4(p, 1.0);\n"
	"}\n";

const char *fragment_program =
	"#version 330 core\n"
	"uniform sampler2D tex;\n"
	"in vec2 vert_tex_coords;\n"
	"out vec4 frag_color;\n"
	"void main() {\n"
	" frag_color = vec4(texture(tex, vec2(0.5, -0.5) * (1 + vert_tex_coords)).rgb, 1.0);\n"
	"}\n";

#define PI 3.14159265358979323846

static GLchar gllog[10000];

struct glfw_globals {
	bool forward;
	bool left;
	bool backward;
	bool right;
	double mousesens[2];
	double lastcursorpos[2];
	float pos[3];
	float viewangles[2];
};

static void keycb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	struct glfw_globals *globals = glfwGetWindowUserPointer(window);
	bool *k = NULL;
	switch (key) 
	{
	case GLFW_KEY_ESCAPE:
		exit(0);
		break;
	case GLFW_KEY_W:
		k = &globals->forward;
		break;
	case GLFW_KEY_A:
		k = &globals->left;
		break;
	case GLFW_KEY_S:
		k = &globals->backward;
		break;
	case GLFW_KEY_D:
		k = &globals->right;
		break;
	}
	if (k != NULL) 
	{
		if (action == GLFW_PRESS)
			*k = true;
		if (action == GLFW_RELEASE)
			*k = false;
	}
}

static void cursorposcb(GLFWwindow* window, double xpos, double ypos)
{
	struct glfw_globals *globals = glfwGetWindowUserPointer(window);
	double cursorpos[2] = {xpos, ypos};
	for (int i = 0; i < 2; i++) 
	{
		globals->viewangles[i] += (globals->lastcursorpos[i] - cursorpos[i]) * globals->mousesens[i];
		globals->lastcursorpos[i] = cursorpos[i];
	}
}

static void qmul2(float *x, float *y, float *z)
{
	z[0] += (x[3] * y[0]) + (x[0] * y[3]) + (x[1] * y[2]) - (x[2] * y[1]);
	z[1] += (x[3] * y[1]) - (x[0] * y[2]) + (x[1] * y[3]) + (x[2] * y[0]);
	z[2] += (x[3] * y[2]) + (x[0] * y[1]) - (x[1] * y[0]) + (x[2] * y[3]);
	z[3] += (x[3] * y[3]) - (x[0] * y[0]) - (x[1] * y[1]) - (x[2] * y[2]);
}

int main(int argc, char **argv)
{
	GLenum err;
	if (!glfwInit())
		assert(0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "h", NULL, NULL);
	if (window == NULL)
		assert(0);
	struct glfw_globals globals = {0};
	globals.mousesens[0] = 0.003;
	globals.mousesens[1] = 0.003;
	glfwSetWindowUserPointer(window, &globals);
	glfwSetKeyCallback(window, keycb);
	glfwSetCursorPosCallback(window, cursorposcb);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();
	glfwSwapInterval(1);
	puts(glGetString(GL_RENDERER));
	puts(glGetString(GL_VERSION));
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_program, NULL);
	glCompileShader(vs);
	glGetShaderInfoLog(vs, sizeof(gllog), NULL, gllog);
	puts(gllog);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_program, NULL);
	glCompileShader(fs);
	glGetShaderInfoLog(fs, sizeof(gllog), NULL, gllog);
	puts(gllog);
	GLuint s = glCreateProgram();
	glAttachShader(s, fs);
	glAttachShader(s, vs);
	glLinkProgram(s);
	glGetProgramInfoLog(s, sizeof(gllog), NULL, gllog);
	puts(gllog);
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	/* reading number of blocks */
	FILE *F;
	if ((F = fopen("textures.txt", "rt")) == NULL)
		assert(0);

	int c = 0, n = 0;
	while ((c = fgetc(F)) != '\n')
		n = n * 10 + c - '0';
	GLuint *textures = malloc(n * n * n * sizeof(*textures));
	if (!textures)
		assert(0);
	glGenTextures(n*n*n, textures);
	for (int i = 0; i < n*n*n; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		char texname[999] = { 0 };
		char *x = texname;
		while ((c = fgetc(F)) != '\n')
			*(x++) = c;
		FILE *f = fopen(texname, "rb");
		if (!f) 
		{
			printf("fopen: %s\n", texname);
			assert(0);
		}
		fseek(f, 0, SEEK_END);
		long len = ftell(f);
		fseek(f, 0, SEEK_SET);
		void *data = malloc(len);
		if (!data)
			assert(0);
		fread(data, len, 1, f);
		fclose(f);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)sqrt(len/3), (GLsizei)sqrt(len/3), 
														0, GL_RGB, GL_UNSIGNED_BYTE, data);
		free(data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("start: glGetError: %d\n", err);
	float speed = 0.05f;
	float pos[4] = {0.0f, 0.0f, 1.0f, 0.0f};
	while (!glfwWindowShouldClose(window))
	{
		float yrot[4] = {0.0f, sinf(globals.viewangles[0]), 0.0f, cosf(globals.viewangles[0])};
		float xrot[4] = {sinf(globals.viewangles[1]), 0.0f, 0.0f, cosf(globals.viewangles[1])};
		float rot[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		qmul2(xrot, yrot, rot);
		float lvel[4] = 
		{
			globals.left * speed - globals.right * speed,
			0.0f,
			globals.forward * speed - globals.backward * speed,
			0.0f,
		};
		float rot2[4] = {-rot[0], -rot[1], -rot[2], rot[3]};
		float tmp[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		qmul2(rot2, lvel, tmp);
		qmul2(tmp, rot, pos);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.67f, 0.84f, 0.9f, 1.0f);
		glClearDepth(1.0f);
		glBindVertexArray(vao);
		while ((err = glGetError()) != GL_NO_ERROR)
			printf("glBindVertexArray: glGetError: %d\n", err);
		glUseProgram(s);
		while ((err = glGetError()) != GL_NO_ERROR)
			printf("glUseProgram: glGetError: %d\n", err);
		double yfov = 45.0 / 180.0 * PI;
		double half_yfov_tan = tan(yfov / 2.0);
		double znear = 1.0;
		double zfar = 1000.0;
		float proj[16] = 
		{
			(double) height / width / half_yfov_tan, 0.0f, 0.0f, 0.0f,
			0.0f, -1.0 / half_yfov_tan, 0.0f, 0.0f,
			0.0f, 0.0f, (znear + zfar) / (znear - zfar), 2.0 * (znear * zfar) / (znear - zfar),
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		glUniformMatrix4fv(glGetUniformLocation(s, "proj"), 1, GL_FALSE, proj);
		glUniform4fv(glGetUniformLocation(s, "rot"), 1, rot);
		for (int i = 0; i < n*n*n; i++) 
		{
			int z = i / n / n;
			int y = i / n % n;
			int x = i % n;
			float half_n = (float) (n - 1) / 2;
			float pos2[3] = 
			{
				pos[0] - half_n + x,
				pos[1] - half_n + y,
				pos[2] - half_n + z
			};
			glUniform3fv(glGetUniformLocation(s, "pos"), 1, pos2);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			while ((err = glGetError()) != GL_NO_ERROR)
				printf("glDrawArrays: glGetError: %d\n", err);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
		while ((err = glGetError()) != GL_NO_ERROR)
			printf("loop: glGetError: %d\n", err);
	}
	return 0;
}
