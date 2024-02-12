#include <QFuture>
#include <QThread>
#include <QScrollArea>
#include <QtConcurrent>

#include "../qt/screen_qt.hpp"
#include "../qt/database_qt.hpp"

#include "window.hpp"
#include "imageview.hpp"

class DatabaseSaveWorker : public QObject {
    Q_OBJECT
public:
    DatabaseSaveWorker(DatabaseInterface *database = nullptr) : database_{database} {}

public slots:
    void AddImage(const QImage &image, double similarity) {
        const bool result = database_->AddImage(image, similarity);
        emit       ImageAdded(result);
    }

signals:
    void ImageAdded(bool result);

private:
    DatabaseInterface *database_ = nullptr;
};

Window::Window(QWidget *parent)
    : QMainWindow{parent},
      screen_interface_{new ScreenQt},
      tracker_{new Tracker{screen_interface_}},
      database_{new DatabaseQt},
      flow_layout_{new FlowLayout} {
    auto *widget = new QWidget{this};
    widget->setLayout(flow_layout_);

    auto *scroll_area = new QScrollArea{this};
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    setCentralWidget(scroll_area);

    // database
    database_->SetPath("./database.db");
    database_->Open();
    LoadLastImage();
    if (database_->GetCount() > 1) {
        LoadImage(database_->GetCount() - 2);
    }

    // tracker
    connect(tracker_, &Tracker::OnTick, [this](const QImage &image, double similarity) {
        DisplayResult(image, similarity);
    });
    connect(tracker_, &Tracker::OnTick, this, &Window::SaveImage);
    tracker_->Start();
}

Window::~Window() {
    database_->Close();
    tracker_->Stop();

    delete screen_interface_;
    delete database_;
}

void Window::LoadLastImage() {
    if (database_->GetCount() == 0) {
        return;
    }

    const Image image = database_->At(database_->GetCount() - 1);
    DisplayResult(image.image, image.similarity);
    tracker_->SetPreviousImage(image.image);
}

void Window::LoadImage(int index) {
    if (index < 1 || index >= database_->GetCount()) {
        unsetCursor();
        return;
    }
    setCursor(Qt::WaitCursor);

    QFuture<Image> future  = QtConcurrent::run(database_, &DatabaseInterface::At, index);
    auto          *watcher = new QFutureWatcher<Image>;
    QObject::connect(watcher, &QFutureWatcherBase::finished, [this, future]() {
        const auto image = future.result();
        DisplayResult(image.image, image.similarity, false);
    });
    QObject::connect(watcher, &QFutureWatcherBase::finished, watcher, &QObject::deleteLater);
    QObject::connect(watcher, &QFutureWatcherBase::finished, [this, index]() {
        LoadImage(index - 1);
    });
    watcher->setFuture(future);
}

void Window::DisplayResult(const QImage &image, double similarity, bool back) {
    auto *image_view = new ImageView{image, similarity};
    if (back) {
        flow_layout_->addWidget(image_view);
    } else {
        flow_layout_->prependWidget(image_view);
    }
}

void Window::SaveImage(const QImage &image, double similarity) {
    auto    *worker        = new DatabaseSaveWorker{database_};
    QThread *worker_thread = new QThread;
    worker->moveToThread(worker_thread);

    connect(worker_thread, &QThread::started, [worker, image, similarity]() {
        worker->AddImage(image, similarity);
    });
    connect(worker_thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker_thread, &QThread::finished, worker_thread, &QObject::deleteLater);
    connect(worker, &DatabaseSaveWorker::ImageAdded, worker_thread, &QThread::quit);

    worker_thread->start();
}

#include "window.moc"