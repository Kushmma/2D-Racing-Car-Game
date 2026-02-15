#include <graphics.h>
#include <stdio.h>
#include "score.h"

// Global variables
int sp_totalMatches = 0;
int sp_playerWins = 0;
int sp_aiWins = 0;

int mp_totalMatches = 0;
int mp_player1Wins = 0;
int mp_player2Wins = 0;

// Single player
void showSinglePlayerScores() {
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(250, 150, (char*)"Single Player Scores");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    char buffer[100];
    sprintf(buffer, "Total Matches: %d", sp_totalMatches);
    outtextxy(260, 220, buffer);

    sprintf(buffer, "Won by Player: %d", sp_playerWins);
    outtextxy(260, 260, buffer);

    sprintf(buffer, "Won by Computer: %d", sp_aiWins);
    outtextxy(260, 300, buffer);

    outtextxy(260, 360, (char*)"Click anywhere to return...");
    while (!ismouseclick(WM_LBUTTONDOWN)) delay(50);
    clearmouseclick(WM_LBUTTONDOWN);
}

// Multiplayer
void showMultiplayerScores() {
    cleardevice();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
    outtextxy(250, 150, (char*)"Multiplayer Scores");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);

    char buffer[100];
    sprintf(buffer, "Total Matches: %d", mp_totalMatches);
    outtextxy(260, 220, buffer);

    sprintf(buffer, "Won by Player 1: %d", mp_player1Wins);
    outtextxy(260, 260, buffer);

    sprintf(buffer, "Won by Player 2: %d", mp_player2Wins);
    outtextxy(260, 300, buffer);

    outtextxy(260, 360, (char*)"Click anywhere to return...");
    while (!ismouseclick(WM_LBUTTONDOWN)) delay(50);
    clearmouseclick(WM_LBUTTONDOWN);
}

// Display
void showScores() {
    cleardevice();

    int singleBtnX = 250, singleBtnY = 250, singleBtnW = 600, singleBtnH = 60;
    int multiBtnX = 250, multiBtnY = 320, multiBtnW = 600, multiBtnH = 60;

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(260, 150, (char*)"Select Score Type");

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(singleBtnX, singleBtnY, singleBtnX + singleBtnW, singleBtnY + singleBtnH);
    rectangle(singleBtnX, singleBtnY, singleBtnX + singleBtnW, singleBtnY + singleBtnH);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    outtextxy(singleBtnX + 80, singleBtnY + 15, (char*)"Single Player Scores");

    bar(multiBtnX, multiBtnY, multiBtnX + multiBtnW, multiBtnY + multiBtnH);
    rectangle(multiBtnX, multiBtnY, multiBtnX + multiBtnW, multiBtnY + multiBtnH);
    outtextxy(multiBtnX + 80, multiBtnY + 15, (char*)"Multiplayer Scores");

    while (true) {
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int mx = mousex();
            int my = mousey();
            clearmouseclick(WM_LBUTTONDOWN);

            if (mx >= singleBtnX && mx <= singleBtnX + singleBtnW &&
                my >= singleBtnY && my <= singleBtnY + singleBtnH) {
                showSinglePlayerScores();
                break;
            }
            else if (mx >= multiBtnX && mx <= multiBtnX + multiBtnW &&
                     my >= multiBtnY && my <= multiBtnY + multiBtnH) {
                showMultiplayerScores();
                break;
            }
        }
        delay(50);
    }
}

