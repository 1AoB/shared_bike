import QtQuick 2.2
import QtQuick.Controls.Styles 1.2

ProgressBarStyle
{
    background:  Rectangle
    {
        radius: 2
        color: "transparent"
        implicitHeight: Math.round(urlField.height / 1.3)   	//round: 四舍五入
        implicitWidth: urlField.width - 12
    }
}
