import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle
{
    id: msgWindow
    property bool isphone: Screen.width < Screen.height ? 1 : 0   //如果宽小于高那么就是手机端

    property real multiplierW: window.width / 320      //宽高系数
    property real multiplierH: window.height / 480

    function dpH(num)      //计算高的函数
    {
        return num * multiplierH
    }

    function dpW(num)      //计算宽的函数
    {
        return num * multiplierW
    }

    anchors.centerIn:  parent

    property alias tipText: msg.text   //提示框内容  //alias作用：允许转发一个属性或者转发一个属性对象到另一个作用域

    property string backGroundColor: "#8ee1f0"       //提示框颜色
    property Item parentItem: Rectangle{}

    width: dpW(120)
    height: dpW(100)

    Dialog
    {
        id : dialog
        width: msgWindow.width
        height: msgWindow.height
        modal: true
        background: Rectangle
        {
            color: backGroundColor
            anchors.fill: parent
            radius: 5
        }

        header: Rectangle      //窗口标题项
        {
            width: dialog.width
            height: dpW(20)
            border.color: backGroundColor
            color: backGroundColor
            radius: 5

            Image
            {
                width: dpW(20);
                height: dpW(20);
                anchors.centerIn: parent
                source: "qrc:/images/tip.png"
            }
        }

        contentItem: Rectangle      //内容项
        {
            border.color: backGroundColor
            color: backGroundColor
            Text
            {
                anchors.fill: parent
                anchors.centerIn: parent
                font.family: "微软雅黑"
                color: "gray"

                text: tipText
                wrapMode: Text.WordWrap   //此属性设置文本换行为文本项的宽度  //换行只在单词边界上完成。如果单词太长，contentWidth将超过设置的宽度
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        footer: Rectangle        //窗口页脚项
        {
            width: dpW(40)
            height: dpH(20)
            border.color: backGroundColor
            color: backGroundColor
            radius: 5
            Button
            {
                anchors.centerIn: parent
                width: dpW(40)
                height: dpH(20)
                background: Rectangle
                {
                    anchors.centerIn: parent
                    width: 130
                    height: 60
                    radius: 5
                    border.color: "#0f748b"
                    border.width: 2
                    color: backGroundColor

                    Text {
                        anchors.centerIn: parent
                        font.family: "微软雅黑"
                        font.bold: true
                        color: "#0f748b"
                        text: "确定"
                    }
                }
                onClicked:
                {
                    dialog.close();
                }
            }
        }
    }

    //利用Text的implicitWidth属性来调节提示框的大小
    //该Text的字体格式需要与contentItem中的字体一模一样
    Text
    {
        id: msg
        visible: false
        width: 300
        wrapMode: Text.WordWrap
        font.family: "微软雅黑"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    function open(msgtext)
    {
        msgWindow.x = (parent.width - dialog.width) * 0.5
        msgWindow.y = (parent.height - dialog.height) * 0.5

        msg.text = msgtext
        dialog.open();
    }
}
