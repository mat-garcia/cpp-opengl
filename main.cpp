#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Callback para ajustar a viewport quando a janela é redimensionada
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	glfwInit();

	//define qual opengl estamos usando, no caso o 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//defina o perofile do opengl a ser usado - CORE OU COMPATIBILITY
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//define a criacao de uma janela -----(W , H , name window , fullscreen, ...)
	GLFWwindow* window = glfwCreateWindow(800,800, "CPP OPENGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Falha ao criar GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//diz para o glfw usar a janela no contexto atual
	glfwMakeContextCurrent(window);

	///carrega o opengl com o glad
	gladLoadGL();

	//configura a viewport
	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//RGBA - BUFFER DE GERACAO DE FRAMES
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);


	//matnem a janela aberta ate pedir-mos para que seja fechada.
	while (!glfwWindowShouldClose(window)) {
		//processa os eventos da janela
		glfwPollEvents();

	}

	//fecha a janela
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}