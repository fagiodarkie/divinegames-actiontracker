#ifndef TURNMODEL_H
#define TURNMODEL_H

#include <QObject>
#include <QVector>
#include <QRandomGenerator>
#include "player.h"


class TurnModel : public QObject {

  Q_OBJECT

public:
  TurnModel() : _currentTurn(0), _current(-1)
  {
    connect (this, &TurnModel::nextTurn, this, &TurnModel::refreshCurrentPlayer);
  }

  void addPlayer(const Player& player)
  {
    _players.append(player);
    refreshCurrentPlayer();
    refreshTurn();
  }

  unsigned int currentTurn() const { return _currentTurn; }

  void refreshTurn()
  {
    _currentTurn = _current < 0 ? 0 : current().nextTurn();
    for (auto player: _players)
      if (_currentTurn == 0 || player.nextTurn() < _currentTurn)
        {
          _currentTurn = player.nextTurn();
        }
  }

  QVector<Player> players() const { return _players; }

  Player currentPlayer() const { return current(); }

  void refreshCurrentPlayer()
  {
    if (_players.empty())
      return;

    _current = 0;
    for (int i = 1; i < _players.size(); ++i)
      {
        if (player(i) < current())
          {
            _current = i;
          }
        else if (!(current() < player(i)) && !(current() == player(i)))
          {
            // equals: let a d10 decide.
            int currentDie = (qrand() % 10) + 1,
                playerDie = (qrand() % 10) + 1;
            while (currentDie == playerDie)
              {
                currentDie = (qrand() % 10) + 1;
                playerDie = (qrand() % 10) + 1;
              }
            if (playerDie > currentDie)
              _current = i;
          }
      }
  }

  QVector<Player> nextPlayers() const
  {
    if (_players.size() == 1)
      return _players;

    unsigned int minDiff = player(0).nextTurn() - current().nextTurn();
    for (auto player: _players)
      if ((player.nextTurn() - current().nextTurn()) < minDiff)
        {
          minDiff = player.nextTurn() - current().nextTurn();
        }

    unsigned int next_tick = current().nextTurn() + minDiff;

    QVector<Player> result;
    for (auto player: _players)
      if (player.nextTurn() == next_tick)
        result.push_back(player);

    return result;
  }

  void actionForCurrentPlayer(action::duration action)
  {
    current().action(action);
    refreshCurrentPlayer();
    refreshTurn();
    emit actionTaken();
  }

  void removeCurrent()
  {
    _players.remove(_current);
    if (_players.empty())
      return;

    refreshCurrentPlayer();
    refreshTurn();
  }

signals:
  void actionTaken();

private slots:
  void nextTurn()
  {
    refreshCurrentPlayer();
  }

private:

  inline Player& current() {
    return _players[_current];
  }

  inline Player current() const {
    return _players[_current];
  }

  inline Player& player(int i)
  {
    return _players[i];
  }

  inline Player player(int i) const
  {
    return _players[i];
  }

  int _current;
  unsigned int _currentTurn;
  QVector<Player> _players;

};

#endif // TURNMODEL_H
