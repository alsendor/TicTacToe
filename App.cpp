#include "App.h"
#include <string>
#include <vector>
#include "math.h"

using namespace std;

void writeText(const char *text, int x, int y, int length)
{
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x,y);
    for(int i = 0; i < length; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);

}

struct Rect{

    float pointX, pointY, width, height;
    bool clickedOn = false;

    Rect(float x, float y, float w, float h) {
        pointX = x;
        pointY = y;
        width = w;
        height = h;
    }

    float getX() {
        return pointX;
    }

    float getY() {
        return pointY;
    }

    float getW() {
        return width;
    }

    float getH() {
        return height;
    }

    void setX(float temp) {
        pointX = temp;
    }

    void setY(float temp) {
        pointY = temp;
    }

    void setW(float temp) {
        width = temp;
    }

    void setH(float temp) {
        height = temp;
    }

    void click() {
        if (!clickedOn) clickedOn = true;
        else clickedOn = false;
    }

    void unClick() {
        clickedOn = false;
    }

    bool checkClick() {
        return clickedOn;
    }

    void getColor() {
        if (clickedOn) glColor3d(0.0, 1.0, 0.0);
        else glColor3d(1.0, 0.0, 0.0);
    }

    bool contains(float x, float y) {
        return ((x > pointX && x < (pointX + width)) && (y < pointY && y > (pointY - height)));
    }

    ~Rect() {

    }

};


struct gameState {

    int gameMode = 0; //0 for main menu, 1 for double player, 2 for single player
    int turn;			// current move: 0 for player 1 and 1 for player 2 or cpu
    int result;			// Result of the game: 0 is a draw, 1 player 1 wins, 2 player 2 wins
    bool over;			// Is the game Over?
    const int GBsize = 9;
    int gameBoard[9]; //game board each showing value of 0:empty 1:X 2:O

    gameState() {
        turn = 0;
        result = 0;
        over = false;
        for (int i = 0; i < 9; i++) {
            gameBoard[i] = 0;
        }
    }

    int size() {
       return GBsize;
    }

    bool isGameOver() {
        return over;
    }

    void endGame() {
        over = true;
    };

    void changeTurn() {
        if (turn == 0) turn = 1;
        else if (turn == 1) turn = 0;
    }

    int checkTurn() {
        return turn;
    }

    int checkResult() {
        return result;
    }

    void checkWin() {
        //vertical three in a row
        if (gameBoard[0] == 1 && gameBoard[3] == 1 && gameBoard[6] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[1] == 1 && gameBoard[4] == 1 && gameBoard[7] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[2] == 1 && gameBoard[5] == 1 && gameBoard[8] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[0] == 2 && gameBoard[3] == 2 && gameBoard[6] == 2) {
            result = 2;
            over = true;
        }
        if (gameBoard[1] == 2 && gameBoard[4] == 2 && gameBoard[7] == 2) {
            result = 2;
            over = true;
        }
        if (gameBoard[2] == 2 && gameBoard[5] == 2 && gameBoard[8] == 2) {
            result = 2;
            over = true;
        }

        //horizontal three in a row
        if (gameBoard[0] == 1 && gameBoard[1] == 1 && gameBoard[2] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[3] == 1 && gameBoard[4] == 1 && gameBoard[5] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[6] == 1 && gameBoard[7] == 1 && gameBoard[8] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[0] == 2 && gameBoard[1] == 2 && gameBoard[2] == 2) {
            result = 2;
            over = true;
        }
        if (gameBoard[3] == 2 && gameBoard[4] == 2 && gameBoard[5] == 2) {
            result = 2;
            over = true;
        }
        if (gameBoard[6] == 2 && gameBoard[7] == 2 && gameBoard[8] == 2) {
            result = 2;
            over = true;
        }

        //diagonal three in a row
        if (gameBoard[0] == 1 && gameBoard[4] == 1 && gameBoard[8] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[2] == 1 && gameBoard[4] == 1 && gameBoard[6] == 1) {
            result = 1;
            over = true;
        }
        if (gameBoard[0] == 2 && gameBoard[4] == 2 && gameBoard[8] == 2) {
            result = 2;
            over = true;
        }
        if (gameBoard[2] == 2 && gameBoard[4] == 2 && gameBoard[6] == 2) {
            result = 2;
            over = true;
        }

        //check draw
        int check = 1;
        for (int i = 0; i < size(); i++) {
            if (gameBoard[i] == 0) {
                check = 0;
            }
        }
        if (check == 1) {
            result = 0;
            over = true;
        }

    }

