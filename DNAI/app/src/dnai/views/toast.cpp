#include "dnai/toastermanagerservice.h"
#include "dnai/views/toast.h"

Toast::Toast(QQuickItem *item, ToasterManagerService *toasterManager, std::function<void ()> func)
    : QObject(NULL),
    m_msecDisable(5000),
    m_item(item),
    m_timer(new QTimer),
    m_toasterManager(toasterManager),
    m_func(func) {
    m_timer->start(m_msecDisable);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

Toast::~Toast() {
    qDebug() << "~Toast";
    delete m_item;
    if (m_timer) {
        delete m_timer;
    }
}

QQuickItem *Toast::item() {
    return (m_item);
}

QTimer &Toast::timer() {
    return *m_timer;
}

void Toast::onClickToast() {
    m_func();
    delete m_timer;
    m_timer = NULL;
    timeout();
}

void Toast::onEnterToast() {
  //  m_timer.stop();
    delete m_timer;
    m_timer = NULL;
}

void Toast::onExitToast() {
//    m_timer.
 //   m_timer.start(m_msecDisable);
    m_timer = new QTimer;
    m_timer->start(m_msecDisable);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void Toast::timeout() {
    m_toasterManager->timeout(this);
    delete this;
}

//void Toast::timeout() {

//}
