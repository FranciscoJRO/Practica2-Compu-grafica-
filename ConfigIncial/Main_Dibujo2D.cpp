// ============================================================================
// Practica 2
// Alumno: Francisco Javier Reynoso Ortega
// No. de cuenta: 421056697
// Fecha de entrega: 20/08/2025
//
// Título: Render de sprite tipo “pixel-art” con OpenGL (GLFW + GLEW)
// Descripción:
//   - Dibuja un sprite 32x32 usando únicamente primitivas 2D (triángulos).
//   - Cada celda del arreglo S representa un “pixel” del sprite y se
//     rasteriza como un quad (2 triángulos).
//   - La paleta PALETTE mapea códigos {0..4} a colores RGB.
//   - Se mantiene un viewport cuadrado centrado para que las celdas
//     conserven proporción 1:1 independientemente del tamaño de ventana.
//
// Dependencias:
//   - GLEW, GLFW
//   - Clase Shader (Shader.h) con método Use() y carga de core.vs/core.frag
//
// Controles / Flags:
//   - drawGrid: si es true dibuja una rejilla guía (líneas GL_LINES).
//
// Notas:
//   - 0 en el sprite = fondo; no se genera geometría (se “ahorra” vértices).
//   - Coordenadas NDC: mapeamos cada celda a [-1,1]x[-1,1].
// ============================================================================

#include <iostream>
#include <vector>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

// ---- Prototipos -------------------------------------------------------------
/** Callback de cambio de tamaño. Mantiene un viewport cuadrado centrado. */
void resize(GLFWwindow* window, int width, int height);

// Tamaño inicial de ventana (no fuerza proporción; el viewport lo corrige)
const GLint WIDTH = 800, HEIGHT = 600;

// -----------------------------------------------------------------------------
// Paleta: índice -> color RGB
// 0 = fondo (solo clear; no se dibuja)
// 1 = negro, 2 = rojo, 3 = blanco, 4 = azul
// -----------------------------------------------------------------------------
static const std::array<std::array<float, 3>, 5> PALETTE = { {
    {0.95f, 0.95f, 0.95f}, // 0 (clear color de fondo)
    {0.00f, 0.00f, 0.00f}, // 1 negro
    {0.85f, 0.15f, 0.18f}, // 2 rojo
    {1.00f, 1.00f, 1.00f}, // 3 blanco
    {0.08f, 0.25f, 0.75f}  // 4 azul
} };

// -----------------------------------------------------------------------------
// Sprite (32x32). Cada entero es un índice a PALETTE.
// 0=fondo, 1=negro, 2=rojo, 3=blanco, 4=azul
// -----------------------------------------------------------------------------
static const int COLS = 32, ROWS = 32;
static const int S[ROWS][COLS] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//Aqui empieza spiderman
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, //13-19
    {0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,2,1,1,2,2,2,2,2,2,2,1,1,2,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,3,3,1,2,2,2,2,2,1,3,3,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,3,3,3,3,1,2,2,2,1,3,3,3,3,1,0,0,0,0,0,0,0,0,0},//punto
    {0,0,0,0,0,0,0,0,1,3,3,3,3,3,1,2,1,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,1,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,3,3,3,3,3,3,1,3,3,3,3,3,3,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,3,3,3,1,2,1,3,3,3,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,2,1,1,1,2,2,2,1,1,1,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,1,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,2,2,1,1,1,1,1,2,2,1,0,0,0,0,0,0,0,0,0,0,0},//hasta aqui vamos bien
    {0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,4,4,1,4,2,1,1,1,2,4,1,4,4,1,0,0,0,0,0,0,0,0,0},//Empezamos con el color azul 
    {0,0,0,0,0,0,0,1,2,2,4,1,4,1,2,1,2,1,4,1,4,2,2,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,1,4,4,4,4,4,4,4,1,0,1,1,0,0,0,0,0,0,0,0,0},//terminamos las manos 
    {0,0,0,0,0,0,0,0,0,0,1,4,4,4,4,4,4,4,4,4,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,4,4,4,1,1,1,4,4,4,1,0,0,0,0,0,0,0,0,0,0,0},//Terminaamos el cuerpo 
    {0,0,0,0,0,0,0,0,0,1,1,2,2,1,0,0,0,1,2,2,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,2,2,2,1,0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},//TErminamos la imagen 
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

