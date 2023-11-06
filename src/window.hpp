#ifndef SCREEN_TRACKER_WINDOW_HPP
#define SCREEN_TRACKER_WINDOW_HPP

#include "tracker.hpp"
#include "database.hpp"

#include <QQmlApplicationEngine>

class qml_elements : public QObject {
    Q_OBJECT
public:
    qml_elements(QObject *parent = nullptr) : QObject{parent} {};
    qml_elements(const QList<database::element> &ele, QObject *parent = nullptr) : elements_{ele}, QObject{parent} {}

    Q_INVOKABLE QString path(int index) const {
        return elements_.at(index).path;
    }
    Q_INVOKABLE double similarity(int index) const {
        return elements_.at(index).similarity;
    }
    Q_INVOKABLE int length() const {
        return elements_.length();
    }

    void push_back(const database::element &ele) {
        elements_.push_back(ele);
    }

private:
    QList<database::element> elements_;
};

class window : public QQmlApplicationEngine {
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);

private:
    void display(const database::element &ele);

    qml_elements *elements_;
    tracker      *tracker_;
};

#endif // SCREEN_TRACKER_WINDOW_HPP