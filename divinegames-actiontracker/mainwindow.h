#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "turnmodel.h"
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  TurnModel _model;

  bool displayingTurns;
  QPushButton *swapScreensBtn, *addPlayer, *kill;
  QSpinBox *turn, *wits, *dex, *bonus;
  QLineEdit *playerName;

  QMap<QPushButton*, QString> buttonLabels;
  QList<QPushButton*> buttons;

  void swapScreens();
  void initNewPlayer();
  void initTurnScreen();
  void drawNewPlayer();
  void drawTurnScreen();

  void generatePlayer();
  void validateAddPlayer();
  void actionChosen();
  void killCurrent();

signals:
  void playerActionChosen(action::duration duration);

};

#endif // MAINWINDOW_H
