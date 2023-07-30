#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(600, 600, "opengl-template", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	// sync with monitor refresh rate
	glfwSwapInterval(1);

	/* Initialize GLEW */
	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	// gpu driver version
	GLCall(std::cout << glGetString(GL_VERSION) << std::endl);

	{
		// triangle info
		float positions[] = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		// index buffer
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// enable blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// vertex array object
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		// create a buffer
		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		// send index buffer (ibo = index buffer object)
		IndexBuffer ib(indices, 6);

		// shader code
		Shader shader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
		shader.Bind();

		// uniform
		// shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		// texture
		// Texture texture("res/textures/backdrop.png");
		// texture.Bind();
		// shader.SetUniform1i("u_Texture", 0);

		// unbind everything
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		// create renderer
		Renderer renderer;


		// for color animation
		float r = 0.0f;
		float increment = 0.025f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			// bind shader
			shader.Bind();
			// shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);


			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}