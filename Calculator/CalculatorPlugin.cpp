
#include "CalculatorPlugin.h"

#include "AdditionIdea.h"
#include "SubtractionIdea.h"
#include "MultiplicationIdea.h"
#include "DivisionIdea.h"
#include "NumberDisplayIdea.h"
#include "NumberInputIdea.h"
#include "SquaredIdea.h"
#include "SquareRootIdea.h"
#include "ReciprocalIdea.h"
#include "LogarithmBase10Idea.h"
#include "LogarithmBaseEIdea.h"
#include "LogarithmBase2Idea.h"
#include "ExponentialIdea.h"
#include "PowerOfTwoIdea.h"
#include "PowerOfTenIdea.h"
#include "PowerIdea.h"
#include "SineIdea.h"
#include "CosineIdea.h"
#include "TangentIdea.h"
#include "AbsoluteValueIdea.h"
#include "FloorIdea.h"
#include "CeilingIdea.h"
#include "InverseSineIdea.h"
#include "InverseCosineIdea.h"
#include "InverseTangentIdea.h"
#include "MathematicalConstantsIdea.h"
#include "LabeledNumberInputIdea.h"
#include "LabeledNumberDisplayIdea.h"
#include "NumberWeighterIdea.h"
#include "SliderNumberInputIdea.h"
#include "FractionInputIdea.h"
#include "SquareRootFractionInputIdea.h"

QString CalculatorPlugin::PluginName()
{
    return "Calculator";
}

QString CalculatorPlugin::PluginDescription()
{
    return "This library is designed to look like a calculator";
}

QStringList CalculatorPlugin::authors()
{
    return QStringList("dBob");
}

void CalculatorPlugin::registerIdeas(std::shared_ptr<IdeaRegistry> ideaRegistry)
{
    ideaRegistry->registerIdea<SliderNumberInputIdea>();
    ideaRegistry->registerIdea<NumberWeighterIdea>();
    ideaRegistry->registerIdea<LabeledNumberDisplayIdea>();
    ideaRegistry->registerIdea<LabeledNumberInputIdea>();
    ideaRegistry->registerIdea<MathematicalConstantsIdea>();
    ideaRegistry->registerIdea<InverseTangentIdea>();
    ideaRegistry->registerIdea<InverseCosineIdea>();
    ideaRegistry->registerIdea<InverseSineIdea>();
    ideaRegistry->registerIdea<CeilingIdea>();
    ideaRegistry->registerIdea<FloorIdea>();
    ideaRegistry->registerIdea<AbsoluteValueIdea>();
    ideaRegistry->registerIdea<TangentIdea>();
    ideaRegistry->registerIdea<CosineIdea>();
    ideaRegistry->registerIdea<SineIdea>();
    ideaRegistry->registerIdea<PowerIdea>();
    ideaRegistry->registerIdea<ExponentialIdea>();
    ideaRegistry->registerIdea<PowerOfTwoIdea>();
    ideaRegistry->registerIdea<PowerOfTenIdea>();
    ideaRegistry->registerIdea<LogarithmBase2Idea>();
    ideaRegistry->registerIdea<LogarithmBaseEIdea>();
    ideaRegistry->registerIdea<LogarithmBase10Idea>();
    ideaRegistry->registerIdea<ReciprocalIdea>();
    ideaRegistry->registerIdea<SquareRootIdea>();
    ideaRegistry->registerIdea<SquaredIdea>();
    ideaRegistry->registerIdea<DivisionIdea>();
    ideaRegistry->registerIdea<MultiplicationIdea>();
    ideaRegistry->registerIdea<SubtractionIdea>();
    ideaRegistry->registerIdea<AdditionIdea>();
    ideaRegistry->registerIdea<NumberDisplayIdea>();
    ideaRegistry->registerIdea<NumberInputIdea>();
    ideaRegistry->registerIdea<FractionInputIdea>();
    ideaRegistry->registerIdea<SquareRootFractionInputIdea>();
}

void CalculatorPlugin::registerDataTypeAttributes(std::shared_ptr<DataTypeRegistry> dataTypeRegistry)
{
    dataTypeRegistry->registerDataTypeAttributes<NumberDataTypeAttributes>();
}

void CalculatorPlugin::registerToolbars(std::shared_ptr<ToolbarRegistry> toolbarRegistry)
{
    QStringList orderList;
    orderList << Magic<NumberInputAttributes>::cast().listName();
    orderList << Magic<LabeledNumberInputAttributes>::cast().listName();
    orderList << Magic<FractionInputAttributes>::cast().listName();
    orderList << Magic<NumberDisplayAttributes>::cast().listName();
    orderList << Magic<LabeledNumberDisplayAttributes>::cast().listName();
    orderList << Magic<AdditionAttributes>::cast().listName();
    orderList << Magic<SubtractionAttributes>::cast().listName();
    orderList << Magic<MultiplicationAttributes>::cast().listName();
    orderList << Magic<DivisionAttributes>::cast().listName();
    orderList << Magic<ReciprocalAttributes>::cast().listName();
    orderList << Magic<PowerOfTenIdeaAttributes>::cast().listName();
    orderList << Magic<ExponentialAttributes>::cast().listName();
    orderList << Magic<PowerOfTwoIdeaAttributes>::cast().listName();
    orderList << Magic<PowerAttributes>::cast().listName();
    orderList << Magic<LogarithmBase10Attributes>::cast().listName();
    orderList << Magic<LogarithmBaseEAttributes>::cast().listName();
    orderList << Magic<LogarithmBase2Attributes>::cast().listName();
    orderList << Magic<MathematicalConstantsAttributes>::cast().listName();
    orderList << Magic<SquaredAttributes>::cast().listName();
    orderList << Magic<SquareRootAttributes>::cast().listName();
    orderList << Magic<SineAttributes>::cast().listName();
    orderList << Magic<CosineAttributes>::cast().listName();
    orderList << Magic<TangentAttributes>::cast().listName();
    orderList << Magic<AbsoluteValueAttributes>::cast().listName();
    orderList << Magic<CeilingAttributes>::cast().listName();
    orderList << Magic<FloorAttributes>::cast().listName();
    orderList << Magic<InverseSineAttributes>::cast().listName();
    orderList << Magic<InverseCosineAttributes>::cast().listName();
    orderList << Magic<InverseTangentAttributes>::cast().listName();
    orderList << Magic<NumberWeighterAttributes>::cast().listName();
    orderList << Magic<SliderNumberInputAttributes>::cast().listName();
    orderList << Magic<SquareRootFractionInputAttributes>::cast().listName();

    toolbarRegistry->registerToolbar(PluginName(),orderList);
}
