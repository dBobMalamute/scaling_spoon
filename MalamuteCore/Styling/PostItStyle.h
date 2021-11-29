#ifndef POSTITSTYLE_H
#define POSTITSTYLE_H

#include <QObject>
#include <QColor>
#include <QFont>

class PostItStyle : public QObject
{
    Q_OBJECT
public:
    explicit PostItStyle(QObject *parent = nullptr);

signals:

public slots:
    QColor gradientTop() const;
    QColor gradientBottom() const;

    int borderWidth() const;
    QColor borderColor() const;
    QColor borderColorSelected() const;

    int glowRadius() const;
    double glowSpread() const;
    QColor glowColor() const;

    QColor titleColor() const;
    QFont titleFont() const;

    QColor shortMessageColor() const;
    QFont shortMessageFont() const;

    QColor longMessageColor() const;
    QFont longMessageFont() const;

private:
    QColor m_gradientTop = QColor("#ffcc00");
    QColor m_gradientBottom = QColor("#ffeeaa");

    int m_borderWidth = 4;
    QColor m_borderColor = QColor("#936416");
    QColor m_borderColorSelected = QColor("#cc5708");

    int m_glowRadius = 6;
    double m_glowSpread = 0.4;
    QColor m_glowColor = QColor("#323232");

    QColor m_titleColor = QColor("black");
    QFont m_titleFont;

    QColor m_shortMessageColor = QColor("black");
    QFont m_shortMessageFont;

    QColor m_longMessageColor = QColor("black");
    QFont m_longMessageFont;
};

#endif // POSTITSTYLE_H
