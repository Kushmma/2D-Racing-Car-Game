#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <dos.h>
#include "score.h"

using namespace std;

// Global variables
int lapPlayer = 0, lapAI = 0;
bool gameOver = false;
bool playerPassedFinish = false;
bool aiPassedFinish = false;
int gameMode = 0;

// Screen dimensions
const int screenWidth = 1900;
const int screenHeight = 1000;

// Road boundaries
const int roadLeft = 200;
const int roadRight = 1700;
const int roadTop = 100;
const int roadBottom = 900;

// Finish line position
const int finishLineX = roadLeft + 150;

// Car dimensions
const int carWidth = 60;
const int carHeight = 40;

int laneHeight = (roadBottom - roadTop) / 2;

// Obstacle constants
const int MAX_OBSTACLES = 8; // Increased from 5 to 8
const int REQUIRED_LAPS = 5;

struct Obstacle {
    int x;
    int y;
    int type;
    bool active;
};

void drawRoad() {
    // Draw black road surface
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, roadTop, screenWidth, roadBottom);

    // Draw yellow boundary lines
    setcolor(YELLOW);
    setlinestyle(SOLID_LINE, 0, 3);
    line(0, roadTop, screenWidth, roadTop);
    line(0, roadBottom, screenWidth, roadBottom);
    line(0, roadBottom + 20, screenWidth, roadBottom + 20);

    // Draw finish line (red)
    setcolor(RED);
    setlinestyle(SOLID_LINE, 0, 5);
    line(finishLineX, roadTop, finishLineX, roadBottom);
    
    // Draw "FINISH" text
    outtextxy(finishLineX - 40, roadTop - 30, (char*)"FINISH");

    // Draw dashed yellow center line
    setcolor(YELLOW);
    setlinestyle(DASHED_LINE, 0, 3);
    int centerY = (roadTop + roadBottom) / 2;
    for (int x = 20; x < screenWidth; x += 40) {
        line(x, centerY, x + 20, centerY);
    }

    // Draw white lane markers (dashed)
    setcolor(WHITE);
    setlinestyle(DASHED_LINE, 0, 2);
    int lane1 = roadTop + (roadBottom - roadTop) / 4;
    int lane2 = roadTop + 3 * (roadBottom - roadTop) / 4;
    
    for (int x = 20; x < screenWidth; x += 60) {
        line(x, lane1, x + 30, lane1);
        line(x, lane2, x + 30, lane2);
    }

    // Reset to default
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 1);
}

void drawCar(int x, int y, int color) {
    COLORREF bodyColor;
    switch(color) {
        case BLUE: bodyColor = RGB(0, 0, 255); break;
        case RED: bodyColor = RGB(255, 0, 0); break;
        case GREEN: bodyColor = RGB(0, 255, 0); break;
        case CYAN: bodyColor = RGB(0, 255, 255); break;
        case YELLOW: bodyColor = RGB(255, 255, 0); break;
        default: bodyColor = RGB(255, 255, 255);
    }

    setfillstyle(SOLID_FILL, bodyColor);
    int body[] = {x+10,y+15, x+30,y, x+90,y, x+110,y+15, x+110,y+35, 
                 x+100,y+40, x+80,y+45, x+40,y+45, x+20,y+40, x+10,y+35, x+10,y+15};
    fillpoly(11, body);

    setfillstyle(SOLID_FILL, COLOR(120, 180, 220));
    int windows[] = {x+30,y+5, x+90,y+5, x+85,y+20, x+30,y+20};
    fillpoly(4, windows);

    setfillstyle(SOLID_FILL, COLOR(30, 30, 30));
    int spoiler[] = {x+80,y, x+100,y-8, x+100,y-3, x+80,y+5};
    fillpoly(4, spoiler);
    
    setcolor(COLOR(80, 80, 75));
    line(x+50, y+25, x+70, y+25);

    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x+25, y+45, 18, 12);
    fillellipse(x+95, y+45, 18, 12);
    
    setfillstyle(SOLID_FILL, COLOR(180, 180, 180));
    fillellipse(x+25, y+45, 12, 8);
    fillellipse(x+95, y+45, 12, 8);
    
    setfillstyle(SOLID_FILL, COLOR(100, 100, 100));
    fillellipse(x+25, y+45, 5, 4);
    fillellipse(x+95, y+45, 5, 4);

    setfillstyle(SOLID_FILL, COLOR(255, 255, 200));
    sector(x+10, y+20, 270, 360, 12, 8);
    
    setfillstyle(SOLID_FILL, COLOR(255, 40, 40));
    sector(x+110, y+20, 90, 180, 12, 8);

    setfillstyle(SOLID_FILL, COLOR(60, 60, 60));
    bar(x+105, y+35, x+110, y+40);
    
    setcolor(WHITE);
}

