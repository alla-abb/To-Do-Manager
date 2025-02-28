
#include "event.h"

Event::Event(const QString& name, const QDateTime& dateTime, const QString& location, const QString& description, const QString& type,int priority)
    : name(name), dateTime(dateTime), location(location), description(description), type(type), priority(priority) {}

QJsonObject Event::toJson() const {
    QJsonObject obj;
    obj["name"] = name;
    obj["dateTime"] = dateTime.toString(Qt::ISODate);
    obj["location"] = location;
    obj["description"] = description;
    obj["type"] = type;
    obj["priority"] =  priority;
    return obj;
}

Event Event::fromJson(const QJsonObject& obj) {
    return Event(
        obj["name"].toString(),
        QDateTime::fromString(obj["dateTime"].toString(), Qt::ISODate),
        obj["location"].toString(),
        obj["description"].toString(),
        obj["type"].toString(),
        obj["priority"].toInt()
    );
}
