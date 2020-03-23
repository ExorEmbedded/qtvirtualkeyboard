#ifndef EXORKEYBOARDPANEL_H
#define EXORKEYBOARDPANEL_H

#include <QQuickView>

class ExorKeyboardPanel : public QQuickView
{
    Q_OBJECT
public:
    ExorKeyboardPanel();
    void activate(bool active);

private:
    void showPanel();
    void hidePanel();
    void initGUI();
    QObject *m_inputPanel = NULL;
};

#endif // EXORKEYBOARDPANEL_H