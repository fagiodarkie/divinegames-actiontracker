#include "mainwindow.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), displayingTurns(false)
{
  swapScreensBtn = new QPushButton;
  connect(swapScreensBtn, &QPushButton::clicked, this, &MainWindow::swapScreens);
  connect(this, &MainWindow::playerActionChosen, &_model, &TurnModel::actionForCurrentPlayer);
  connect(&_model, &TurnModel::actionTaken, this, &MainWindow::drawTurnScreen);

  initNewPlayer();
}


void MainWindow::swapScreens()
{
  if (displayingTurns) initNewPlayer();
  else initTurnScreen();

  displayingTurns = !displayingTurns;
}

void MainWindow::killCurrent()
{
  _model.removeCurrent();
  if (_model.players().size() > 0)
    drawTurnScreen();
  else
    swapScreens();
}

void MainWindow::initNewPlayer()
{
  addPlayer = new QPushButton("Add Player!");
  connect(addPlayer, &QPushButton::clicked, this, &MainWindow::generatePlayer);

  turn    = new QSpinBox;
  wits    = new QSpinBox;
  dex     = new QSpinBox;
  bonus   = new QSpinBox;
  playerName = new QLineEdit;

  for (QSpinBox* spin: {turn ,
       wits ,
       dex  ,
       bonus})
    {
      spin->setStyleSheet("QSpinBox::up-button { width: 16px; height: 16px; }"
                          "QSpinBox::down-button { width: 16px; height: 16px; }"
                          "QSpinBox { height: 32px; width: 80px; }");
    }

  turn->setMinimum(1);
  wits->setMinimum(0);
  dex ->setMinimum(0);
  bonus->setMinimum(0);
  bonus->setMaximum(13);

  connect(playerName, &QLineEdit::textEdited, this, &MainWindow::validateAddPlayer);

  drawNewPlayer();
}

void MainWindow::drawNewPlayer()
{
  QVBoxLayout *outer = new QVBoxLayout, *players = new QVBoxLayout;

  for (auto player: _model.players())
    players->addWidget(new QLabel(player.name() + " (tick " + QString::number(player.nextTurn()) + ")"));

  QWidget* playersWidget = new QWidget;
  playersWidget->setLayout(players);
  QScrollArea *playerScroll = new QScrollArea;
  playerScroll->setWidget(playersWidget);

  QFormLayout *generator = new QFormLayout;
  playerName->setText("");
  playerName->setFocus();
  playerName->setInputMethodHints(Qt::ImhNoAutoUppercase);
  wits->setValue(1);
  dex->setValue(1);
  bonus->setValue(0);
  turn->setValue(_model.currentTurn());
  generator->addRow("New Player Name:", playerName);
  generator->addRow("Starting turn",    turn);
  generator->addRow("New Player Wits:", wits);
  generator->addRow("New Player Dexterity:", dex);
  generator->addRow("New Player Speed Bonus:", bonus);
  QWidget *playerGeneratorWidget = new QWidget;
  playerGeneratorWidget->setLayout(generator);

  swapScreensBtn->setText("To Turnation!");
  swapScreensBtn->setEnabled(!_model.players().empty());
  QVBoxLayout *buttonLayout = new QVBoxLayout;
  buttonLayout->addWidget(addPlayer);
  buttonLayout->addWidget(swapScreensBtn);
  QWidget* buttonsWidget = new QWidget;
  buttonsWidget->setLayout(buttonLayout);

  outer->addWidget(playerGeneratorWidget);
  outer->addWidget(playersWidget);
  outer->addWidget(buttonsWidget);
  outer->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

  QWidget *newPlayerWidget = new QWidget;
  newPlayerWidget->setLayout(outer);
  setCentralWidget(newPlayerWidget);
  validateAddPlayer();
}

void MainWindow::initTurnScreen()
{
  kill = new QPushButton("Kill current Player!");
  buttonLabels.clear();
  buttons.clear();
  for (auto tempo: action::DURATIONS)
    {
      QPushButton *button = new QPushButton;
      button->setProperty("tempo", tempo);
      connect (button, &QPushButton::clicked, this, &MainWindow::actionChosen);
      buttonLabels[button] = action::DURATIONNAME.value(tempo) + " (%1 ticks)";
      buttons.push_back(button);
    }

  connect(kill, &QPushButton::clicked, this, &MainWindow::killCurrent);

  drawTurnScreen();
}

void MainWindow::drawTurnScreen()
{

  QVBoxLayout *outer = new QVBoxLayout, *players = new QVBoxLayout, *actionButtons = new QVBoxLayout;

  actionButtons->addWidget(new QLabel("Tick #" + QString::number(_model.currentTurn())));
  actionButtons->addWidget(new QLabel(_model.currentPlayer().name() + " acts!"));

  for (auto button: buttons)
    {
      unsigned int ticks = button->property("tempo").toInt();
      button->setText(buttonLabels[button].arg(QString::number(ticks - _model.currentPlayer().bonus())));
      actionButtons->addWidget(button);
    }
  actionButtons->addWidget(kill);
  QWidget* currentTurnWidget = new QWidget;
  currentTurnWidget->setLayout(actionButtons);

  for (auto player: _model.players())
    players->addWidget(new QLabel(player.name() + " (tick " + QString::number(player.nextTurn()) + ")"));

  QWidget* playersWidget = new QWidget;
  playersWidget->setLayout(players);
  QScrollArea *playerScroll = new QScrollArea;
  playerScroll->setWidget(playersWidget);

  swapScreensBtn->setText("Add new Player!");
  QVBoxLayout *buttonLayout = new QVBoxLayout;
  buttonLayout->addWidget(swapScreensBtn);
  QWidget* buttonsWidget = new QWidget;
  buttonsWidget->setLayout(buttonLayout);

  outer->addWidget(currentTurnWidget);
  outer->addWidget(playersWidget);
  outer->addWidget(buttonsWidget);
  outer->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

  QWidget *turnsWidget = new QWidget;
  turnsWidget->setLayout(outer);
  setCentralWidget(turnsWidget);
}

void MainWindow::generatePlayer()
{
  Player newPlayer(playerName->text().trimmed(), turn->value(), wits->value(), dex->value(), bonus->value());
  _model.addPlayer(newPlayer);
  drawNewPlayer();
}

void MainWindow::validateAddPlayer()
{
  auto name = playerName->text().trimmed();
  auto players = _model.players();
  bool alreadyThere = std::accumulate(players.begin(), players.end(), false, [name](bool there, Player p) {
    return there || (p.name() == name);
  });

  addPlayer->setEnabled(!name.isEmpty() && !alreadyThere);
}

void MainWindow::actionChosen()
{
  unsigned int tempo = sender()->property("tempo").toInt();
  emit playerActionChosen(static_cast<action::duration>(tempo));
}


MainWindow::~MainWindow() { }
