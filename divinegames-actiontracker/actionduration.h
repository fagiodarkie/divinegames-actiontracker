#ifndef ACTIONDURATION_H
#define ACTIONDURATION_H

#include <QMap>
#include <QString>

namespace action {
    enum duration {
      PRESTO = 20,
      ALLEGRO = 22,
      MODERATO = 25,
      ANDANTE = 28,
      LENTO = 30
    };

    static const QList<duration> DURATIONS = {
      PRESTO,
      ALLEGRO,
      MODERATO,
      ANDANTE,
      LENTO
    };

    static const QMap<duration, QString> DURATIONNAME = {
      { PRESTO  , "Presto" },
      { ALLEGRO , "Allegro" },
      { MODERATO, "Moderato" },
      { ANDANTE , "Andante" },
      { LENTO   , "Lento" }
    };
}

#endif // ACTIONDURATION_H
