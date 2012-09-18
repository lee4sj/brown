#ifndef PONG_H
#define PONG_H
#include <qgl.h>

class Pong {
public:
    Pong(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY);
    void setPaddleVelocity(GLfloat vel, bool isPlayer1);
    void drawObjects();
    void updateGame();
    void startGame();
    int getScore1() { return m_p1Score;}
    int getScore2() { return m_p2Score;}

    bool m_started;

private:
    int moveBall();
    void resetGame();

    //paddle position variables
    GLfloat m_paddle1X;
    GLfloat m_paddle1Y;
    GLfloat m_paddle2X;
    GLfloat m_paddle2Y;

    //paddle velocity
    GLfloat m_paddle1Vel;
    GLfloat m_paddle2Vel;

    //paddle dimensions
    GLfloat m_paddleWidth;
    GLfloat m_paddleHeight;

    //ball variables
    GLfloat m_ballDiameter;
    GLfloat m_ballX;
    GLfloat m_ballY;
    GLfloat m_ballXVel;
    GLfloat m_ballYVel;
    GLfloat m_ballSpeedMultiplier;

    //game score and game dimensions
    int m_p1Score;
    int m_p2Score;
    int m_minX;
    int m_minY;
    int m_maxX;
    int m_maxY;
    int m_gameHeight;
    int m_gameWidth;
};

#endif // PONG_H
