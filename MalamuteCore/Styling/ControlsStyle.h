#ifndef CONTROLSSTYLE_H
#define CONTROLSSTYLE_H

#include <QObject>
#include <QColor>
#include <QFont>

class ControlsStyle : public QObject
{
    Q_OBJECT
public:
    explicit ControlsStyle(QObject *parent = nullptr);

signals:

public slots:
    int radius() const;

    int borderWidth() const;
    QColor borderColor() const;

    QColor gradientTop() const;
    QColor gradientMid() const;
    QColor gradientBottom() const;

    QColor textColor() const;
    QFont textFont() const;

private:
    int m_radius = 16;

    int m_borderWidth = 6;
    QColor m_borderColor = QColor("black");

    QColor m_gradientTop = QColor("#232323");
    QColor m_gradientMid = QColor("#3b3b3b");
    QColor m_gradientBottom = QColor("#232323");

    QColor m_textColor = QColor("#d36f2c");
    QFont m_textFont;
};

#endif // CONTROLSSTYLE_H
