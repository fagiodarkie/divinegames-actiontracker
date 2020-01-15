#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

#include "actionduration.h"

class Player
{
public:
  Player(const QString& name = "", unsigned int start = 0, unsigned int wits = 0, unsigned int dex = 0, unsigned int bonus = 0)
    : _name(name), _turn(start), _bonus(bonus), _wits(wits), _dex(dex) {}

  void operator=(const Player& o) {
    _name = o._name;
    _turn = o._turn;
    _bonus = o._bonus;
  }

  bool operator==(const Player& o) {
    return o._name == _name
        && o._turn == _turn
        && o._bonus == _bonus
        && o._dex  == _dex
        && o._wits == _wits;
  }

  QString name() const { return _name; }
  unsigned int nextTurn() const { return _turn; }
  unsigned int bonus() const { return _bonus; }

  void action(action::duration ticks) { _turn += (ticks - _bonus); }

  bool operator< (const Player& other)
  {
    return (_turn < other._turn)
        || (_turn == other._turn && _wits > other._wits)
        || (_turn == other._turn && _wits == other._wits && _dex > other._dex);
  }

private:

  QString _name;
  unsigned int _turn, _bonus, _wits, _dex;
};

#endif // PLAYER_H
