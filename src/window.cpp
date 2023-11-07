#include "window.hpp"

#include <QWidget>
#include <QQmlContext>
#include <QQmlApplicationEngine>

// qml_elements

int qml_elements::rowCount(const QModelIndex &parent) const {
    return elements_.size();
}

QVariant qml_elements::data(const QModelIndex &index, int role) const {
    if (!hasIndex(index.row(), index.column(), index.parent())) {
        return {};
    }

    const database::element &ele = elements_[index.row()];
    switch (role) {
    case PathRole:
        return ele.path;
    case SimilarityRole:
        return ele.similarity * 100;
    default:
        return {};
    }
}

bool qml_elements::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid()) {
        return false;
    }

    database::element &ele = elements_[index.row()];
    switch (role) {
    case PathRole:
        ele.path = value.toString();
        break;
    case SimilarityRole:
        ele.similarity = value.toDouble();
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

QHash<int, QByteArray> qml_elements::roleNames() const {
    return {{PathRole, "path"}, {SimilarityRole, "similarity"}};
}

void qml_elements::push_front(const database::element &ele) {
    beginInsertRows(QModelIndex{}, 0, 0);
    elements_.push_front(ele);
    endInsertRows();
}

// window

window::window(QWidget *parent) : QQmlApplicationEngine{parent}, tracker_{new tracker{this}} {
    qmlRegisterType<qml_elements>("QMLElements", 1, 0, "QMLElement");

    database                 db{this};
    QList<database::element> ele = db.elements(true);
    elements_                    = new qml_elements{ele, this};

    rootContext()->setContextProperty("elements", elements_);
    load(QUrl::fromLocalFile("window.qml"));

    connect(tracker_, &tracker::on_save, this, &window::display);
}

void window::display(const database::element &ele) {
    elements_->push_front(ele);
}