    void drawCircle(float x, float y, int resolution){

        float step = (2 * M_PI)/resolution;

        glColor3d(1.0, 1.0, 1.0);
        glBegin(GL_LINE_LOOP);
        for (double theta = 0; theta <= (2 * M_PI); theta += step) {
            float pointX = x + .2 * cos(theta);
            float pointY = y + .2 * sin(theta);
            glVertex2f(pointX, pointY);
        }
        glEnd();
    }

    void drawCross(float x, float y) {
        glColor3d(1.0, 0.0, 0.0);
        glBegin(GL_LINES);

        glVertex2f(x, y);
        glVertex2f(x + .2, y + .2);

        glVertex2f(x, y);
        glVertex2f(x + .2, y - .2);

        glVertex2f(x, y);
        glVertex2f(x - .2, y - .2);

        glVertex2f(x, y);
        glVertex2f(x - .2, y + .2);

        glEnd();
    }

};

vector<Rect*> list;
vector<Rect*> grid;
gameState *game = new gameState();

struct AI {

    AI() {

    }

    //checks if cpu can get three in a row
    int checkThree (int space) {
        switch(space) {
            case 0:
                if ((game->gameBoard[1] == 2 && game->gameBoard[2] == 2) || (game->gameBoard[3] == 2 && game->gameBoard[6] == 2) || (game->gameBoard[4] == 2 && game->gameBoard[8] == 2))
                    return space;
                break;
            case 1:
                if ((game->gameBoard[0] == 2 && game->gameBoard[2] == 2) || (game->gameBoard[4] == 2 && game->gameBoard[7] == 2))
                    return space;
                break;
            case 2:
                if ((game->gameBoard[1] == 2 && game->gameBoard[0] == 2) || (game->gameBoard[4] == 2 && game->gameBoard[6] == 2) || (game->gameBoard[5] == 2 && game->gameBoard[8] == 2))
                    return space;
                break;
            case 3:
                if ((game->gameBoard[0] == 2 && game->gameBoard[6] == 2) || (game->gameBoard[4] == 2 && game->gameBoard[5] == 2))
                    return space;
                break;
            case 4:
                if ((game->gameBoard[0] == 2 && game->gameBoard[8] == 2) || (game->gameBoard[2] == 2 && game->gameBoard[6] == 2) || (game->gameBoard[1] == 2 && game->gameBoard[7] == 2) || (game->gameBoard[3] == 2 && game->gameBoard[5] == 2))
                    return space;
                break;
            case 5:
                if ((game->gameBoard[2] == 2 && game->gameBoard[8] == 2) || (game->gameBoard[3] == 2 && game->gameBoard[4] == 2))
                    return space;
                break;
            case 6:
                if ((game->gameBoard[0] == 2 && game->gameBoard[3] == 2) || (game->gameBoard[4] == 2 && game->gameBoard[2] == 2) || (game->gameBoard[7] == 2 && game->gameBoard[8] == 2))
                    return space;
                break;
            case 7:
                if ((game->gameBoard[4] == 2 && game->gameBoard[1] == 2) || (game->gameBoard[6] == 2 && game->gameBoard[8] == 2))
                    return space;
                break;
            case 8:
                if ((game->gameBoard[0] == 2 && game->gameBoard[4] == 2) || (game->gameBoard[6] == 2 && game->gameBoard[7] == 2) || (game->gameBoard[2] == 2 && game->gameBoard[5] == 2))
                    return space;
                break;
        }
        return -1;
    }

