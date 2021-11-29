#ifndef WIRESTYLE_H
#define WIRESTYLE_H

#include <QObject>
#include <QColor>

class WireStyle : public QObject
{
    Q_OBJECT
public:
    explicit WireStyle(QObject *parent = nullptr);

signals:

public slots:
    double lineWidth();
    double endpointSize();

    QColor selectedColor();
    double selectedLineWidth();

    double transmittingLineWidth();
    double transmittingLightenFactor();
    int transmittingAnimationTime();

private:
    double m_lineWidth = 4.0;
    double m_endpointSize = 15;

    QColor m_selectedColor = QColor("darkblue");
    double m_selectedLineWidth = 12.0;

    double m_transmittingLineWidth = 7.5;
    double m_transmittingLightenFactor = 1.3;
    int m_transmittingAnimationTime = 300;
};

#endif // WIRESTYLE_H
