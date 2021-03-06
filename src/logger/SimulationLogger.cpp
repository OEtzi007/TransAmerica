/*
 * SimulationLogger.cpp
 *
 *  Created on: 17.01.2014
 *      Author: David
 */

#include "../../hdr/logger/SimulationLogger.h"
#include "../../hdr/logger/GameLogger.h"
#include "../../hdr/logger/RoundLogger.h"
#include "../../hdr/userinterface/DynamicState.h"
#include <cstdlib>

SimulationLogger::SimulationLogger(vector<AI*> playerList, Board& board,
		int numberOfGames, unsigned int seed) :
		playerList(playerList), gamesWon(0), gameList(
				vector<GameLogger*>(numberOfGames)), board(board), seed(seed), winnerPoints(
				calculateWinnerPoints()) {
	cout << "The seed of this run is: " << seed << endl;
	srand(seed);
	assert(numberOfGames == (int )gameList.size());
	bool twoPlayersSameColor = false;
	for (int i = 0; i < (int) playerList.size(); i++)
		for (int j = i + 1; j < (int) playerList.size(); j++)
			twoPlayersSameColor |= (playerList[i]->playerColor
					== playerList[j]->playerColor);
	assert(!twoPlayersSameColor);
	statLogger = new StatisticsLogger(this);
}

SimulationLogger::~SimulationLogger() {
	for (int i = 0; i < (int) gameList.size(); i++)
		delete gameList[i];
	for (int i = 0; i < (int) playerList.size(); i++)
		delete playerList[i];
	delete statLogger;
}

int gcd(int a, int b) {
	for (;;) {
		if (a == 0)
			return b;
		b %= a;
		if (b == 0)
			return a;
		a %= b;
	}
	cout << "gcd-error in SimulationLogger.cpp" << endl;
	return 0;
}

int lcm(int a, int b) {
	int temp = gcd(a, b);
	return temp ? (a / temp * b) : 0;
}

unsigned int SimulationLogger::calculateWinnerPoints() {
	int numberPlayers = playerList.size();
	unsigned int retValue = 1;
	for (int i = 2; i <= numberPlayers; i++)
		retValue = lcm(retValue, i);
	return retValue;
}

DynamicState SimulationLogger::calculateDynamicState(int game, int round,
		int move) const {
	DynamicState retValue(board);
	for (int i = 0; i < move; i++)
		gameList[game]->getRoundList()[round]->getMoveList()[i]->execute(
				retValue);
	if (move > 0)
		retValue.lastMove =
				gameList[game]->getRoundList()[round]->getMoveList()[move - 1];
	for (int i = 0; i < MAX_PLAYER; i++)
		if (gameList[game]->getRoundList()[round]->getPawnList()[i])
			retValue.addPawn(
					*gameList[game]->getRoundList()[round]->getPawnList()[i]);
	return retValue;
}

const City** SimulationLogger::getHandOfPlayer(PLAYERCOLOR playerColor,
		int game, int round) const {
	return gameList[game]->getRoundList()[round]->getPlayingCards()[playerColor];
}

Counter SimulationLogger::getPointsEndOfRound(int game, int round) const {
	Counter retValue(13);
	for (int i = 0; i <= round; i++)
		retValue -= gameList[game]->getRoundList()[i]->getLostPoints();
	return retValue;
}

int SimulationLogger::getDeadLine(int game, int round) const {
	if (round <= 1)
		return 0;
	else
		return gameList[game]->getDeadLine();
}

void SimulationLogger::fillStatisticsLogger() const {
	for (int i = 0; i < (int) gameList.size(); i++) {
		for (int j = 0; j < (int) gameList[i]->getRoundList().size(); j++) {
			const RoundLogger* actRound = gameList[i]->getRoundList()[j];
			const vector<AI*> actPlayerList = actRound->getPlayerList();
			statLogger->allLostPoints += actRound->getLostPoints();
			for (int k = 0; k < (int) actPlayerList.size(); k++)
				if (actRound->getPlayerStatus(actPlayerList[k]) != NOT_BANNED)
					statLogger->addEvent((i + 1) * 10 + j + 1, /*TODO create a proper pos. system*/
					actPlayerList[k]->playerColor,
							actRound->getPlayerStatus(actPlayerList[k]));
		}
	}
}

const Board& SimulationLogger::getBoard() const {
	return board;
}

const vector<GameLogger*>& SimulationLogger::getGameList() const {
	return gameList;
}

const Counter& SimulationLogger::getGamesWon() const {
	return gamesWon;
}

const vector<AI*>& SimulationLogger::getPlayerList() const {
	return playerList;
}

const unsigned int SimulationLogger::getSeed() const {
	return seed;
}

StatisticsLogger* SimulationLogger::getStatisticsLogger() const {
	return statLogger;
}

const unsigned int SimulationLogger::getWinnerPoints() const {
	return winnerPoints;
}