    //if player 1 is going to win then cpu will block out three in a row
    int checkBlock (int space) {
        switch(space) {
            case 0:
                if ((game->gameBoard[1] == 1 && game->gameBoard[2] == 1) || (game->gameBoard[3] == 1 && game->gameBoard[6] == 1) || (game->gameBoard[4] == 1 && game->gameBoard[8] == 1))
                    return space;
                break;
            case 1:
                if ((game->gameBoard[0] == 1 && game->gameBoard[2] == 1) || (game->gameBoard[4] == 1 && game->gameBoard[7] == 1))
                    return space;
                break;
            case 2:
                if ((game->gameBoard[1] == 1 && game->gameBoard[0] == 1) || (game->gameBoard[4] == 1 && game->gameBoard[6] == 1) || (game->gameBoard[5] == 1 && game->gameBoard[8] == 1))
                    return space;
                break;
            case 3:
                if ((game->gameBoard[0] == 1 && game->gameBoard[6] == 1) || (game->gameBoard[4] == 1 && game->gameBoard[5] == 1))
                    return space;
                break;
            case 4:
                if ((game->gameBoard[0] == 1 && game->gameBoard[8] == 1) || (game->gameBoard[2] == 1 && game->gameBoard[6] == 1) || (game->gameBoard[1] == 1 && game->gameBoard[7] == 1) || (game->gameBoard[3] == 1 && game->gameBoard[5] == 1))
                    return space;
                break;
            case 5:
                if ((game->gameBoard[2] == 1 && game->gameBoard[8] == 1) || (game->gameBoard[3] == 1 && game->gameBoard[4] == 1))
                    return space;
                break;
            case 6:
                if ((game->gameBoard[0] == 1 && game->gameBoard[3] == 1) || (game->gameBoard[4] == 1 && game->gameBoard[2] == 1) || (game->gameBoard[7] == 1 && game->gameBoard[8] == 1))
                    return space;
                break;
            case 7:
                if ((game->gameBoard[4] == 1 && game->gameBoard[1] == 1) || (game->gameBoard[6] == 1 && game->gameBoard[8] == 1))
                    return space;
                break;
            case 8:
                if ((game->gameBoard[0] == 1 && game->gameBoard[4] == 1) || (game->gameBoard[6] == 1 && game->gameBoard[7] == 1) || (game->gameBoard[2] == 1 && game->gameBoard[5] == 1))
                    return space;
                break;
        }
        return -1;
    }

    void click() {
        bool check = false;
        if (game->gameBoard[4] == 0) {
            game->gameBoard[4] = 2;
            check = true;
        }
        for (int i = 0; i < game->size(); i++) {
            if (check) break;
            if (checkThree(i) != -1 && game->gameBoard[i] == 0) {
                game->gameBoard[i] = 2;
                check = true;
            }
        }
        for (int i = 0; i < game->size(); i++) {
            if (check) break;
            if (checkBlock(i) != -1 && game->gameBoard[i] == 0) {
                game->gameBoard[i] = 2;
                check = true;
            }
        }
        for (int i = 0; i < game->size(); i++) {
            if (check) break;
            if (game->gameBoard[i] == 0) {
                game->gameBoard[i] = 2;
                check = true;
            }
        }
    }

};

AI *cpu = new AI();

App::App(const char* label, int x, int y, int w, int h): GlutApp(label, x, y, w, h){
    // Initialize state variables
    mx = 0.0;
    my = 0.0;

    list.push_back(new Rect(-.4, .5, .8, .3)); //2 player button
    list.push_back(new Rect(-.4, -.5, .8, .3)); //1 player button

    grid.push_back(new Rect(-1, -.33, .66, .66));
    grid.push_back(new Rect(-1, .33, .66, .66));
    grid.push_back(new Rect(-1, 1, .66, .66));
    grid.push_back(new Rect(-.33, -.33, .66, .66));
    grid.push_back(new Rect(-.33, .33, .66, .66));
    grid.push_back(new Rect(-.33, 1, .66, .66));
    grid.push_back(new Rect(.33, -.33, .66, .66));
    grid.push_back(new Rect(.33, .33, .66, .66));
    grid.push_back(new Rect(.33, 1, .66, .66));

}

