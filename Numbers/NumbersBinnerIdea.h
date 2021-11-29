#ifndef NUMBERSBINNERIDEA_H
#define NUMBERSBINNERIDEA_H

#include "../MalamuteCore/InheritedClasses/Idea.h"
#include "../DataTypes/NumbersDataType.h"
#include <QPointF>

struct NumbersBinnerAttributes : public IdeaAttributes
{
    NumbersBinnerAttributes();
};

class NumbersBinnerIdea : public Idea
{
    Q_OBJECT

    Q_PROPERTY(QList<double> gridBounds READ gridBounds NOTIFY gridBoundsChanged)
    Q_PROPERTY(bool autoFitGrid READ autoFitGrid WRITE setAutoFitGrid NOTIFY autoFitGridChanged)

    Q_PROPERTY(QList<double> binBounds READ binBounds WRITE setBinBounds NOTIFY binBoundsChanged)
    Q_PROPERTY(int numBins READ numBins WRITE setNumBins NOTIFY numBinsChanged)
    Q_PROPERTY(double lowBinBoundary READ lowBinBoundary WRITE setLowBinBoundary NOTIFY lowBinBoundaryChanged)
    Q_PROPERTY(double upBinBoundary READ upBinBoundary WRITE setUpBinBoundary NOTIFY upBinBoundaryChanged)

    Q_PROPERTY(double animationSpeed READ animationSpeed WRITE setAnimationSpeed NOTIFY animationSpeedChanged)

public:
    explicit NumbersBinnerIdea();
    const IdeaAttributes attributes() override;

    QJsonObject save() const override;
    void load(const QJsonObject &) override;

    void onNewDataIn(std::shared_ptr<DataType> dataType, PlugNumber plugNumber) override;

    void resetDisplay() override;

    QList<double> gridBounds() const;
    bool autoFitGrid() const;

    QList<double> binBounds() const;
    int numBins() const;
    double lowBinBoundary() const;
    double upBinBoundary() const;

    double animationSpeed() const;

signals:
    void newDots(int numDots);

    void gridBoundsChanged(QList<double> gridBounds);
    void autoFitGridChanged(bool autoFitGrid);

    void binBoundsChanged(QList<double> bounds);
    void numBinsChanged(int numBins);
    void lowBinBoundaryChanged(double lowBinBoundary);
    void upBinBoundaryChanged(double upBinBoundary);

    void animationSpeedChanged(double animationSpeed);


public slots:
    void setGridBound(double bound, int index);
    void setAutoFitGrid(bool autoFitGrid);

    void setBinBounds(QList<double> bounds);
    void sanitizeInputBounds(QString input);
    void setBoundsByButton();
    void setNumBins(int numBins);
    void setLowBinBoundary(double lowBinBoundary);
    void setUpBinBoundary(double upBinBoundary);

    double getInitialPositionForDot(int i);
    QPointF getFinalPositionForDot(int i);
    double getPositionForBoundLine(int i);
    double getBinWidth(int i);
    double getBinHeight(int i);

    void replay();
    void setAnimationSpeed(double animationSpeed);

private:
    std::weak_ptr<NumbersDataType> m_nums;
    void validate();
    void compute();
    bool checkSetGridBound(int index, double bound);

    QList<double> m_gridBounds;
    bool m_autoFitGrid;

    QList<double> m_binBounds;
    QList<double> m_dotInitialPositions;
    QList<QPointF> m_dotFinalPositions;
    QList<int> m_binWidths;
    QList<double> m_binHeights;

    int m_numBins;
    double m_lowBinBoundary;
    double m_upBinBoundary;

    double m_animationSpeed;
};

#endif // NUMBERSBINNERIDEA_H
