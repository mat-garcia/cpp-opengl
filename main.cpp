#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"

// Vertex Shader Source Code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"  FragColor = color;\n"
"}\n\0";

// Callback para ajustar a viewport quando a janela � redimensionada
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Inicializa o GLFW
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    // Define qual vers�o do OpenGL estamos usando, no caso 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Define o perfil do OpenGL a ser usado - CORE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria a janela
    GLFWwindow* window = glfwCreateWindow(800, 800, "CPP OPENGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Torna o contexto da janela atual
    glfwMakeContextCurrent(window);

    // Carrega todas as fun��es do OpenGL com o GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Configura a viewport
    glViewport(0, 0, 800, 800);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Define os vertices do tri�ngulo
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };

    // Cria e compila o Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Verifica por erros no Vertex Shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar o Vertex Shader:\n" << infoLog << std::endl;
    }

    // Cria e compila o Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Verifica por erros no Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar o Fragment Shader:\n" << infoLog << std::endl;
    }

    // Linka os Shaders em um programa
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Verifica por erros no Linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro ao linkar o Shader Program:\n" << infoLog << std::endl;
    }

    // Deleta os shaders pois eles j� foram linkados ao programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Cria VAO e VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind o VAO
    glBindVertexArray(VAO);

    // Bind e define o VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Define os atributos do Vertex Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind o VBO e VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        // Processa eventos do ImGui
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Criando a interface do usu�rio do ImGui
        ImGui::Begin("Controles");
        static ImVec4 triangleColor = ImVec4(0.8f, 0.3f, 0.02f, 1.0f);
        ImGui::ColorEdit3("Cor do Tri�ngulo", (float*)&triangleColor);
        ImGui::End();

        // Renderizando
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glUniform4f(glGetUniformLocation(shaderProgram, "color"), triangleColor.x, triangleColor.y, triangleColor.z, triangleColor.w);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Renderiza o ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Troca os buffers
        glfwSwapBuffers(window);
    }

    // Deleta os recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Encerra o ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Fecha a janela
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}