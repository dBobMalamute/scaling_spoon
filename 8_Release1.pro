QT += quick widgets quickcontrols2

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
    Calculator/AbsoluteValueIdea.h \
    Calculator/AdditionIdea.h \
    Calculator/CalculatorPlugin.h \
    Calculator/CeilingIdea.h \
    Calculator/CosineIdea.h \
    Calculator/DivisionIdea.h \
    Calculator/ExponentialIdea.h \
    Calculator/FloorIdea.h \
    Calculator/FractionInputIdea.h \
    Calculator/InverseCosineIdea.h \
    Calculator/InverseSineIdea.h \
    Calculator/InverseTangentIdea.h \
    Calculator/LabeledNumberDisplayIdea.h \
    Calculator/LabeledNumberInputIdea.h \
    Calculator/LogarithmBase10Idea.h \
    Calculator/LogarithmBase2Idea.h \
    Calculator/LogarithmBaseEIdea.h \
    Calculator/MathematicalConstantsIdea.h \
    Calculator/MultiplicationIdea.h \
    Calculator/NumberDisplayIdea.h \
    Calculator/NumberInputIdea.h \
    Calculator/NumberWeighterIdea.h \
    Calculator/PowerIdea.h \
    Calculator/PowerOfTenIdea.h \
    Calculator/PowerOfTwoIdea.h \
    Calculator/ReciprocalIdea.h \
    Calculator/SineIdea.h \
    Calculator/SliderNumberInputIdea.h \
    Calculator/SquareRootFractionInputIdea.h \
    Calculator/SquareRootIdea.h \
    Calculator/SquaredIdea.h \
    Calculator/SubtractionIdea.h \
    Calculator/TangentIdea.h \
    DataTypes/DistributionDataType.h \
    DataTypes/NumberDataType.h \
    DataTypes/NumbersDataType.h \
    Distributions/BetaDistributionIdea.h \
    Distributions/BinomialDistributionIdea.h \
    Distributions/DistributionBinnerIdea.h \
    Distributions/DistributionCommonFunctions.h \
    Distributions/DistributionDisplay.h \
    Distributions/DistributionDisplayIdea.h \
    Distributions/DistributionMeanIdea.h \
    Distributions/DistributionSamplerIdea.h \
    Distributions/DistributionStandardDeviationIdea.h \
    Distributions/DistributionVarianceIdea.h \
    Distributions/DistributionWeighterIdea.h \
    Distributions/DistributionsPlugin.h \
    Distributions/ExponentialDistributionIdea.h \
    Distributions/GaussianDistributionIdea.h \
    Distributions/GaussianPPDistributionIdea.h \
    Distributions/GeometricDistributionIdea.h \
    Distributions/LogNormalDistributionIdea.h \
    Distributions/MeanFromDistributionSamplerIdea.h \
    Distributions/NegativeBinomialDistributionIdea.h \
    Distributions/PoissonDistributionIdea.h \
    Distributions/PowerLawDistributionIdea.h \
    Distributions/StdevFromDistributionSamplerFixedMeanIdea.h \
    Distributions/StdevFromDistributionSamplerIdea.h \
    Distributions/StudentTDistributionIdea.h \
    Distributions/TriangleDistributionIdea.h \
    Distributions/UniformDistributionIdea.h \
    Distributions/VarianceFromDistributionSamplerFixedMeanIdea.h \
    Distributions/VarianceFromDistributionSamplerIdea.h \
    MalamuteCore/Casing.h \
    MalamuteCore/CasingBackend.h \
    MalamuteCore/CasingState.h \
    MalamuteCore/Commands/AddCasingCommand.h \
    MalamuteCore/Commands/AddDefaultDisplayCommand.h \
    MalamuteCore/Commands/AddDefaultSourceCommand.h \
    MalamuteCore/Commands/AddPostItCommand.h \
    MalamuteCore/Commands/AddSelectionCommand.h \
    MalamuteCore/Commands/AddWireCommand.h \
    MalamuteCore/Commands/ChangeColorsCommand.h \
    MalamuteCore/Commands/ChangeParamsCommand.h \
    MalamuteCore/Commands/DeleteCasingCommand.h \
    MalamuteCore/Commands/DeletePostItCommand.h \
    MalamuteCore/Commands/DeleteSelectionCommand.h \
    MalamuteCore/Commands/DeleteWireCommand.h \
    MalamuteCore/Commands/MinimizeCasingCommand.h \
    MalamuteCore/Commands/MoveSelectionCommand.h \
    MalamuteCore/Commands/ResizeCasingCommand.h \
    MalamuteCore/Commands/ShiftWireCommand.h \
    MalamuteCore/Corkboard.h \
    MalamuteCore/CorkboardBackend.h \
    MalamuteCore/CorkboardHandler.h \
    MalamuteCore/InheritedClasses/DataType.h \
    MalamuteCore/InheritedClasses/DataTypeAttributes.h \
    MalamuteCore/InheritedClasses/Idea.h \
    MalamuteCore/InheritedClasses/IdeaAttributes.h \
    MalamuteCore/InheritedClasses/Magic.h \
    MalamuteCore/InheritedClasses/MalamutePlugin.h \
    MalamuteCore/Plug.h \
    MalamuteCore/PostIt.h \
    MalamuteCore/QMLRegisterHelper.h \
    MalamuteCore/Registries/DataTypeRegistry.h \
    MalamuteCore/Registries/IdeaRegistry.h \
    MalamuteCore/Registries/ToolbarRegistry.h \
    MalamuteCore/Styling/CasingStyle.h \
    MalamuteCore/Styling/ControlsStyle.h \
    MalamuteCore/Styling/CorkboardStyle.h \
    MalamuteCore/Styling/GearStyle.h \
    MalamuteCore/Styling/PostItStyle.h \
    MalamuteCore/Styling/WireStyle.h \
    MalamuteCore/SurfaceAndStyleSetup.h \
    MalamuteCore/Wire.h \
    MalamuteCore/WireBackend.h \
    MalamuteCore/WireCasingInteraction.h \
    MalamuteCore/WireState.h \
    Numbers/NumbersBinnerIdea.h \
    Numbers/NumbersDisplayIdea.h \
    Numbers/NumbersInputIdea.h \
    Numbers/NumbersMeanIdea.h \
    Numbers/NumbersPlugin.h \
    Numbers/NumbersSkewIdea.h \
    Numbers/NumbersStandardDeviationFixedMeanIdea.h \
    Numbers/NumbersStandardDeviationIdea.h \
    Numbers/NumbersVarianceFixedMeanIdea.h \
    Numbers/NumbersVarianceIdea.h \
    Utilities/ColoredSquareIdea.h \
    Utilities/TextAreaIdea.h \
    Utilities/TextLineIdea.h \
    Utilities/UtilitiesPlugin.h

