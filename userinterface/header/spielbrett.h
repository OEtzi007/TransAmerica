#ifndef SPIELBRETT_H
#define SPIELBRETT_H

#include <QWidget>
#include <QColor>
#include "../../game/header/Constants.h"
#include "../../game/header/State.h"
#include "window.h"

class Window;

class Spielbrett: public QWidget
{
  Q_OBJECT
public:
  Spielbrett(Window* parentalWindow);
  //QSize minimumSizeHint ()const;
  //QSize sizeHint ()const;

public slots:
  void zustandChanged(int);
  void drawCityChanged(bool);

protected:
  void paintEvent(QPaintEvent* event);

private:
  bool drawCity;
  Window* parentalWindow;


};

#endif // SPIELBRETT_H
