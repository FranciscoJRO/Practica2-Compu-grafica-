//Practica 2 
//Francisco Javier Reynoso Ortega 
//Fecha de entrega 10/08/2025
// Numero de cuenta : 421056697

#include <iostream>
#include <vector>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

// Paleta: 0=fondo (no se dibuja), 1=negro, 2=rojo, 3=blanco, 4=azul
static const std::array<std::array<float, 3>, 5> PALETTE = { {
    {0.95f, 0.95f, 0.95f}, // 0 (solo clear)
    {0.00f, 0.00f, 0.00f}, // 1 negro
    {0.85f, 0.15f, 0.18f}, // 2 rojo
    {1.00f, 1.00f, 1.00f}, // 3 blanco
    {0.08f, 0.25f, 0.75f}  // 4 azul
} };

// ===== Sprite Spider‑Man (28x28). 0=fondo, 1=negro, 2=rojo, 3=blanco, 4=azul =====
static const int COLS = 32, ROWS = 32;
static const int S[ROWS][COLS] = {
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},//Aqui empieza spiderman
    {3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3}, //13-19
    {3,3,3,3,3,3,3,3,3,3,3,1,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,1,2,2,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,1,2,2,2,2,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,2,1,1,2,2,2,2,2,2,2,1,1,2,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,1,3,3,1,2,2,2,2,2,1,3,3,1,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,3,3,3,3,1,2,2,2,1,3,3,3,3,1,3,3,3,3,3,3,3,3,3},//punto
    {3,3,3,3,3,3,3,3,1,3,3,3,3,3,1,2,1,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,1,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,1,3,3,3,3,3,3,1,3,3,3,3,3,3,1,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,1,1,3,3,3,1,2,1,3,3,3,1,1,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,1,2,1,1,1,2,2,2,1,1,1,2,1,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,1,2,2,2,2,2,2,2,2,2,1,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,1,1,2,2,2,2,2,1,1,3,3,3,3,3,3,3,3,3,3,3,3},//hasta aqui vamos bien
	{3,3,3,3,3,3,3,3,3,3,1,3,3,1,1,1,1,1,3,3,1,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
    {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
};




// Crea dos triángulos (6 vértices) para una celda (cx,cy)
static void pushQuad(std::vector<float>& V, int cx, int cy, float cellW, float cellH, const std::array<float, 3>& c)
{
    float x0 = -1.0f + cx * cellW;
    float y0 = 1.0f - cy * cellH;
    float x1 = x0 + cellW;
    float y1 = y0 - cellH;

    auto add = [&](float x, float y) {
        V.push_back(x); V.push_back(y); V.push_back(0.0f);      // pos
        V.push_back(c[0]); V.push_back(c[1]); V.push_back(c[2]); // color
        };
    // Triángulo 1
    add(x0, y0); add(x1, y0); add(x1, y1);
    // Triángulo 2
    add(x0, y0); add(x1, y1); add(x0, y1);
}

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 2 Reynoso Ortega Francisco Javier", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, resize);

    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialise GLEW\n";
        return EXIT_FAILURE;
    }

    std::cout << "> Version:  " << glGetString(GL_VERSION) << '\n';
    std::cout << "> Vendor:   " << glGetString(GL_VENDOR) << '\n';
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << '\n';

    // Ajuste inicial del viewport cuadrado
    int fbw, fbh; glfwGetFramebufferSize(window, &fbw, &fbh);
    resize(window, fbw, fbh);

    Shader shader("Shader/core.vs", "Shader/core.frag");

    // Generar vértices del sprite
    std::vector<float> vertices; vertices.reserve(ROWS * COLS * 6 * 6);
    float cellW = 2.0f / COLS;
    float cellH = 2.0f / ROWS;

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            int code = S[r][c];
            if (code == 0) continue; // fondo no se dibuja
            pushQuad(vertices, c, r, cellW, cellH, PALETTE[code]);
        }
    }

    // VAO/VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // (Opcional) rejilla
    bool drawGrid = false;
    GLuint gridVAO = 0, gridVBO = 0;
    std::vector<float> grid;
    if (drawGrid) {
        for (int c = 0; c <= COLS; ++c) {
            float x = -1.0f + c * cellW;
            grid.insert(grid.end(), { x,  1.0f, 0.0f, 0.8f,0.8f,0.8f });
            grid.insert(grid.end(), { x, -1.0f, 0.0f, 0.8f,0.8f,0.8f });
        }
        for (int r = 0; r <= ROWS; ++r) {
            float y = 1.0f - r * cellH;
            grid.insert(grid.end(), { -1.0f, y, 0.0f, 0.8f,0.8f,0.8f });
            grid.insert(grid.end(), { 1.0f, y, 0.0f, 0.8f,0.8f,0.8f });
        }
        glGenVertexArrays(1, &gridVAO);
        glGenBuffers(1, &gridVBO);
        glBindVertexArray(gridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
        glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(float), grid.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    // Loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(PALETTE[0][0], PALETTE[0][1], PALETTE[0][2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();

        if (drawGrid) {
            glBindVertexArray(gridVAO);
            glLineWidth(1.0f);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(grid.size() / 6));
        }

        glBindVertexArray(VAO);
        GLsizei count = static_cast<GLsizei>(vertices.size() / 6); // pos3+col3
        glDrawArrays(GL_TRIANGLES, 0, count);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
    // Viewport cuadrado y centrado (para mantener pixeles perfectos)
    int size = (width < height) ? width : height;
    int x = (width - size) / 2;
    int y = (height - size) / 2;
    glViewport(x, y, size, size);
}