void drawBarrel(int x, int y) {
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x + 5, y, x + carWidth - 5, y + carHeight);

    setfillstyle(SOLID_FILL, COLOR(255, 140, 0)); 
    bar(x + 5, y + 4, x + carWidth - 5, y + 8);
    bar(x + 5, y + 14, x + carWidth - 5, y + 18);
}

void drawOilSpill(int x, int y) {
    setfillstyle(SOLID_FILL, DARKGRAY);
    fillellipse(x + carWidth/2, y + carHeight/2, carWidth/2, carHeight/3);
}

void drawSpikes(int x, int y) {
    setcolor(DARKGRAY);
    setfillstyle(SOLID_FILL, RED);
    bar(x, y + carHeight/2, x + carWidth, y + carHeight);

    setcolor(WHITE);
    for (int i = x + 4; i < x + carWidth - 4; i += 8) {
        line(i, y + carHeight/2, i + 4, y);
        line(i + 4, y, i + 8, y + carHeight/2);
    }
}

void drawObstacle(int x, int y, int type) {
    switch (type) {
        case 0: drawBarrel(x, y); break;
        case 1: drawOilSpill(x, y); break;
        case 2: drawSpikes(x, y); break;
    }
}

bool checkCollision(int carX, int carY, int obsX, int obsY) {
    return (carX < obsX + carWidth && carX + carWidth > obsX &&
            carY < obsY + carHeight && carY + carHeight > obsY);
}

void checkLapCompletion(int carX, int carY, bool &passedFlag, int &lapCount) {
    if (carX + carWidth > finishLineX && carX < finishLineX + 10) {
        if (!passedFlag) {
            passedFlag = true;
            lapCount++;
        }
    } else if (carX < finishLineX - 50) {
        passedFlag = false;
    }
}

void showLapCounter1() {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);  
    setcolor(WHITE);

    ostringstream ss;
    ss << "Player Lap: " << min(lapPlayer, REQUIRED_LAPS) << " / " << REQUIRED_LAPS;
    outtextxy(roadLeft + 10, 10, (char*)ss.str().c_str());
}

void showLapCounter2() {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);  
    setcolor(WHITE);

    int textWidth = 300;
    int gap = 100;
    int totalWidth = 2 * textWidth + gap;
    int startX = (screenWidth - totalWidth) / 2;
    
    ostringstream p1Lap;
    p1Lap << "Player 1: " << min(lapPlayer, REQUIRED_LAPS) << "/" << REQUIRED_LAPS;
    outtextxy(startX, 10, (char*)p1Lap.str().c_str());
    
    ostringstream p2Lap;
    p2Lap << "Player 2: " << min(lapAI, REQUIRED_LAPS) << "/" << REQUIRED_LAPS;
    outtextxy(startX + textWidth + gap, 10, (char*)p2Lap.str().c_str());
}

void showWinner(const char* msg) {
    if (gameMode == 1) {
        sp_totalMatches++;
        if (strcmp(msg, "You Win!") == 0) sp_playerWins++;
        else if (strcmp(msg, "	COMPUTER Wins!") == 0) sp_aiWins++;
    } else if (gameMode == 2) {
        mp_totalMatches++;
        if (strcmp(msg, "Player 1 Wins!") == 0) mp_player1Wins++;
        else if (strcmp(msg, "Player 2 Wins!") == 0) mp_player2Wins++;
    }

    cleardevice();
    settextstyle(4, 0, 14);
    setcolor(YELLOW);
    outtextxy(screenWidth/2 - 250, screenHeight/2-150, (char*)msg);
    delay(4000);
}

void spawnObstacle(Obstacle obstacles[], int count) {
    for (int i = 0; i < count; i++) {
        if (!obstacles[i].active) {
            obstacles[i].x = roadRight + rand() % 500;
            obstacles[i].y = roadTop + 40 + rand() % (roadBottom - roadTop - 80);
            obstacles[i].type = rand() % 3;
            obstacles[i].active = true;
            break;
        }
    }
}

