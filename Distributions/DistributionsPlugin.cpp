#include "DistributionsPlugin.h"

#include "NegativeBinomialDistributionIdea.h"
#include "BinomialDistributionIdea.h"
#include "GeometricDistributionIdea.h"
#include "PoissonDistributionIdea.h"
#include "BetaDistributionIdea.h"
#include "LogNormalDistributionIdea.h"
#include "TriangleDistributionIdea.h"
#include "UniformDistributionIdea.h"
#include "ExponentialDistributionIdea.h"
#include "GaussianDistributionIdea.h"
#include "GaussianPPDistributionIdea.h"
#include "PowerLawDistributionIdea.h"
#include "DistributionDisplayIdea.h"
#include "DistributionSamplerIdea.h"
#include "MeanFromDistributionSamplerIdea.h"
#include "DistributionWeighterIdea.h"
#include "DistributionBinnerIdea.h"
#include "DistributionMeanIdea.h"
#include "DistributionStandardDeviationIdea.h"
#include "StdevFromDistributionSamplerIdea.h"
#include "DistributionVarianceIdea.h"
#include "VarianceFromDistributionSamplerIdea.h"
#include "StdevFromDistributionSamplerFixedMeanIdea.h"
#include "VarianceFromDistributionSamplerFixedMeanIdea.h"
#include "StudentTDistributionIdea.h"

#include "../DataTypes/NumberDataType.h"
#include "../DataTypes/DistributionDataType.h"

#include <QQmlEngine>

QString DistributionsPlugin::PluginName()
{
    return "Distribution";
}

QStringList DistributionsPlugin::authors()
{
    return QStringList("dBob");
}

QString DistributionsPlugin::PluginDescription()
{
    return "This plugin holds mathematical distributions";
}

void DistributionsPlugin::registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    ideaRegistry->registerIdea<BetaDistributionIdea>();
    ideaRegistry->registerIdea<LogNormalDistributionIdea>();
    ideaRegistry->registerIdea<TriangleDistributionIdea>();
    ideaRegistry->registerIdea<UniformDistributionIdea>();
    ideaRegistry->registerIdea<ExponentialDistributionIdea>();
    ideaRegistry->registerIdea<GaussianDistributionIdea>();
    ideaRegistry->registerIdea<PowerLawDistributionIdea>();
    ideaRegistry->registerIdea<GaussianPPDistributionIdea>();
    ideaRegistry->registerIdea<DistributionDisplayIdea>();
    ideaRegistry->registerIdea<DistributionWeighterIdea>();
    ideaRegistry->registerIdea<DistributionSamplerIdea>();
    ideaRegistry->registerIdea<MeanFromDistributionSamplerIdea>();
    ideaRegistry->registerIdea<DistributionBinnerIdea>();
    ideaRegistry->registerIdea<DistributionMeanIdea>();
    ideaRegistry->registerIdea<DistributionStandardDeviationIdea>();
    ideaRegistry->registerIdea<DistributionVarianceIdea>();
    ideaRegistry->registerIdea<NegativeBinomialDistributionIdea>();
    ideaRegistry->registerIdea<BinomialDistributionIdea>();
    ideaRegistry->registerIdea<GeometricDistributionIdea>();
    ideaRegistry->registerIdea<PoissonDistributionIdea>();
    ideaRegistry->registerIdea<StdevFromDistributionSamplerIdea>();
    ideaRegistry->registerIdea<VarianceFromDistributionSamplerIdea>();
    ideaRegistry->registerIdea<StdevFromDistributionSamplerFixedMeanIdea>();
    ideaRegistry->registerIdea<VarianceFromDistributionSamplerFixedMeanIdea>();
    ideaRegistry->registerIdea<StudentTDistributionIdea>();

    qmlRegisterType(QUrl("qrc:/QML/DistributionDisplay.qml"),"com.distributions.dists", 1, 0, "DistributionDisplay");
}

void DistributionsPlugin::registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry)
{
    dataTypeRegistry->registerDataTypeAttributes<NumberDataTypeAttributes>();
    dataTypeRegistry->registerDataTypeAttributes<DistributionDataTypeAttributes>();
    dataTypeRegistry->registerDataTypeAttributes<NumbersDataTypeAttributes>();
}

void DistributionsPlugin::registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry)
{
    QStringList distributionList;
    distributionList << Magic<DistributionDisplayAttributes>::cast().listName();
    distributionList << Magic<GaussianDistributionAttributes>::cast().listName();
    distributionList << Magic<ExponentialDistributionAttributes>::cast().listName();
    distributionList << Magic<UniformDistributionAttributes>::cast().listName();
    distributionList << Magic<TriangleDistributionAttributes>::cast().listName();
    distributionList << Magic<LogNormalDistributionAttributes>::cast().listName();
    distributionList << Magic<BetaDistributionAttributes>::cast().listName();
    //orderList << Magic<PowerLawDistributionAttributes>::cast().listName();
    distributionList << Magic<GaussianPPDistributionAttributes>::cast().listName();
    distributionList << Magic<StudentTDistributionAttributes>::cast().listName();

    distributionList << Magic<PoissonDistributionAttributes>::cast().listName();
    distributionList << Magic<BinomialDistributionAttributes>::cast().listName();
    distributionList << Magic<NegativeBinomialDistributionAttributes>::cast().listName();
    distributionList << Magic<GeometricDistributionAttributes>::cast().listName();
    toolbarRegistry->registerToolbar("Distributions", distributionList);

    QStringList distributionUtilitiesList;

    distributionUtilitiesList << Magic<DistributionMeanAttributes>::cast().listName();
    distributionUtilitiesList << Magic<DistributionStandardDeviationAttributes>::cast().listName();
    distributionUtilitiesList << Magic<DistributionVarianceAttributes>::cast().listName();
    distributionUtilitiesList << Magic<DistributionBinnerAttributes>::cast().listName();
    distributionUtilitiesList << Magic<DistributionSamplerAttributes>::cast().listName();
    distributionUtilitiesList << Magic<DistributionWeighterAttributes>::cast().listName();
    distributionUtilitiesList << Magic<MeanFromDistributionSamplerAttributes>::cast().listName();
    distributionUtilitiesList << Magic<StdevFromDistributionSamplerAttributes>::cast().listName();
    distributionUtilitiesList << Magic<VarianceFromDistributionSamplerAttributes>::cast().listName();
    distributionUtilitiesList << Magic<StdevFromDistributionSamplerFixedMeanAttributes>::cast().listName();
    distributionUtilitiesList << Magic<VarianceFromDistributionSamplerFixedMeanAttributes>::cast().listName();

    toolbarRegistry->registerToolbar("Distribution Utilities", distributionUtilitiesList);
}
