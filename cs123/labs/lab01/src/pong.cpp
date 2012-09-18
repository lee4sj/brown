#include "pong.h"
#include <iostream>
#include <math.h>
#include <time.h>
using namespace std;

static const GLfloat PADDLE_VELOCITY = 0.02;
static const GLfloat PLAYER_1_POINT = 1;
static const GLfloat PLAYER_2_POINT = 2;

Pong::Pong(GLfloat minX, GLfloat maxX, GLfloat minY, GLfloat maxY) {
    m_p1Score = 0;
    m_p2Score = 0;

    //setup initial values for paddles
    m_minX = minX;
    m_minY = minY;
    m_maxX = maxX;
    m_maxY = maxY;
    m_gameHeight = m_maxX - m_minX;
    m_gameWidth = m_maxY - m_minY;

    m_paddleWidth = m_gameWidth * 0.01;
    m_paddleHeight = m_gameHeight * 0.2;
    m_ballDiameter = m_gameHeight * 0.02;
    m_started = false;
    resetGame();
    srand(time(NULL));
}

/**
    Draw the ball and paddles.
    You will have to fill this in.
**/
void Pong::drawObjects() {
    //TODO: draw the ball for 6.3
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(m_ballX, m_ballY, 0);

    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(m_ballDiameter, 0);
    glVertex2f(m_ballDiameter, m_ballDiameter);
    glVertex2f(0, m_ballDiameter);
    glEnd();

    glPopMatrix();


    //TODO: draw paddles for 6.5
    glPushMatrix();
    glTranslatef(m_paddle1X, m_paddle1Y, 0);

    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(m_paddleWidth, 0);
    glVertex2f(m_paddleWidth, m_paddleHeight);
    glVertex2f(0, m_paddleHeight);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(m_paddle2X, m_paddle2Y, 0);

    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(m_paddleWidth, 0);
    glVertex2f(m_paddleWidth, m_paddleHeight);
    glVertex2f(0, m_paddleHeight);
    glEnd();

    glPopMatrix();
}


/**
    Resets the game state.
**/
void Pong::resetGame() {
    m_paddle1X = m_minX;
    m_paddle2X = m_maxX - m_paddleWidth;

    m_paddle1Y = (m_maxY - (m_gameHeight / 2.0)) - (m_paddleHeight / 2.0);
    m_paddle2Y = m_paddle1Y;

    m_ballX = (m_maxX - (m_gameWidth / 2.0)) - (m_ballDiameter / 2.0);
    m_ballY = (m_maxY - (m_gameHeight / 2.0)) - (m_ballDiameter / 2.0);

    m_ballXVel = .5 - (rand() % 2);
    m_ballYVel = 1 - 2 * (rand() % 2);
    m_paddle1Vel = 0.0;
    m_paddle2Vel = 0.0;
    m_ballSpeedMultiplier = .01;
}

/**
    Called on each tick in paintGL from glwidget.cpp
**/
void Pong::updateGame() {
    if (!m_started) {
        return;
    }

    //update position of paddles, making sure paddles stay in bounds of game
    if ((m_paddle1Vel < 0 && m_paddle1Y > m_minY) ||
            (m_paddle1Vel > 0  && (m_paddle1Y + m_paddleHeight) < m_maxY)) {
        m_paddle1Y += m_paddle1Vel;
    }

    if ((m_paddle2Vel < 0 && m_paddle2Y > m_minY) ||
            (m_paddle2Vel > 0  && (m_paddle2Y + m_paddleHeight) < m_maxY)) {
        m_paddle2Y += m_paddle2Vel;
    }

    //update position of ball
    int gameState = moveBall();
    if (gameState == PLAYER_1_POINT) {
        m_p1Score += 1;
        m_started = false;
        resetGame();
    } else if (gameState == PLAYER_2_POINT) {
        m_p2Score += 1;
        m_started = false;
        resetGame();
    }
}

/**
    Called by glwidget.cpp key events to update paddle movement
**/
void Pong::setPaddleVelocity(GLfloat strength, bool isPlayer1) {
    if (isPlayer1) {
        m_paddle1Vel = strength * PADDLE_VELOCITY;
    } else {
        m_paddle2Vel = strength * PADDLE_VELOCITY;
    }
}

/**
    Helper method to update balls position by checking against bounds of the game and paddles.
**/
int Pong::moveBall() {
    bool hitPaddle = false;
    m_ballX += m_ballSpeedMultiplier * m_ballXVel;
    m_ballY += m_ballSpeedMultiplier * m_ballYVel;

    if ((m_ballY <= m_minY || m_ballY >= m_maxY) &&
        (m_ballX >= m_minX && m_ballX <= m_maxX)) { //check against top and bottom
        m_ballYVel *= -1;
    }

    if (m_ballX <= m_minX) { //check against left paddle
        if (m_ballY >= m_paddle1Y && m_ballY <= (m_paddle1Y + m_paddleHeight)) {
            m_ballXVel *= -1;
            hitPaddle = true;
        } else {
            return PLAYER_2_POINT;
        }
    } else if ((m_ballX + m_ballDiameter) >= (m_maxX - m_paddleWidth)) { //check against right paddle
        if (m_ballY >= m_paddle2Y && m_ballY <= (m_paddle2Y + m_paddleHeight)) {
            m_ballXVel *= -1;
            hitPaddle = true;
        } else {
            return PLAYER_1_POINT;
        }
    }

    if (hitPaddle) { //increase ball speed over time
        m_ballSpeedMultiplier += 0.001;
    }

    return 0;
}
