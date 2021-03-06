#ifndef STATISTICSLOGGER_H
#define STATISTICSLOGGER_H

#include "SimulationLogger.h"
#include <QList>
#include <QPair>
#include <QSet>
#include "../game/Constants.h"
#include "../game/Counter.h"

class SimulationLogger;


/*NOTE This is an raw layout for a class to log events. Those events should be displayed on
    the DataWidget of the GUI. NOTICE: There should only be one Event at one particular Position
*/
//TODO @Oetzi: Maybe you have to do some changes or improvements

class StatisticsLogger {
//    int numberOfGames
    QList <QPair <int, QPair <PLAYERCOLOR, BANNED_STATUS> > >positionList;
    QSet <int> positionsUsed;
//    const vector<AI*> playerlist;
    SimulationLogger* simLogger;
public:
     StatisticsLogger(SimulationLogger*);
     ~StatisticsLogger();
     Counter allLostPoints;

    void addEvent(int position, PLAYERCOLOR player, BANNED_STATUS status);
//    QPair<PLAYERCOLOR, BANNED_STATUS> getEventAtPosition (int position);
//    QList<QPair<int,BANNED_STATUS> > getEventsFromPlayer (PLAYERCOLOR player);
//    QList<QPair<int, PLAYERCOLOR> > getEventsFromStatus (BANNED_STATUS);
    QList <QPair <int, QPair <PLAYERCOLOR, BANNED_STATUS> > >& getPositionList();
    QString playercolorToQString(PLAYERCOLOR);
    unsigned short getNumberOfEvents(){
        return positionsUsed.size();
    }
//    void setPlayerlist(const vector<AI*> & list);
//    void setGamesWon(const Counter* counter);
};

#endif // STATISTICSLOGGER_H
