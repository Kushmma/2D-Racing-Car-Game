#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include "game.h"
#include "score.h"

// Functions
int showMenu();
void showStartingScreen();
void singlePlayer();
void multiPlayer();
void drawStaticBackground();
void drawMenu(int selected);
void drawSportsCar(int x, int y, COLORREF bodyColor);
void drawClassicCar(int x, int y, COLORREF bodyColor);

const int NUM_STARS = 200;
const int NUM_MOUNTAINS = 5;
const int NUM_TREES = 30;

int starX[NUM_STARS], starY[NUM_STARS], starSpeed[NUM_STARS], starSize[NUM_STARS];
COLORREF starColors[NUM_STARS];
int mountainPeaks[NUM_MOUNTAINS];
int titleX = 730;
int titleY = 120;
int speedLinesX[20], speedLinesAlpha[20];
int treeX[NUM_TREES], treeHeight[NUM_TREES], treeType[NUM_TREES];

// Stars and environment initialization
void initStars() {
    // Initialize stars
    for (int i = 0; i < NUM_STARS; i++) {
        starX[i] = rand() % getmaxx();
        starY[i] = rand() % int(getmaxy() * 0.5);
        starSpeed[i] = 1 + rand() % 3;
        starSize[i] = 1 + rand() % 3;
        int blueTint = 200 + rand() % 55;
        starColors[i] = COLOR(blueTint, blueTint, 255);
    }

    // Initialize mountains
    mountainPeaks[0] = -50;
    for (int i = 1; i < NUM_MOUNTAINS; i++) {
        mountainPeaks[i] = mountainPeaks[i-1] + (getmaxx() / (NUM_MOUNTAINS-1)) + rand() % 150 - 75;
    }

    // Initialize speed lines
    for (int i = 0; i < 20; i++) {
        speedLinesX[i] = rand() % getmaxx();
    }
}

// Draw mountains
void drawMountains() {
    int mountainBase = getmaxy() * 0.6;
    
    // Distant mountains (darker)
    setfillstyle(SOLID_FILL, COLOR(15, 20, 30));
    for (int i = 1; i < NUM_MOUNTAINS; i++) {
        int height = 60 + rand() % 90;
        int poly[6] = {
            mountainPeaks[i-1], mountainBase,
            (mountainPeaks[i-1] + mountainPeaks[i])/2, mountainBase - height,
            mountainPeaks[i], mountainBase
        };
        fillpoly(3, poly);
    }
    
    // Closer mountains (lighter)
    setfillstyle(SOLID_FILL, COLOR(25, 30, 40));
    for (int i = 1; i < NUM_MOUNTAINS; i++) {
        int height = 90 + rand() % 120;
        int poly[6] = {
            mountainPeaks[i-1], mountainBase,
            (mountainPeaks[i-1] + mountainPeaks[i])/2, mountainBase - height,
            mountainPeaks[i], mountainBase
        };
        fillpoly(3, poly);
    }
}

// Draw stars
void drawStars() {
    for (int i = 0; i < NUM_STARS; i++) {
        starX[i] += starSpeed[i];
        starY[i] += starSpeed[i]/3;
        
        if (starX[i] > getmaxx() || starY[i] > getmaxy() * 0.5) {
            starX[i] = rand() % 50;
            starY[i] = rand() % int(getmaxy() * 0.3);
        }
        
        if (rand() % 100 < 5) {
            starSize[i] = 2 + rand() % 4;
        } else {
            starSize[i] = 1 + rand() % 2;
        }
        
        setcolor(starColors[i]);
        setfillstyle(SOLID_FILL, starColors[i]);
        fillellipse(starX[i], starY[i], starSize[i], starSize[i]);
        
        if (starSize[i] > 2) {
            setcolor(COLOR(
                GetRValue(starColors[i])/2,
                GetGValue(starColors[i])/2,
                GetBValue(starColors[i])/2
            ));
            fillellipse(starX[i], starY[i], starSize[i]+1, starSize[i]+1);
        }
    }
}
const COLORREF RED_CAR = COLOR(220, 50, 50);
const COLORREF BLUE_CAR = COLOR(50, 50, 220);

