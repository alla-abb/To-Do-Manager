
#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class Event {
public:
    Event(const QString& name, const QDateTime& dateTime, const QString& location, const QString& description, const QString& type, int priority);


    QJsonObject toJson() const;
    static Event fromJson(const QJsonObject& obj);
    QString name;
    QDateTime dateTime;
    QString location;
    QString description;
    QString type;
    int priority;

private:

};

#endif // EVENT_H
