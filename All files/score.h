#ifndef SCORES_H
#define SCORES_H

extern int sp_totalMatches;
extern int sp_playerWins;
extern int sp_aiWins;

extern int mp_totalMatches;
extern int mp_player1Wins;
extern int mp_player2Wins;

void showScores();
void showSinglePlayerScores();
void showMultiplayerScores();

#endif

