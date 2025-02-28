#include "eventmanager.h"
#include <QJsonArray>
#include <QJsonDocument>

EventManager::EventManager(QString filePath) : filePath(filePath) {
    loadEvents();
}

void EventManager::addEvent(Event& event) {
    events.append(event);
    saveEvents();

}



void EventManager::saveEvents() const {
    QJsonArray jsonArray;
    for (const Event& event : events) {
        jsonArray.append(event.toJson());
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(jsonArray).toJson());
    }
}

void EventManager::loadEvents() {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray jsonArray = doc.array();
        events.clear();
        for (const QJsonValue& value : jsonArray) {
            events.append(Event::fromJson(value.toObject()));
        }
    }
}
