#include "window.hpp"

#include <QWidget>
#include <QQmlContext>
#include <QQmlApplicationEngine>

window::window(QWidget *parent) : QQmlApplicationEngine{parent}, tracker_{new tracker{this}} {
    qmlRegisterType<qml_elements>("QMLElements", 1, 0, "QMLElement");

    database db{this};
    elements_ = new qml_elements{db.elements(), this};

    rootContext()->setContextProperty("elements", elements_);
    load(QUrl::fromLocalFile("window.qml"));

    connect(tracker_, &tracker::on_save, this, &window::display);
}

void window::display(const database::element &ele) {
    elements_->push_back(ele);
    rootContext()->setContextProperty("elements", elements_);
}