#define GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>

#include "stb_loader.h"

#include "piece.h"
#include "game.h"

unsigned int loadShader(char* fileName);
unsigned int loadTexture(const char* fileName);
int fillBufferFromBoard(_BOARD* board, unsigned int vbo);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

_BOARD* b;

char turn;

char selected = 0;
int selectedX = 0;
int selectedY = 0;
int nPossibleMoves = 0;
MOVE* possibleMoves = NULL;

int main()
{
    srand(time(0));

    b = createStdBoard();

    system("pause");

    print_board(b, 0);

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowTitle(window, "Chess");

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    if (glewInit() != GLEW_OK) {
        printf("Error in GlewInit()!");
        return -1;
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    unsigned int boardVao;
    glGenVertexArrays(1, &boardVao);
    glBindVertexArray(boardVao);

    float vertices[] = {
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0, 1,
    -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0, 0,
     1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1, 1,
     1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1, 0
    };

    unsigned int elements[] = {
        0, 1, 3, 3, 2, 0
    };

    unsigned int BoardVbo;
    glGenBuffers(1, &BoardVbo);
    glBindBuffer(GL_ARRAY_BUFFER, BoardVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(boardVao);
    glBindBuffer(GL_ARRAY_BUFFER, BoardVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    unsigned int vertSh = loadShader("basicShader.vert");
    unsigned int fragSh = loadShader("basicShader.frag");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertSh);
    glAttachShader(shaderProgram, fragSh);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    unsigned int attribLoc = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);

    attribLoc = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    
    attribLoc = glGetAttribLocation(shaderProgram, "texCoord");
    glEnableVertexAttribArray(attribLoc);
    glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));


    unsigned int piecesVao;
    glCreateVertexArrays(1, &piecesVao);
    glBindVertexArray(piecesVao);

    unsigned int vertPieces = loadShader("piecesShader.vert");
    unsigned int geomPieces = loadShader("piecesShader.geom");
    unsigned int fragPieces = loadShader("piecesShader.frag");

    unsigned int piecesShaderProgram = glCreateProgram();
    glAttachShader(piecesShaderProgram, vertPieces);
    glAttachShader(piecesShaderProgram, geomPieces);
    glAttachShader(piecesShaderProgram, fragPieces);

    glBindFragDataLocation(piecesShaderProgram, 0, "outColor");

    glLinkProgram(piecesShaderProgram);
    glUseProgram(piecesShaderProgram);

    attribLoc = glGetAttribLocation(piecesShaderProgram, "a_position");
    glEnableVertexAttribArray(attribLoc);

    attribLoc = glGetAttribLocation(piecesShaderProgram, "a_tex");
    glEnableVertexAttribArray(attribLoc);

    unsigned int piecesVbo;
    glCreateBuffers(1, &piecesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, piecesVbo);

    fillBufferFromBoard(b, piecesVbo);
    
    glVertexAttribIPointer(0, 2, GL_INT, 3 * sizeof(int), 0);
    glVertexAttribIPointer(1, 1, GL_INT, 3 * sizeof(int), 2 * sizeof(int));

    

    unsigned int pieceTextures[] = {
        loadTexture("rsc/img/pieces/white/King.png"),
        loadTexture("rsc/img/pieces/white/Queen.png"),
        loadTexture("rsc/img/pieces/white/Rook.png"),
        loadTexture("rsc/img/pieces/white/Knight.png"),
        loadTexture("rsc/img/pieces/white/Bishop.png"),
        loadTexture("rsc/img/pieces/white/Pawn.png"),
        loadTexture("rsc/img/pieces/black/King.png"),
        loadTexture("rsc/img/pieces/black/Queen.png"),
        loadTexture("rsc/img/pieces/black/Rook.png"),
        loadTexture("rsc/img/pieces/black/Knight.png"),
        loadTexture("rsc/img/pieces/black/Bishop.png"),
        loadTexture("rsc/img/pieces/black/Pawn.png"),
        loadTexture("rsc/img/Selection.png"),
        loadTexture("rsc/img/Col2.png"),
        loadTexture("rsc/img/Capture.png"),
        loadTexture("rsc/img/Evolve.png")
    };

    unsigned int boardTexture = loadTexture("ChessBoard.png");

    glUseProgram(piecesShaderProgram);
    for (size_t i = 0; i < sizeof(pieceTextures) / 4; i++)
    {
        glActiveTexture(GL_TEXTURE1 + i);
        glBindTexture(GL_TEXTURE_2D, pieceTextures[i]);
        glUniform1i(i, i + 1);
    }

    unsigned int uniLoc = glGetUniformLocation(piecesShaderProgram, "pieceSize");
    glUniform1f(uniLoc, .22);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, boardTexture);

    int i = 0;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.03f, 0.04f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(boardVao);
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(piecesVao);
        glUseProgram(piecesShaderProgram);


        int nPieces = fillBufferFromBoard(b, piecesVbo);

        glDrawArrays(GL_POINTS, 0, nPieces);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    
    free(b);

    return 0;
}

