
#include "../header/uiexec.h"
#include "../../game/header/AI.h"
#include "../../ai/testKI.h"
#include "../../game/header/Simulation.h"
#include <iostream>
#include <cassert>

UIEXEC::UIEXEC() {
}

void UIEXEC::simulateSimulation(int games) {
    /**
     window implementation
     */
    board = new Board;
    std::vector<AI*> players;
    players.push_back(new testKI(P_YELLOW));
    players.push_back(new testKI(P_BLUE));
    players.push_back(new testKI(P_ORANGE));

    simulationLogger = new SimulationLogger(players, *board, games);
    simulation = new Simulation(simulationLogger);
    simulation->run();

}