void singlePlayer() {
    if (getmaxx() == 0) {
        initwindow(screenWidth, screenHeight, "Racing Game");
    }
    cleardevice();

    gameMode = 1;
    const int playerSpeed = 10;
    const int aiSpeed = 10;
    const int obstacleSpeed = 8;
    
    int playerX = roadLeft + 100;
    int playerY = roadTop + laneHeight + 40;
    int aiX = roadLeft + 100;
    int aiY = roadTop + 40;
    
    // Initialize obstacles - start with 3 active obstacles
    Obstacle obstacles[MAX_OBSTACLES];
    for (int i = 0; i < 3; i++) {  // Start with 3 obstacles
        obstacles[i] = {roadRight + rand() % 500, 
                       roadTop + 40 + rand() % (roadBottom - roadTop - 80), 
                       rand() % 3, true};
    }
    for (int i = 3; i < MAX_OBSTACLES; i++) {
        obstacles[i] = {0, 0, 0, false};
    }
    
    lapPlayer = lapAI = 0;
    playerPassedFinish = aiPassedFinish = false;
    gameOver = false;
    srand(time(0));
    
    float aiYVel = 0;
    float aiTargetY = aiY;
    int avoidanceTimer = 0;
    float frameCounter = 0;
    int obstacleSpawnTimer = 0;
    int difficultyTimer = 0;
    int currentObstacleSpeed = obstacleSpeed;

    // For smooth controls
    int playerYVel = 0;
    const float playerAcceleration = 0.5f;
    const float playerFriction = 0.9f;

    while (!gameOver) {
        frameCounter += 0.1f;
        obstacleSpawnTimer++;
        difficultyTimer++;

        // Gradually increase difficulty
        if (difficultyTimer % 200 == 0 && currentObstacleSpeed < 12) {
            currentObstacleSpeed++;
        }

        if (playerX >= screenWidth - 20) {
            playerX = 0;
        }
        if (aiX >= screenWidth -20 ) {
            aiX = 0;
        }
        
        setactivepage(1 - getvisualpage());
        cleardevice();
    
        drawRoad();
        drawCar(playerX, playerY, YELLOW);
        drawCar(aiX, aiY, CYAN);
        
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                drawObstacle(obstacles[i].x, obstacles[i].y, obstacles[i].type);
            }
        }
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        setcolor(WHITE);
        
        ostringstream playerLap;
        playerLap << "Player Lap: " << min(lapPlayer, REQUIRED_LAPS) << "/" << REQUIRED_LAPS;
        ostringstream ailap;
        ailap << "Computer Lap: " << min(lapAI, REQUIRED_LAPS) << "/" << REQUIRED_LAPS;
        
        int textWidth = textwidth((char*)playerLap.str().c_str());
        int startX = (screenWidth - textWidth) / 2;
        
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(startX - 20, 10, startX + textWidth + 20, 50);
        
        outtextxy(startX, 15, (char*)playerLap.str().c_str());
        
        // Move obstacles
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                obstacles[i].x -= currentObstacleSpeed;
                
                if (obstacles[i].x < roadLeft - carWidth - 100) {
                    obstacles[i].active = false;
                }
            }
        }
        
        // Spawn new obstacles more frequently (every 30-50 frames)
        if (obstacleSpawnTimer > 30 && rand() % 100 < 40) {
            spawnObstacle(obstacles, MAX_OBSTACLES);
            obstacleSpawnTimer = 0;
        }

        checkLapCompletion(playerX, playerY, playerPassedFinish, lapPlayer);
        checkLapCompletion(aiX, aiY, aiPassedFinish, lapAI);

        // Improved AI movement logic
        if (rand() % 100 < 3) {
            // Occasionally pick a new random target position
            aiTargetY = roadTop + 40 + rand() % (laneHeight - 80);
        }

        // Check for upcoming obstacles
        bool dangerAhead = false;
        int closestObstacleY = aiY;
        int closestDistance = 10000;

        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                int distance = obstacles[i].x - aiX;
                
                // If obstacle is ahead and within 400 pixels
                if (distance > -carWidth && distance < 400) {
                    // If obstacle is in our path (with some margin)
                    if (abs(obstacles[i].y - aiY) < carHeight + 20) {
                        dangerAhead = true;
                        
                        // Track the closest obstacle
                        if (distance < closestDistance) {
                            closestDistance = distance;
                            closestObstacleY = obstacles[i].y;
                        }
                    }
                }
            }
        }

        if (dangerAhead) {
            // Determine best avoidance direction
            int avoidanceMargin = 30; // How much space to leave when avoiding
            
            // Calculate top and bottom available space
            int topSpace = closestObstacleY - (roadTop + 10);
            int bottomSpace = (roadTop + laneHeight - 10) - (closestObstacleY + carHeight);
            
            if (topSpace > bottomSpace && topSpace > carHeight + avoidanceMargin) {
                // More space above - go up
                aiTargetY = roadTop + 40;
            } else if (bottomSpace > carHeight + avoidanceMargin) {
                // More space below - go down
                aiTargetY = roadTop + laneHeight - carHeight - 10;
            } else {
                // Not enough space - emergency brake (slow down temporarily)
                aiX -= 5;
                // Try to squeeze through
                if (topSpace > bottomSpace) {
                    aiTargetY = roadTop + 40;
                } else {
                    aiTargetY = roadTop + laneHeight - carHeight - 10;
                }
            }
        }

        // Smooth AI movement
        aiYVel += (aiTargetY - aiY) * 0.02f;
        aiYVel *= 0.8f;
        aiY += aiYVel;
        
        // Keep AI in bounds
        aiY = max(roadTop + 10, min(roadTop + laneHeight - carHeight - 10, aiY));

        playerX += playerSpeed;
        aiX += aiSpeed;

        // Smooth player controls
        if (kbhit()) {
            char ch = tolower(getch());
            
            switch(ch) {
                case 'w': playerYVel = -playerSpeed; break;
                case 's': playerYVel = playerSpeed; break;
            }
        }
        
        // Apply movement physics
        playerY += playerYVel;
        playerYVel *= playerFriction;
        
        // Keep player in bounds
        playerY = max(roadTop + laneHeight + 10, min(roadBottom - carHeight - 10, playerY));

        // Check collisions
        bool playerCrashed = false;
        bool aiCrashed = false;
        
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active && obstacles[i].x < roadRight - carWidth) {
                if (checkCollision(playerX, playerY, obstacles[i].x, obstacles[i].y)) {
                    playerCrashed = true;
                }
                if (checkCollision(aiX, aiY, obstacles[i].x, obstacles[i].y)) {
                    // Only count as crash if it's a significant collision
                    if (abs(obstacles[i].y - aiY) < carHeight/2) {
                        aiCrashed = true;
                    } else {
                        // Minor collision - just bounce off
                        aiYVel = (obstacles[i].y > aiY) ? -5 : 5;
                    }
                }
            }
        }

        if (playerCrashed || aiCrashed) {
            gameOver = true;
            setvisualpage(1 - getvisualpage());

            if (playerCrashed && aiCrashed) showWinner("Double Crash! Tie Game!");
            else if (playerCrashed) showWinner("Game Over! You Crashed!");
            else showWinner("You Win! Bot Crashed!");
            break;
        }

        // Check win conditions
        if (lapPlayer >= REQUIRED_LAPS && lapAI >= REQUIRED_LAPS) {
            gameOver = true;
            setvisualpage(1 - getvisualpage());
            showWinner("Tie Game!");
            break;
        } else if (lapPlayer >= REQUIRED_LAPS) {
            gameOver = true;
            setvisualpage(1 - getvisualpage());
            showWinner("You Win!");
            break;
        } else if (lapAI >= REQUIRED_LAPS) {
            gameOver = true;
            setvisualpage(1 - getvisualpage());
            showWinner("Bot Wins!");
            break;
        }

        setvisualpage(1 - getvisualpage());
        delay(30);  // Reduced delay for smoother gameplay
    }
}