void App::draw() {


    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set background color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Set up the transformations stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set Color
    glColor3d(1.0, 1.0, 1.0);

    int size;
    //main menu
    if (game->gameMode == 0) {

        glLineWidth(5);
        glBegin(GL_LINES);
        glColor3d(0.0, 0.0, 1.0);

        glVertex2f(0, .45);
        glVertex2f(0, .25);

        glVertex2f(-.025, -.55);
        glVertex2f(-.025, -.75);

        glVertex2f(.025, -.55);
        glVertex2f(.025, -.75);


        glEnd();

        size = list.size();
        for (int i = 0; i < size; i++) {
            list[i]->getColor();
            glBegin(GL_POLYGON);

            glVertex2f(list[i]->getX(), list[i]->getY());
            glVertex2f(list[i]->getX() + list[i]->getW(), list[i]->getY());
            glVertex2f(list[i]->getX() + list[i]->getW(), list[i]->getY() - list[i]->getH());
            glVertex2f(list[i]->getX(), list[i]->getY() - list[i]->getH());

            glEnd();
        }



    }
    else if (game->isGameOver()) {
        if (game->result == 0){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            string text1;
            text1 = "Draw";
            glColor3f(1.0, 1.0, 1.0);
            writeText(text1.data(), 350, 300, 15);
        }
        else if (game->result == 1){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            string text2;
            text2 = "Player 1 Wins";
            glColor3f(1.0, 1.0, 1.0);
            writeText(text2.data(), 350, 300, 15);
        }
        else if (game->result == 2){
            glClearColor(0.0, 0.0, 0.0, 1.0);
            string text3;
            text3 = "Player 2 Wins";
            glColor3f(1.0, 1.0, 1.0);
            writeText(text3.data(), 350, 300, 15);
        }
    }
        //actual game
    else if (game->gameMode == 1 || game->gameMode == 2){

        size = grid.size();
        for (int i = 0; i < size; i++) {
            if (game->gameBoard[i] == 2) {
                game->drawCircle(grid[i]->getX() + .33, grid[i]->getY() - .33, 69);
            }
            else if (game->gameBoard[i] == 1) {
                game->drawCross(grid[i]->getX() + .33, grid[i]->getY() - .33);
            }
        }

        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3d(1.0, 1.0, 1.0);

        glVertex2f(-.33, 1);
        glVertex2f(-.33, -1);

        glVertex2f(.33, 1);
        glVertex2f(.33, -1);

        glVertex2f(-1, -.33);
        glVertex2f(1, -.33);

        glVertex2f(-1, .33);
        glVertex2f(1, .33);

        glEnd();


    }

    /*
    // Draw some points
    glBegin(GL_POINTS);
    
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    
    glEnd();

     */
    // Draw a yellow cross
    glColor3d(1.0, 1.0, 0.0);

    glLineWidth(2);
    glBegin(GL_LINES);
    
    glVertex2f(mx - 0.05f, my);
    glVertex2f(mx + 0.05f, my);
    
    glVertex2f(mx, my - 0.05f);
    glVertex2f(mx, my + 0.05f);
    
    glEnd();

    
    // We have been drawing everything to the back buffer
    // Swap the buffers to see the result of what we drew
    glFlush();
    glutSwapBuffers();
}

void App::mouseDown(float x, float y){
    // Update app state
    mx = x;
    my = y;

    if (game->gameMode == 0) {
        if (list[0]->contains(x, y)) {
            game->gameMode = 2;
        }
        else if (list[1]->contains(x, y)) {
            game->gameMode = 1;
        }
    }
    else if (game->gameMode == 1 && game->isGameOver() == false) {
        int size = grid.size();
        for (int i = 0; i < size; i++) {
            if(grid[i]->contains(x, y) && game->gameBoard[i] == 0) {
                grid[i]->click();
                if (game->checkTurn() == 0 ) {
                    game->gameBoard[i] = 1;
                }
                else if (game->checkTurn() == 1) {
                    game->gameBoard[i] = 2;
                }
                game->changeTurn();
            }
            else {
                grid[i]->unClick();
            }
        }
        game->checkWin();
    }
    else if (game->gameMode == 2 && game->isGameOver() == false) {
        int size = grid.size();
        for (int i = 0; i < size; i++) {
            if(grid[i]->contains(x, y)) {
                grid[i]->click();
                if (game->checkTurn() == 0  && game->gameBoard[i] == 0) {
                    game->gameBoard[i] = 1;
                    game->changeTurn();
                }
                else if (game->checkTurn() == 1) {
                    cpu->click();
                    game->changeTurn();
                }
            }
            else {
                grid[i]->unClick();
            }
        }
        game->checkWin();
    }

    // Redraw the scene
    redraw();
}

void App::mouseDrag(float x, float y){
    // Update app state
    mx = x;
    my = y;
    
    // Redraw the scene
    redraw();
}

void App::keyPress(unsigned char key) {
    if (key == 27){
        // Exit the app when Esc key is pressed
        exit(0);
    }
}
