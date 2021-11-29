#ifndef CORKBOARDSTYLE_H
#define CORKBOARDSTYLE_H

#include <QObject>
#include <QColor>

class CorkboardStyle : public QObject
{
    Q_OBJECT
public:
    explicit CorkboardStyle(QObject *parent = nullptr);

signals:

public slots:
    QColor selectionRectangleColor() const;
    int selectionRectangleWidth() const;
    int selectionRectangleRadius() const;
    QString backgroundImagePath() const;
    QColor backgroundTintColor() const;

private:
    QColor m_selectionRectangleColor = QColor("#cc5708");
    int m_selectionRectangleWidth = 4;
    int m_selectionRectangleRadius = 4;

    QString m_backgroundImagePath = "qrc:/Images/CorkboardImage.jpg";
    QColor m_backgroundTintColor = "#25204020";
};

#endif // CORKBOARDSTYLE_H
