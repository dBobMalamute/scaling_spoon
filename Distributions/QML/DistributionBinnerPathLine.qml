import QtQuick 2.12
import QtQuick.Shapes 1.12

PathLine
{
    Behavior on y
    {
        SequentialAnimation
        {
            PauseAnimation
            {
                duration: 2200 * idea.animationSpeed;
            }
            NumberAnimation
            {
                duration: 1000 * idea.animationSpeed;
            }
        }
    }
}