// -----------------------------------------------------------------------------
// pushQuad: agrega a 'V' la geometría (2 triángulos) de una celda (cx, cy)
// Parámetros:
//   V      -> buffer de vértices (x,y,z,r,g,b) intercalados
//   cx, cy -> índice de columna/fila dentro del sprite
//   cellW/H-> tamaño de celda en NDC
//   c      -> color RGB a usar
// -----------------------------------------------------------------------------
static void pushQuad(std::vector<float>& V, int cx, int cy,
    float cellW, float cellH,
    const std::array<float, 3>& c)
{
    // Esquina superior-izquierda en NDC
    float x0 = -1.0f + cx * cellW;
    float y0 = 1.0f - cy * cellH;
    // Esquina inferior-derecha
    float x1 = x0 + cellW;
    float y1 = y0 - cellH;

    auto add = [&](float x, float y) {
        // Posición (x,y,0) + color (r,g,b)
        V.push_back(x); V.push_back(y); V.push_back(0.0f);
        V.push_back(c[0]); V.push_back(c[1]); V.push_back(c[2]);
        };

    // Triángulo 1
    add(x0, y0); add(x1, y0); add(x1, y1);
    // Triángulo 2
    add(x0, y0); add(x1, y1); add(x0, y1);
}

// -----------------------------------------------------------------------------
// main: inicializa GLFW/GLEW, crea el VAO/VBO, carga el shader y dibuja el sprite
// -----------------------------------------------------------------------------
int main()
{
    // --- Inicialización de ventana/contexto -----------------------------------
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(
        WIDTH, HEIGHT,
        "Practica 2 Reynoso Ortega Francisco Javier",
        nullptr, nullptr
    );
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

    // Información de GPU/driver
    std::cout << "> Version:  " << glGetString(GL_VERSION) << '\n';
    std::cout << "> Vendor:   " << glGetString(GL_VENDOR) << '\n';
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << '\n';

    // Ajuste inicial de viewport cuadrado
    int fbw, fbh; glfwGetFramebufferSize(window, &fbw, &fbh);
    resize(window, fbw, fbh);

    // --- Carga de shader ------------------------------------------------------
    Shader shader("Shader/core.vs", "Shader/core.frag");

    // --- Generación de vértices del sprite -----------------------------------
    std::vector<float> vertices;
    vertices.reserve(ROWS * COLS * 6 /*verts*/ * 6 /*attr*/);

    // Tamaño de cada celda en NDC (dos unidades en [-1,1])
    float cellW = 2.0f / COLS;
    float cellH = 2.0f / ROWS;

    // Recorre la matriz y genera quads para celdas != 0 (no dibujar fondo)
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            int code = S[r][c];
            if (code == 0) continue;              // 0 = transparente
            pushQuad(vertices, c, r, cellW, cellH, PALETTE[code]);
        }
    }

    // --- VAO/VBO para el sprite ----------------------------------------------
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW);

    // Atributo 0: posición (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo 1: color (r,g,b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // --- (Opcional) Rejilla guía ---------------------------------------------
    bool drawGrid = false;              // activar si deseas ver la rejilla
    GLuint gridVAO = 0, gridVBO = 0;
    std::vector<float> grid;

    if (drawGrid) {
        // Líneas verticales
        for (int c = 0; c <= COLS; ++c) {
            float x = -1.0f + c * cellW;
            grid.insert(grid.end(), { x,  1.0f, 0.0f, 0.8f,0.8f,0.8f });
            grid.insert(grid.end(), { x, -1.0f, 0.0f, 0.8f,0.8f,0.8f });
        }
        // Líneas horizontales
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

    // --- Bucle principal ------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Limpiar con el color de fondo (índice 0 de la paleta)
        glClearColor(PALETTE[0][0], PALETTE[0][1], PALETTE[0][2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use(); // activa el programa de shader

        // Dibuja rejilla si está habilitada
        if (drawGrid) {
            glBindVertexArray(gridVAO);
            glLineWidth(1.0f);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(grid.size() / 6));
        }

        // Dibuja el sprite (triángulos)
        glBindVertexArray(VAO);
        GLsizei count = static_cast<GLsizei>(vertices.size() / 6); // (pos3+col3)
        glDrawArrays(GL_TRIANGLES, 0, count);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    // --- Limpieza -------------------------------------------------------------
    glfwTerminate();
    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// resize: fija un viewport cuadrado, centrado en la ventana.
// Así, cada celda del sprite mantiene proporción 1:1 (pixel-perfect).
// -----------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    int size = (width < height) ? width : height; // lado del cuadrado
    int x = (width - size) / 2;
    int y = (height - size) / 2;
    glViewport(x, y, size, size);
}
