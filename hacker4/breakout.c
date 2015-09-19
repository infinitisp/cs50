//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

//initial paddle width; variable so it can change
int PADDLEWIDTH = 100;

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    //set initial vel, xvel, and yvel (this includes the angle)
    double vel = 0.08;
    double xvel = (drand48()*(vel*0.6) + vel*0.2)  * (((int) drand48()*2)*2 - 1); 
    double yvel = sqrt(vel*vel - xvel*xvel);
    double ballx = WIDTH/2 - 10.0;
    double bally = HEIGHT/2 -10.0;
    
     //wait for click before starting game
     while (true){
     
                    GEvent event = getNextEvent(MOUSE_EVENT);
                    if (event != NULL)
                        if (getEventType(event) == MOUSE_CLICKED)
                            break;
                    }

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
                       
            //MOVE PADDLE
            // check for mouse event
            GEvent event = getNextEvent(MOUSE_EVENT);

            // if we heard one
            if (event != NULL)
            {
                // if the event was movement
                if (getEventType(event) == MOUSE_MOVED)
                {
                    // make paddle follow x
                    double x = getX(event) - getWidth(paddle) / 2;
                    setLocation(paddle, x, getY(paddle));
                }
            }
        
            // MOVE BALL
            ballx += xvel;
            bally += yvel;
            setLocation(ball, (int) ballx, (int) bally);

            // bounce on x-dimension
            if (ballx + getWidth(ball) >= WIDTH || ballx <= 0)
                xvel *= -1;
            
            //bounce on top of screen
            if (bally <= 0)
                yvel *= -1;
                
            //hits bottom of screen
            if (bally + getHeight(ball) >= HEIGHT) {
                
                lives -=1;
                
                //update scoreboard
                char s[20];
                sprintf(s, "Score: %d | Lives: %d", points, lives);
                setLabel(label, s);
                
                //quit if dead 
                if (lives == 0) break;
                
                //reset ball and reset ball angle/speed
                removeGWindow(window,ball);
                ball = initBall(window);
                xvel = (drand48()*(vel*0.6) + vel*0.2) * (((int) drand48()*2)*2 - 1); 
                yvel = sqrt(vel*vel - xvel*xvel);
                ballx = WIDTH/2 - 10.0;
                bally = HEIGHT/2 -10.0;
                
                //wait for click before starting again
                while (true){
                    GEvent event = getNextEvent(MOUSE_EVENT);
                    if (event != NULL)
                        if (getEventType(event) == MOUSE_CLICKED)
                            break;
                    }
            }
                 
        
        //DEAL WITH COLLISIONS    
            GObject object = detectCollision(window, ball);
            
            //if paddle bounce according to spot on paddle
            if (object == paddle) {
                xvel = ((ballx+RADIUS) - (getX(paddle)+PADDLEWIDTH/2))/PADDLEWIDTH * vel; //+ (0.5 - drand48())*vel/5;
                yvel = -1 * sqrt(vel*vel - xvel*xvel);
            } 
            
            //if brick, take out the brick and add points and stuff
            else if (object != NULL && strcmp(getType(object),"GRect") == 0) {
                yvel *= -1;   
                removeGWindow(window, object);
                points++;
                bricks--;
                
                //I like how hacker edition was simply adding two lines to my code (sorry I'm lazy to do more, but others are pretty easy too):
                PADDLEWIDTH -= 1; //decrease paddlesize 
                vel += 0.005; //increase ball speed
                
                //reset paddle and scoreboard
                setSize(paddle, PADDLEWIDTH, 8);
                char s[20];
                sprintf(s, "Score: %d | Lives: %d", points, lives);
                setLabel(label, s); 
            }
        
        
        
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int spaceSize = 4;
    int rectWidth = WIDTH / (COLS) - spaceSize - 1;
    int rectHeight = 8;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) {
            GRect g = newGRect(c*rectWidth + spaceSize*(c+1), 30 + r*rectHeight + r*spaceSize, rectWidth, rectHeight);
            setColor(g, "BLACK");
            setFilled(g, true);
            add(window, g);
        }

}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval circle = newGOval(WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS, RADIUS*2, RADIUS*2);
    setColor(circle, "BLACK");
    setFilled(circle, true);
    add(window, circle);
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH / 2 - PADDLEWIDTH/2, HEIGHT - 30, PADDLEWIDTH, 8);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    char s[20];
    sprintf(s, "Score: 0 | Lives: 3");
    GLabel label = newGLabel(s);
    setLocation(label, 4, 20);
    setFont(label, "SansSerif-12");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