void drawSportsCar(int x, int y, COLORREF bodyColor, bool isPlayer1 = true) {
    // Main body (sleek design for both cars)
    setfillstyle(SOLID_FILL, bodyColor);
    int body[] = {
        x+10, y+15,  // front-bottom
        x+30, y,     // front-top
        x+90, y,     // rear-top
        x+110, y+15, // rear-mid
        x+110, y+35, // rear-bottom
        x+100, y+40,
        x+80, y+45,
        x+40, y+45,
        x+20, y+40,
        x+10, y+35,  // front-bottom
        x+10, y+15
    };
    fillpoly(11, body);

    // Windows with gradient effect
    if(isPlayer1) {
        // Player 1 windows (wider windshield)
        setfillstyle(SOLID_FILL, COLOR(120, 180, 220));
        int windows[] = {x+30, y+5, x+90, y+5, x+85, y+20, x+30, y+20};
        fillpoly(4, windows);
    } else {
        // Player 2 windows (more angled)
        setfillstyle(SOLID_FILL, COLOR(100, 160, 210));
        int windows[] = {x+35, y+5, x+85, y+5, x+80, y+20, x+35, y+20};
        fillpoly(4, windows);
    }

    // Car details based on player
    if(isPlayer1) {
        // Player 1 - rear spoiler
        setfillstyle(SOLID_FILL, COLOR(30, 30, 30));
        int spoiler[] = {x+80, y, x+100, y-8, x+100, y-3, x+80, y+5};
        fillpoly(4, spoiler);
        
        // Side vents
        setcolor(COLOR(80, 80, 80));
        line(x+50, y+25, x+70, y+25);
    } else {
        // Player 2 - racing stripe
        setfillstyle(SOLID_FILL, COLOR(220, 220, 220));
        int stripe[] = {x+30, y+10, x+90, y+10, x+85, y+15, x+35, y+15};
        fillpoly(4, stripe);
    }

    // Wheels (detailed for both cars)
    setfillstyle(SOLID_FILL, BLACK);
    fillellipse(x+25, y+45, 18, 12);
    fillellipse(x+95, y+45, 18, 12);
    
    // Wheel rims
    setfillstyle(SOLID_FILL, COLOR(180, 180, 180));
    fillellipse(x+25, y+45, 12, 8);
    fillellipse(x+95, y+45, 12, 8);
    
    // Wheel hubs
    setfillstyle(SOLID_FILL, COLOR(100, 100, 100));
    fillellipse(x+25, y+45, 5, 4);
    fillellipse(x+95, y+45, 5, 4);

    // Lights
    setfillstyle(SOLID_FILL, COLOR(255, 255, 200));
    sector(x+10, y+20, 270, 360, 12, 8); // Headlight
    
    setfillstyle(SOLID_FILL, COLOR(255, 40, 40));
    sector(x+110, y+20, 90, 180, 12, 8); // Taillight

    // Exhaust pipes
    setfillstyle(SOLID_FILL, COLOR(60, 60, 60));
    bar(x+105, y+35, x+110, y+40);
    
    // Reset to default color
    setcolor(WHITE);
}

