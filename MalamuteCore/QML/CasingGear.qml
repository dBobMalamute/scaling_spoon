import QtQuick 2.0
import com.malamute.core 1.0

Image
{
    id: gear
    x: parent.width - 53;
    y: parent.height - 35
    z: -1;
    source: GearStyle.gearPath();

    property bool rotating: false;

    NumberAnimation
    {
        id: runningAnimation;
        target: gear
        loops: Animation.Infinite
        property: "rotation"
        from: 0
        to: 360
        duration: GearStyle.gearSpeed()
    }
    SequentialAnimation
    {
        id: stoppingAnimation;
        PauseAnimation
        {
            duration: 2500
        }

        ScriptAction
        {
            script: runningAnimation.stop();
        }
    }

    onRotatingChanged:
    {
        if(rotating)
        {
            runningAnimation.start();
        }
        else
        {
            stoppingAnimation.start();
        }
    }
}

