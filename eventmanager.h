#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "event.h"
#include <QVector>
#include <QJsonArray>
#include <QFile>

class EventManager {
public:
    EventManager( QString filePath);

    void addEvent(Event& event);
    void saveEvents() const;
    void loadEvents();
    QString filePath;
    QVector<Event> events;



};

#endif // EVENTMANAGER_H