SOURCES += \
    Calculator/AbsoluteValueIdea.cpp \
    Calculator/AdditionIdea.cpp \
    Calculator/CalculatorPlugin.cpp \
    Calculator/CeilingIdea.cpp \
    Calculator/CosineIdea.cpp \
    Calculator/DivisionIdea.cpp \
    Calculator/ExponentialIdea.cpp \
    Calculator/FloorIdea.cpp \
    Calculator/FractionInputIdea.cpp \
    Calculator/InverseCosineIdea.cpp \
    Calculator/InverseSineIdea.cpp \
    Calculator/InverseTangentIdea.cpp \
    Calculator/LabeledNumberDisplayIdea.cpp \
    Calculator/LabeledNumberInputIdea.cpp \
    Calculator/LogarithmBase10Idea.cpp \
    Calculator/LogarithmBase2Idea.cpp \
    Calculator/LogarithmBaseEIdea.cpp \
    Calculator/MathematicalConstantsIdea.cpp \
    Calculator/MultiplicationIdea.cpp \
    Calculator/NumberDisplayIdea.cpp \
    Calculator/NumberInputIdea.cpp \
    Calculator/NumberWeighterIdea.cpp \
    Calculator/PowerIdea.cpp \
    Calculator/PowerOfTenIdea.cpp \
    Calculator/PowerOfTwoIdea.cpp \
    Calculator/ReciprocalIdea.cpp \
    Calculator/SineIdea.cpp \
    Calculator/SliderNumberInputIdea.cpp \
    Calculator/SquareRootFractionInputIdea.cpp \
    Calculator/SquareRootIdea.cpp \
    Calculator/SquaredIdea.cpp \
    Calculator/SubtractionIdea.cpp \
    Calculator/TangentIdea.cpp \
    DataTypes/DistributionDataType.cpp \
    DataTypes/NumberDataType.cpp \
    DataTypes/NumbersDataType.cpp \
    Distributions/BetaDistributionIdea.cpp \
    Distributions/BinomialDistributionIdea.cpp \
    Distributions/DistributionBinnerIdea.cpp \
    Distributions/DistributionCommonFunctions.cpp \
    Distributions/DistributionDisplay.cpp \
    Distributions/DistributionDisplayIdea.cpp \
    Distributions/DistributionMeanIdea.cpp \
    Distributions/DistributionSamplerIdea.cpp \
    Distributions/DistributionStandardDeviationIdea.cpp \
    Distributions/DistributionVarianceIdea.cpp \
    Distributions/DistributionWeighterIdea.cpp \
    Distributions/DistributionsPlugin.cpp \
    Distributions/ExponentialDistributionIdea.cpp \
    Distributions/GaussianDistributionIdea.cpp \
    Distributions/GaussianPPDistributionIdea.cpp \
    Distributions/GeometricDistributionIdea.cpp \
    Distributions/LogNormalDistributionIdea.cpp \
    Distributions/MeanFromDistributionSamplerIdea.cpp \
    Distributions/NegativeBinomialDistributionIdea.cpp \
    Distributions/PoissonDistributionIdea.cpp \
    Distributions/PowerLawDistributionIdea.cpp \
    Distributions/StdevFromDistributionSamplerFixedMeanIdea.cpp \
    Distributions/StdevFromDistributionSamplerIdea.cpp \
    Distributions/StudentTDistributionIdea.cpp \
    Distributions/TriangleDistributionIdea.cpp \
    Distributions/UniformDistributionIdea.cpp \
    Distributions/VarianceFromDistributionSamplerFixedMeanIdea.cpp \
    Distributions/VarianceFromDistributionSamplerIdea.cpp \
    MalamuteCore/Casing.cpp \
    MalamuteCore/CasingBackend.cpp \
    MalamuteCore/CasingState.cpp \
    MalamuteCore/Commands/AddCasingCommand.cpp \
    MalamuteCore/Commands/AddDefaultDisplayCommand.cpp \
    MalamuteCore/Commands/AddDefaultSourceCommand.cpp \
    MalamuteCore/Commands/AddPostItCommand.cpp \
    MalamuteCore/Commands/AddSelectionCommand.cpp \
    MalamuteCore/Commands/AddWireCommand.cpp \
    MalamuteCore/Commands/ChangeColorsCommand.cpp \
    MalamuteCore/Commands/ChangeParamsCommand.cpp \
    MalamuteCore/Commands/DeleteCasingCommand.cpp \
    MalamuteCore/Commands/DeletePostItCommand.cpp \
    MalamuteCore/Commands/DeleteSelectionCommand.cpp \
    MalamuteCore/Commands/DeleteWireCommand.cpp \
    MalamuteCore/Commands/MinimizeCasingCommand.cpp \
    MalamuteCore/Commands/MoveSelectionCommand.cpp \
    MalamuteCore/Commands/ResizeCasingCommand.cpp \
    MalamuteCore/Commands/ShiftWireCommand.cpp \
    MalamuteCore/Corkboard.cpp \
    MalamuteCore/CorkboardBackend.cpp \
    MalamuteCore/CorkboardHandler.cpp \
    MalamuteCore/InheritedClasses/DataType.cpp \
    MalamuteCore/InheritedClasses/DataTypeAttributes.cpp \
    MalamuteCore/InheritedClasses/Idea.cpp \
    MalamuteCore/InheritedClasses/IdeaAttributes.cpp \
    MalamuteCore/InheritedClasses/MalamutePlugin.cpp \
    MalamuteCore/PostIt.cpp \
    MalamuteCore/Registries/DataTypeRegistry.cpp \
    MalamuteCore/Registries/IdeaRegistry.cpp \
    MalamuteCore/Registries/ToolbarRegistry.cpp \
    MalamuteCore/Styling/CasingStyle.cpp \
    MalamuteCore/Styling/ControlsStyle.cpp \
    MalamuteCore/Styling/CorkboardStyle.cpp \
    MalamuteCore/Styling/GearStyle.cpp \
    MalamuteCore/Styling/PostItStyle.cpp \
    MalamuteCore/Styling/WireStyle.cpp \
    MalamuteCore/Wire.cpp \
    MalamuteCore/WireBackend.cpp \
    MalamuteCore/WireCasingInteraction.cpp \
    MalamuteCore/WireState.cpp \
    MalamuteCore/main.cpp \
    Numbers/NumbersBinnerIdea.cpp \
    Numbers/NumbersDisplayIdea.cpp \
    Numbers/NumbersInputIdea.cpp \
    Numbers/NumbersMeanIdea.cpp \
    Numbers/NumbersPlugin.cpp \
    Numbers/NumbersSkewIdea.cpp \
    Numbers/NumbersStandardDeviationFixedMeanIdea.cpp \
    Numbers/NumbersStandardDeviationIdea.cpp \
    Numbers/NumbersVarianceFixedMeanIdea.cpp \
    Numbers/NumbersVarianceIdea.cpp \
    Utilities/ColoredSquareIdea.cpp \
    Utilities/TextAreaIdea.cpp \
    Utilities/TextLineIdea.cpp \
    Utilities/UtilitiesPlugin.cpp

RESOURCES += \
    Calculator/CalculatorResources.qrc \
    Distributions/DistributionResources.qrc \
    MalamuteCore/MalamuteCoreResources.qrc \
    Numbers/NumbersResources.qrc \
    Utilities/UtilitiesResources.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
