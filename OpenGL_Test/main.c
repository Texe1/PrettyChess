#define GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>

#include "stb_loader.h"

#include "board.h"
#include "piece.h"
#include "game.h"

unsigned int loadShader(char* fileName);
unsigned int loadTexture(const char* fileName);
int fillBufferFromBoard(_BOARD* board, unsigned int vbo);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

BOARD board;
_BOARD* b;

int main()
{
    srand(time(0));

    b = createStdBoard();

    /*MOVE m = { 0 };
    m.x0 = 6;
    m.y0 = 7;
    m.x1 = 3;
    m.y1 = 3;

    _move(b, &m);
    MOVE* moves = getPossibleMoves(&b->pieces[b->nPieces - 2], b);*/

    system("pause");

    print_board(b, 0);

    /*for (int i = 0; i < 100; i++) {
        MOVE* moves = NULL;
        int nMoves = 0;
        while (1) {
            PIECE* randPiece = &b->pieces[rand() % b->nPieces];

            if (!randPiece->present)
                continue;

            moves = getPossibleMoves(randPiece, b);
            if (!moves)
                continue;

            while (moves[nMoves++].valid);
            nMoves--;

            if (nMoves)
                break;
        }

        MOVE m = moves[rand() % nMoves];
        _move(b, &m);

        print_board(b);
        printf("Made move: (%d, %d) -> (%d,%d), valid:%d\n", m.x0, m.y0, m.x1, m.y1, m.valid);
        system("pause");
    }*/


    
    board = (BOARD){ 0 };
    fillBoard(&board);

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

    float verticesWhiteKing[] = {
    -.97f, -.97f, 1.0f, 0.0f, 0.0f, 0, 1,
    -.97f, -.78f, 0.0f, 1.0f, 0.0f, 0, 0,
    -.78f, -.97f, 0.0f, 0.0f, 1.0f, 1, 1,
    -.78f, -.78f, 0.0f, 0.0f, 1.0f,  1, 0
    };

    unsigned int elementsWhiteKing[] = {
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

    unsigned int whKingVao;
    glGenVertexArrays(1, &whKingVao);
    glBindVertexArray(whKingVao);

    unsigned int whKingVbo;
    glGenBuffers(1, &whKingVbo);
    glBindBuffer(GL_ARRAY_BUFFER, whKingVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), verticesWhiteKing, GL_STATIC_DRAW);

    unsigned int whKingEbo;
    glGenBuffers(1, &whKingEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, whKingEbo);
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

    int* result = (int*)malloc(nPieces * 3 * sizeof(int));
    if (!result)
        return 0;

    int j = 0;
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
    glBufferData(GL_ARRAY_BUFFER, nPieces * 12, result, GL_DYNAMIC_DRAW);

    free(result);

    return nPieces;
}

//int fillBufferFromBoard(BOARD* board, unsigned int vbo) {
//    int nPieces = 0;
//    int nColored = 0;
//    int evolveCol = 0;
//
//    if (board->evolve) {
//        while (PIECE_TYPE(board->squares[56 * board->turn + evolveCol].piece) != PAWN) {
//            evolveCol++;
//        }
//    }
//
//    for (size_t i = 0; i < 64; i++)
//    {
//        SQUARE* s = board->squares + i;
//        if (PIECE_TYPE(s->piece) != 0) {
//            nPieces++;
//        }
//    }
//
//    int selected = (board->selected & ~64);
//    for (int i = 0; i < 64; i++)
//    {
//        SQUARE* s = board->squares + i;
//        if (s->color != 0) {
//            nPieces++;
//            nColored++;
//        }
//    }
//    if (board->selected >= 64) {
//        nPieces++; nColored++;
//    }
//    int* result = (int*)malloc(nPieces * 3 * sizeof(int) + board->evolve * 4*12);
//    if (!result)
//        return NULL;
//
//
//    nColored = 0;
//    if (board->selected >= 64) {
//        result[0] = board->selected % 8;
//        result[1] = (selected - (selected % 8)) / 8;
//        result[2] = 12;
//        nColored++;
//    }
//
//
//
//    for (int i = 0; i < 64; i++)
//    {
//        SQUARE* s = board->squares + i;
//        if (s->color) {
//            result[3 * nColored] = i % 8;
//            result[3 * nColored + 1] = (i - i % 8) / 8;
//            result[3 * nColored + 2] = (s->piece || ((i % 8 != board->selected % 8) && (PIECE_TYPE(board->squares[board->selected & 0b111111].piece) == PAWN))) ? 14 : 13;
//            nColored++;
//        }
//    }
//
//    int j = 0;
//    for (int i = 0; i < 64; i++)
//    {
//        SQUARE* s = board->squares + i;
//        if (PIECE_TYPE(s->piece) != 0) {
//            result[3 * (j + nColored)] = i % 8;
//            result[3 * (j + nColored) + 1] = (i - (i % 8)) / 8;
//            result[3 * (j + nColored) + 2] = 6 - PIECE_TYPE(s->piece);
//            if (!IS_BLACK(s->piece))
//                result[3 * (j + nColored) + 2] += 6;
//
//            if (++j == nPieces)
//                break;
//        }
//    }
//
//    if (board->evolve) {
//        printf("evolve!");
//        result[3 * nPieces] = evolveCol;
//        result[3 * nPieces + 1] = board->turn ? 6 : 1;
//        result[3 * nPieces + 2] = 15 & (1 << 4);
//        nPieces++;
//        result[3 * nPieces] = evolveCol;
//        result[3 * nPieces + 1] = board->turn ? 5 : 2;
//        result[3 * nPieces + 2] = 15 & (2 << 4);
//        nPieces++;
//        result[3 * nPieces] = evolveCol;
//        result[3 * nPieces + 1] = board->turn ? 4 : 3;
//        result[3 * nPieces + 2] = 15 & (3 << 4);
//        nPieces++;
//        result[3 * nPieces] = evolveCol;
//        result[3 * nPieces + 1] = board->turn ? 3 : 4;
//        result[3 * nPieces + 2] = 15 & (4 << 4);
//        nPieces++;
//    }
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, nPieces * 12, result, GL_DYNAMIC_DRAW);
//
//    free(result);
//
//    return nPieces;
//}

int t;

int mouseX = 0, mouseY = 0;

int turn = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        /*if (board.selected >= 64) 
            int x0 = board.selected % 8;
            int y0 = (board.selected % 64 - board.selected % 8) / 8;
            int x1 = (mouseX - mouseX % 100) / 100;
            int y1 = 7 - ((mouseY - mouseY % 100) / 100);
            move(&board, x0, y0, x1, y1);
            printf("(%d, %d) -> (%d, %d)\n", x0, y0, x1, y1);
            deselectInBoard(&board);
            
        } else {

            deselectInBoard(&board);
            selectInBoard(&board, (mouseX - mouseX % 100) / 100, 7 - ((mouseY - mouseY % 100) / 100));
        }*/
        MOVE* moves = NULL;
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
        }

        
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = round(xpos);
    mouseY = round(ypos);
}