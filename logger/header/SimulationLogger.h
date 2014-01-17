/*
 * SimulationLogger.h
 *
 *  Created on: 17.01.2014
 *      Author: David
 */

#ifndef SIMULATIONLOGGER_H_
#define SIMULATIONLOGGER_H_

#include<vector>
#include"AI.h"
#include"GameLogger.h"
#include"Counter.h"

class SimulationLogger {
public:
	SimulationLogger();
	virtual ~SimulationLogger();

	unsigned players;
	std::vector<AI> playerList;
	Counter gamesWon;

	unsigned games;
	std::vector<GameLogger> gameList;

	Board board;
};

#endif /* SIMULATIONLOGGER_H_ */