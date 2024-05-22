#ifndef EXORKEYBOARDPANEL_H
#define EXORKEYBOARDPANEL_H

#include <QQuickView>

class ExorKeyboardPanel : public QQuickView
{
    Q_OBJECT
public:
    ExorKeyboardPanel();
    void activate(bool active);

private slots:
    void _update();

private:
    void showPanel();
    void hidePanel();
    void initGUI();
    QObject *m_inputPanel = NULL;
    QObject *m_exorImHandler = NULL;
};

#endif // EXORKEYBOARDPANEL_H
