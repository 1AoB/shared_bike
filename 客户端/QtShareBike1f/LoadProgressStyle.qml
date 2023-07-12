import QtQuick 2.2
import QtQuick.Controls.Styles 1.2

ProgressBarStyle
{
    background: Rectangle           //进度条背景
    {
        radius: 2
        color: "transparent"
        implicitHeight: urlField.height - 2
        implicitWidth: urlField.width
    }
    progress: Rectangle             //进度条填充色
    {
        anchors.fill: parent
        color: "#80c342"
        opacity: 0.5       //不透明度
    }
}