void multiPlayer() {
    if (getmaxx() == 0) {
        initwindow(screenWidth, screenHeight, "Racing Game - Multiplayer Mode");
    }
    cleardevice();

    gameMode = 2;
    const int verticalSpeed = 8;
    const int horizontalSpeedLeft = 10;
    const int horizontalSpeedRight = 7;
    const int obstacleSpeed = 6;
    const int baseSpeed = 8;
    
    const int p1StartX = roadLeft + 50;
    const int p1StartY = roadTop + 40;
    const int p2StartX = roadLeft + 50;
    const int p2StartY = roadTop + laneHeight + 40;

    int p1X = p1StartX;
    int p1Y = p1StartY;
    int p2X = p2StartX;
    int p2Y = p2StartY;

    Obstacle obstacles[MAX_OBSTACLES];
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        int lane = (i % 2) ? 1 : 2;
        int laneCenter = (lane == 1) ? roadTop + (laneHeight/2) : roadTop + laneHeight + (laneHeight/2);
        
        obstacles[i] = {
            roadRight + 200 + rand() % 1000,
            laneCenter - carHeight/2 + (rand() % 40 - 20),
            rand() % 3, 
            true
        };
    }

    lapPlayer = lapAI = 0;
    playerPassedFinish = aiPassedFinish = false;
    gameOver = false;
    srand(time(0));
    int obstacleSpawnTimer = 0;
    bool p1Crashed = false, p2Crashed = false;
    int crashDisplayTimer = 0;
    int difficultyTimer = 0;
    int currentObstacleSpeed = obstacleSpeed;

    while (!gameOver) {
        obstacleSpawnTimer++;
        difficultyTimer++;
        
        if (difficultyTimer % 200 == 0 && currentObstacleSpeed < 12) {
            currentObstacleSpeed++;
        }

        setactivepage(1 - getvisualpage());
        cleardevice();

        if (!p1Crashed) p1X += baseSpeed;
        if (!p2Crashed) p2X += baseSpeed;

       if (p1X >= screenWidth) p1X = -carWidth;
       if (p2X >= screenWidth) p2X = -carWidth;
        
   

        drawRoad();
        drawCar(p1X, p1Y, p1Crashed ? RED : BLUE);
        drawCar(p2X, p2Y, p2Crashed ? RED : YELLOW);
        
        if (p1X < 0) drawCar(p1X + screenWidth, p1Y, p1Crashed ? RED : BLUE);
        if (p2X < 0) drawCar(p2X + screenWidth, p2Y, p2Crashed ? RED : YELLOW);
        
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                if (obstacles[i].x > roadRight - 100 && obstacles[i].x < roadRight + 100) {
                    if ((obstacleSpawnTimer/5) % 2 == 0) {
                        setcolor(YELLOW);
                        circle(obstacles[i].x + carWidth/2, obstacles[i].y + carHeight/2, carWidth);
                        setcolor(WHITE);
                    }
                }
                drawObstacle(obstacles[i].x, obstacles[i].y, obstacles[i].type);
            }
        }

        settextstyle(8, HORIZ_DIR, 3);
        setcolor(WHITE);
        
        int textWidth = 200;
        int gap = 100;
        int totalWidth = 2 * textWidth + gap;
        int startX = (screenWidth - totalWidth) / 2;
        
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(startX - 10, 10, startX + textWidth + 10, 50);
        bar(startX + textWidth + gap - 10, 10, startX + totalWidth + 10, 50);
        
        ostringstream p1Lap;
        p1Lap << "PLAYER1: " << min(lapPlayer, REQUIRED_LAPS) << "/" << REQUIRED_LAPS;
        outtextxy(startX, 15, (char*)p1Lap.str().c_str());
        
        ostringstream p2Lap;
        p2Lap << "PLAYER2: " << min(lapAI, REQUIRED_LAPS) << "/" << REQUIRED_LAPS;
        outtextxy(startX + textWidth + gap, 15, (char*)p2Lap.str().c_str());

        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (obstacles[i].active) {
                obstacles[i].x -= currentObstacleSpeed;
                
                if (obstacles[i].x < roadLeft - carWidth - 50) {
                    obstacles[i].active = false;
                }
            }
        }
        
        if (obstacleSpawnTimer > 40 && rand() % 100 < 40) {
            spawnObstacle(obstacles, MAX_OBSTACLES);
            
            if (rand() % 3 == 0) {
                for (int i = 0; i < MAX_OBSTACLES; i++) {
                    if (!obstacles[i].active) {
                        obstacles[i].x = roadRight + rand() % 500;
                        obstacles[i].y = (roadTop + roadBottom)/2 - carHeight/2;
                        obstacles[i].type = rand() % 3;
                        obstacles[i].active = true;
                        break;
                    }
                }
            }
            obstacleSpawnTimer = 0;
        }

        if (!p1Crashed) checkLapCompletion(p1X, p1Y, playerPassedFinish, lapPlayer);
        if (!p2Crashed) checkLapCompletion(p2X, p2Y, aiPassedFinish, lapAI);

        if (!p1Crashed) {
            if (GetAsyncKeyState('A') & 0x8000) p1X = max(roadLeft + 10, p1X - horizontalSpeedLeft);
            if (GetAsyncKeyState('D') & 0x8000) p1X = min(roadRight - carWidth - 10, p1X + horizontalSpeedRight);
            if (GetAsyncKeyState('W') & 0x8000) p1Y = max(roadTop + 10, p1Y - verticalSpeed);
            if (GetAsyncKeyState('S') & 0x8000) p1Y = min(roadTop + laneHeight - carHeight - 10, p1Y + verticalSpeed);
            
            static int p1LastKey = 0;
            static int p1KeyTimer = 0;
            if (p1KeyTimer > 0) p1KeyTimer--;
            
            if (GetAsyncKeyState('A') & 0x8000) {
                if (p1LastKey == 'A' && p1KeyTimer > 0) p1X -= horizontalSpeedLeft * 2;
                p1LastKey = 'A';
                p1KeyTimer = 10;
            }
            if (GetAsyncKeyState('D') & 0x8000) {
                if (p1LastKey == 'D' && p1KeyTimer > 0) p1X += horizontalSpeedRight * 2;
                p1LastKey = 'D';
                p1KeyTimer = 10;
            }
        }
        
        if (!p2Crashed) {
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) p2X = max(roadLeft + 10, p2X - horizontalSpeedLeft);
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) p2X = min(roadRight - carWidth - 10, p2X + horizontalSpeedRight);
            if (GetAsyncKeyState(VK_UP) & 0x8000) p2Y = max(roadTop + laneHeight + 10, p2Y - verticalSpeed);
            if (GetAsyncKeyState(VK_DOWN) & 0x8000) p2Y = min(roadBottom - carHeight - 10, p2Y + verticalSpeed);
            
            static int p2LastKey = 0;
            static int p2KeyTimer = 0;
            if (p2KeyTimer > 0) p2KeyTimer--;
            
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
                if (p2LastKey == VK_LEFT && p2KeyTimer > 0) p2X -= horizontalSpeedLeft * 2;
                p2LastKey = VK_LEFT;
                p2KeyTimer = 10;
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
                if (p2LastKey == VK_RIGHT && p2KeyTimer > 0) p2X += horizontalSpeedRight * 2;
                p2LastKey = VK_RIGHT;
                p2KeyTimer = 10;
            }
        }

        if (!p1Crashed || !p2Crashed) {
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                if (obstacles[i].active && obstacles[i].x < roadRight - carWidth) {
                    if (!p1Crashed && checkCollision(p1X, p1Y, obstacles[i].x, obstacles[i].y)) {
                        p1Crashed = true;
                        crashDisplayTimer = 30;
                        setcolor(RED);
                        circle(obstacles[i].x + carWidth/2, obstacles[i].y + carHeight/2, carWidth);
                        setcolor(WHITE);
                    }
                    if (!p2Crashed && checkCollision(p2X, p2Y, obstacles[i].x, obstacles[i].y)) {
                        p2Crashed = true;
                        crashDisplayTimer = 30;
                        setcolor(RED);
                        circle(obstacles[i].x + carWidth/2, obstacles[i].y + carHeight/2, carWidth);
                        setcolor(WHITE);
                    }
                }
            }
        }

        if (crashDisplayTimer > 0) crashDisplayTimer--;

        if ((p1Crashed && p2Crashed) || 
            (lapPlayer >= REQUIRED_LAPS && lapAI >= REQUIRED_LAPS)) {
            if (crashDisplayTimer == 0) {
                gameOver = true;
                setvisualpage(1 - getvisualpage());
                if (p1Crashed && p2Crashed) showWinner("Double Crash! Tie Game!");
                else if (lapPlayer >= REQUIRED_LAPS && lapAI >= REQUIRED_LAPS) showWinner("Tie Game!");
                break;
            }
        }
        else if (lapPlayer >= REQUIRED_LAPS || p2Crashed) {
            if (crashDisplayTimer == 0) {
                gameOver = true;
                setvisualpage(1 - getvisualpage());
                showWinner("Player 1 Wins!");
                break;
            }
        }
        else if (lapAI >= REQUIRED_LAPS || p1Crashed) {
            if (crashDisplayTimer == 0) {
                gameOver = true;
                setvisualpage(1 - getvisualpage());
                showWinner("Player 2 Wins!");
                break;
            }
        }

        setvisualpage(1 - getvisualpage());
        delay(40);
    }
}