void drawClassicCar(int x, int y, COLORREF bodyColor) {
    drawSportsCar(x, y, bodyColor, false);
}
void drawStaticBackground() {
    // Sky gradient (unchanged)
    for (int i = 0; i < getmaxy() * 0.6; i++) {
        int blueShade = 30 + i / 3;      
        int purpleTint = i / 25;
        setcolor(COLOR(10+purpleTint, 10+purpleTint, blueShade));
        line(0, i, getmaxx(), i);
    }

    drawMountains();
    drawStars();
    
    // Road surface (unchanged)
    setfillstyle(SOLID_FILL, COLOR(25, 25, 30));
    bar(0, getmaxy() * 0.6, getmaxx(), getmaxy());

    // Road markings (unchanged)
    setcolor(COLOR(220, 220, 180));
    for (int i = 0; i < getmaxx(); i += 100) {
        setlinestyle(SOLID_LINE, 0, 3);
        line(i, getmaxy() * 0.8, i + 60, getmaxy() * 0.8);
        setlinestyle(SOLID_LINE, 0, 2);
        line(i+30, getmaxy() * 0.7, i+50, getmaxy() * 0.7);
        line(i+30, getmaxy() * 0.9, i+50, getmaxy() * 0.9);
    }

    // Road border (unchanged)
    setfillstyle(SOLID_FILL, COLOR(60, 60, 65));
    bar(0, getmaxy() * 0.6 - 15, getmaxx(), getmaxy() * 0.6);
    setfillstyle(SOLID_FILL, COLOR(100, 100, 100));
    bar(0, getmaxy() * 0.6 - 5, getmaxx(), getmaxy() * 0.6);

    // Road texture (unchanged)
    setcolor(COLOR(40, 40, 45));
    int roadTop = static_cast<int>(getmaxy() * 0.6);
    int roadHeight = getmaxy() - roadTop;
    for (int i = 0; i < 300; i++) {
        int x = rand() % getmaxx();
        int y = roadTop + rand() % roadHeight;
        if (rand() % 3 == 0) putpixel(x, y, COLOR(40, 40, 45));
        else if (rand() % 5 == 0) line(x, y, x + rand() % 5, y + rand() % 3);
    }

    // Title (unchanged)
    settextstyle(8, 0, 10);
    setcolor(COLOR(50, 50, 150));
    outtextxy((getmaxx()-textwidth((char*)"RACING CARS"))/2+3, 123, (char*)"RACING CARS");
    setcolor(COLOR(100, 100, 255));
    outtextxy((getmaxx()-textwidth((char*)"RACING CARS"))/2+1, 121, (char*)"RACING CARS");
    setcolor(WHITE);
    outtextxy((getmaxx()-textwidth((char*)"RACING CARS"))/2, 120, (char*)"RACING CARS");

    // Draw 6 cars with cyan at bottom
    int roadTopY = static_cast<int>(getmaxy() * 0.65);
    int roadBottomY = static_cast<int>(getmaxy() * 0.85);
    int carWidth = 120; // Approximate car width
    
    // Array of car colors
    COLORREF carColors[] = {
        COLOR(220, 50, 50),   // Red
        COLOR(50, 50, 220),   // Blue
        COLOR(50, 220, 50),   // Green
        COLOR(220, 220, 50),  // Yellow
        COLOR(220, 50, 220),  // Purple
        COLOR(50, 220, 220)   // Cyan
    };
    
    // Generate random but non-overlapping positions
    int prevX = -carWidth; // Initialize to ensure first car has space
    
    for (int i = 0; i < 6; i++) {
        int carX, carY;
        bool positionValid;
        int attempts = 0;
        
        // Keep trying until we find a non-overlapping position
        do {
            positionValid = true;
            carX = 50 + rand() % (getmaxx() - 100);
            
            // Special case for cyan car (last in array) - position at bottom
            if (i == 5) { // Cyan is the 6th car (index 5)
                carY = roadBottomY - 50; // Fixed position near bottom
            } else {
                carY = roadTopY + rand() % (roadBottomY - roadTopY - 50);
            }
            
            // Check if too close to previous car
            if (abs(carX - prevX) < carWidth * 1.5) {
                positionValid = false;
            }
            
            attempts++;
            if (attempts > 100) break; // Prevent infinite loops
        } while (!positionValid);
        
        prevX = carX; // Update for next car
        
        // Alternate between sports and classic cars
        if (i % 2 == 0) {
            drawSportsCar(carX, carY - 20, carColors[i], false);
        } else {
            drawClassicCar(carX, carY - 20, carColors[i]);
        }
    }
}
// Menu
void drawMenu(int selected) {
    const char* options[] = {"Single Player", "Multiplayer", "View Scores", "Exit"};
    int total = 4;

    settextstyle(8, 0, 5);

    // Calculate center positions
    int centerX = getmaxx() / 2;
    int startY = getmaxy() / 2 - 140; // Start 140 pixels above center

    for (int i = 0; i < total; i++) {
        int textWidth = textwidth((char*)options[i]);
        int x = centerX - textWidth / 2; // Center each menu item
        int y = startY + i * 70;

        if (i == selected) {
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, COLOR(50, 50, 50)); 
            bar(x - 30, y - 10, x + textWidth + 30, y + 40);

            setcolor(YELLOW); 
            outtextxy(x, y, (char*)options[i]);
        } 
        else {
            setcolor(WHITE); 
            outtextxy(x, y, (char*)options[i]);
        }
    }
}