unsigned int loadShader(char* fileName) {
    int fNameSize = -1;
    while (fileName[++fNameSize] != 0);

    int shaderType = 0;

    if ((fileName[fNameSize - 1] == 't') && (fileName[fNameSize - 2] == 'r') && (fileName[fNameSize - 3] == 'e') && (fileName[fNameSize - 4] == 'v') && (fileName[fNameSize - 5] == '.'))
        shaderType = GL_VERTEX_SHADER;
    if ((fileName[fNameSize - 1] == 'g') && (fileName[fNameSize - 2] == 'a') && (fileName[fNameSize - 3] == 'r') && (fileName[fNameSize - 4] == 'f') && (fileName[fNameSize - 5] == '.'))
        shaderType = GL_FRAGMENT_SHADER;
    if ((fileName[fNameSize - 1] == 'm') && (fileName[fNameSize - 2] == 'o') && (fileName[fNameSize - 3] == 'e') && (fileName[fNameSize - 4] == 'g') && (fileName[fNameSize - 5] == '.'))
        shaderType = GL_GEOMETRY_SHADER;
    if ((fileName[fNameSize - 1] == 'p') && (fileName[fNameSize - 2] == 'm') && (fileName[fNameSize - 3] == 'o') && (fileName[fNameSize - 4] == 'c') && (fileName[fNameSize - 5] == '.'))
        shaderType = GL_COMPUTE_SHADER;

    if (shaderType == 0) {
        printf("Error: could not identify type of shader in file '%s'", fileName);
        return 0;
    }

    if (_access(fileName, 0) != 0) {
        printf("Error: could not access file '%s'", fileName);
        return 0;
    }

    FILE* f = fopen(fileName, "r");
    if (!f) return -1;

    fseek(f, 0L, SEEK_END);
    int fSize = ftell(f);
    rewind(f);

    char* fContent = malloc(fSize + 1);
    if (!fContent) return -1;

    int i = 0;
    while ((i < fSize) && (fContent[i++] = fgetc(f)) != EOF);
    
    fContent[i-1] = 0;

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &fContent, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        printf(buffer);
    }

    free(fContent);

    return shader;
}

unsigned int loadTexture(const char* fileName) {
    int tex;

    int w;
    int h;
    int comp;
    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    glGenTextures(1, &tex);
    if (tex == 0)
        return 0;

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    unsigned char* image = stbi_load(fileName, &w, &h, &comp, STBI_rgb_alpha);

    if (!image)
        printf("Failed to load texture '%s'", fileName);

    if (comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if (comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

    return tex;
}

int fillBufferFromBoard(_BOARD* b, unsigned int vbo) {
    int nPieces = 0;

    for (size_t i = 0; i < b->nPieces; i++)
    {
        if (b->pieces[i].present)
            nPieces++;
    }

    int nMoves = 0;
    MOVE* moves = NULL;
    if (&b->pieces[b->squares[selectedY * 8 + selectedX] & 0b1111111] && selected) {
        moves = getPossibleMoves(&b->pieces[b->squares[selectedY * 8 + selectedX] & 0b1111111], b);
        if (moves) {
            while (moves[nMoves++].valid);
            nMoves--;
        }
    }

    int* result = (int*)malloc((nPieces + nPossibleMoves) * 3 * sizeof(int));
    if (!result)
        return 0;

    int j = 0;
    if (possibleMoves) {
        while (possibleMoves[j].valid) {
            result[j * 3] = possibleMoves[j].x1;
            result[j * 3 + 1] = possibleMoves[j].y1;
            result[j * 3 + 2] = possibleMoves[j].cap ? 14 : 12;
            j++;
        }
    }

    for (size_t i = 0; i < b->nPieces; i++)
    {
        if (b->pieces[i].present) {
            result[j * 3] = b->pieces[i].x;
            result[j * 3 + 1] = b->pieces[i].y;
            result[j * 3 + 2] = (int) (b->pieces[i].ptemplate - b->game.pieceTypes);
            if (b->pieces[i].col)
                result[3 * j + 2] += 6;
            j++;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (nPieces + nMoves) * 12, result, GL_DYNAMIC_DRAW);

    free(result);

    return nPieces + nMoves;
}

int t;

int mouseX = 0, mouseY = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        selectedX = mouseX / 100;
        selectedY = 7 - mouseY / 100;
        printf("(%d, %d)\n", selectedX, selectedY);
        selected = b->squares[8 * selectedY + selectedX] && b->pieces[b->squares[8 * selectedY + selectedX] & 0b1111111].col == turn;
        if (selected) {
            if (possibleMoves)
                free(possibleMoves);
            possibleMoves = getPossibleMoves(&b->pieces[b->squares[8 * selectedY + selectedX] & 0b1111111], b);
            nPossibleMoves = 0;
            while (possibleMoves[nPossibleMoves++].valid);
            nPossibleMoves--;
        }
        else {
            if (possibleMoves)
                free(possibleMoves);
            possibleMoves = NULL;
            nPossibleMoves = 0;
        }
        /*MOVE* moves = NULL;
        int nMoves = 0;
        while (1) {
            PIECE* randPiece = &b->pieces[rand() % b->nPieces];

            if (!randPiece->present || randPiece->col != turn)
                continue;

            moves = getPossibleMoves(randPiece, b);
            if (!moves)
                continue;

            while (moves[nMoves++].valid);
            nMoves--;

            if (nMoves) {
                MOVE m = moves[rand() % nMoves];
                _move(b, &m);
                turn ^= 1;
                break;
            }
        }*/
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (selected) {
            MOVE* moves = getPossibleMoves(&b->pieces[b->squares[selectedY * 8 + selectedX] & 0b1111111], b);
            int i = -1;
            while (moves[++i].valid) {
                int x = mouseX / 100;
                int y = 7 - mouseY / 100;
                if ((moves[i].x1 == x) && (moves[i].y1 == y)) {
                    _move(b, moves + i);
                    if (possibleMoves)
                        free(possibleMoves);
                    possibleMoves = NULL;
                    nPossibleMoves = 0;
                    selected = 0;
                    turn ^= 1;
                }
            }
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = round(xpos);
    mouseY = round(ypos);
}