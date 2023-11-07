#ifndef SCREEN_TRACKER_WINDOW_HPP
#define SCREEN_TRACKER_WINDOW_HPP

#include "tracker.hpp"
#include "database.hpp"

#include <QAbstractListModel>
#include <QQmlApplicationEngine>

class qml_elements : public QAbstractListModel {
    Q_OBJECT
public:
    qml_elements(QObject *parent = nullptr) : QAbstractListModel{parent} {};
    qml_elements(const QList<database::element> &ele, QObject *parent = nullptr) : elements_{ele}, QAbstractListModel{parent} {}

    enum MyRoles { PathRole = Qt::UserRole + 1, SimilarityRole };

    int                    rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant               data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool                   setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    void push_front(const database::element &ele);

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