int showMenu() {
    int selected = 0;
    int lastSelected = -1;

    drawStaticBackground();  
    drawMenu(selected);         

    while (true) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 0 || ch == 224) ch = getch();  

            if (ch == 72) {       		 // Up arrow
                selected = (selected - 1 + 4) % 4;
            } else if (ch == 80) { 		// Down arrow
                selected = (selected + 1) % 4;
            } else if (ch == 13) { 		// Enter 
                return selected + 1;
            }

            if (selected != lastSelected) {
                drawMenu(selected);
                lastSelected = selected;
            }
        }

        delay(20); 
    }
}

void showStartingScreen() {
    // Set large font for countdown
    settextstyle(8, HORIZ_DIR, 12);
    
    int centerX = getmaxx() / 2;
    int centerY = getmaxy() / 2;
    
    // Draw the static background first
    drawStaticBackground();
    
    // Show "Starting in..." message first
    const char* startingMsg = "Starting in...";
    int startingW = textwidth((char*)startingMsg);
    int startingH = textheight((char*)startingMsg);
    
    setcolor(WHITE);
    outtextxy(centerX - startingW/2, centerY - startingH - 50, (char*)startingMsg);
    delay(100);
    
    // Countdown from 3
    for (int i = 3; i >= 1; i--) {
        char msg[20];
        sprintf(msg, "%d", i); // Just the number

        // Calculate text width and height
        int textW = textwidth((char*)msg);
        int textH = textheight((char*)msg);
        
        // Clear previous number by redrawing background portion
        setfillstyle(SOLID_FILL, COLOR(10, 10, 50));
        bar(centerX - textW/2 - 10, centerY - textH/2 - 10, 
            centerX + textW/2 + 10, centerY + textH/2 + 10);
        
        // Draw the number
        setcolor(YELLOW);
        outtextxy(centerX - textW/2, centerY - textH/2, (char*)msg);
        
        delay(1000); // 1 second delay
    }

    // "GO!" message
    const char* goMsg = "GO!!";
    int goW = textwidth((char*)goMsg);
    int goH = textheight((char*)goMsg);
    
    // Clear previous number
    setfillstyle(SOLID_FILL, COLOR(10, 10, 50));
    bar(centerX - goW/2 - 10, centerY - goH/2 - 10, 
        centerX + goW/2 + 10, centerY + goH/2 + 10);
    
    // Draw "GO!"
    setcolor(GREEN);
    outtextxy(centerX - goW/2, centerY - goH/2, (char*)goMsg);
    
    delay(800); // 0.8 second delay for "GO!"
}
int main() {
    int i, maxx, midy;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeight, "RACING CAR GAME");
    initStars();
    
    maxx = getmaxx();
    midy = getmaxy()/2;
    
    while (true) {
        int choice = showMenu();

        if (choice == 1) {
            showStartingScreen();
            singlePlayer();
        } else if (choice == 2) {
            showStartingScreen();
            multiPlayer();
        } else if (choice == 3) {
            showScores();
        } else {
            break;
        }
    }

    closegraph();
    return 0;
}
