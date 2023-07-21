import QtQuick 2.12
import QtQuick.Window 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4     //style
import QtQuick.Controls.Styles 1.4
import UserLogin.module 1.0      //使用main.cpp中注册的C++类型UserLogin

Rectangle
{
    id : loginWindow
    color : "white"
    visible : true
    width : (Qt.platform.os == "ios" || Qt.platform.os == "android") ? Screen.width : 320  //Qt.platform.os 返回操作系统名称
    height : (Qt.platform.os == "ios" || Qt.platform.os == "android") ? Screen.height : 480

    property bool isphone: Screen.width < Screen.height ? 1 : 0   //如果宽小于高那么就是手机端

    property real multiplierW: loginWindow.width / 320      //宽高系数
    property real multiplierH: loginWindow.height / 480

    function dpH(num)      //计算高的函数
    {
        return num * multiplierH
    }

    function dpW(num)      //计算宽的函数
    {
        return num * multiplierW
    }

    function dpX(num)      //宽+高平均值函数
    {
        return (dpW(num) + dpH(num)) / 2
    }

    UserLogin          //UserLogin 类的实例
    {
        id : userLogin
    }

    Rectangle
    {
        id : bg_rect
        anchors.fill : parent
        color : "#ecedf1"
        z : 0      //较大的排列在较小的之上
    }

    /*加载动态背景图
    currentFrame，指示当前正在播放的帧序号
    frameCount，指示图片的总帧数
    paused，表示是否暂停，设置它也可以暂停或继续播放
    playing，指示动画是否在播放，默认为 true ，意思是 AnimatedImage 对象创建后立即开始播放
    source，类型为 url ，指定要播放的图片地址，可以使本地文件、 qrc 里的资源、网络文件
    */
    AnimatedImage
    {
        anchors.top: bg_rect.top
        id: animated
        source: "qrc:/images/bike.gif"
        width: parent.width
        height: parent.height
    }

    Column
    {
        width: parent.width
        height: dpH(150)
        x: 0
        y: dpH(120)

        /*                 账号文本框                 */
        TextField
        {
            id: username_TF
            x: 0
            y: 0
            z: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(20)
            anchors.rightMargin: dpW(20)
            width: dpW(parent.width)
            height: dpH(30)
            maximumLength: 18   //最大输入长度
            validator: RegExpValidator
            {
                regExp: /^[0-9a-zA-Z_.@]+$/
            }
            placeholderText: "QQ号/手机号/邮箱"
            style: TextFieldStyle
            {
                font.pixelSize: 55     //字体像素大小
                background: Rectangle
                {
                    color: "transparent"
                    border.color: "grey"
                    radius: dpX(6)
                }
            }
        }

        /*                 密码文本框                 */
        TextField
        {
            id: password_TF
            x: 0
            y: dpH(35)
            z: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(20)
            anchors.rightMargin: dpW(20)
            width: dpW(parent.width)
            height: dpH(30)

            echoMode: TextInput.Password       //密码输入时显示*
            placeholderText: "密码"
            maximumLength: 16 //最大输入长度

            validator: RegExpValidator
            {
                regExp: /^[0-9a-zA-Z_.@]+$/
            }

            style: TextFieldStyle
            {
                font.pixelSize: 55     //字体像素大小
                background: Rectangle
                {
                    color: "transparent"
                    border.color: "grey"
                    radius: dpX(6)
                }
            }
        }

        /*                 登录按钮                 */
        Rectangle
        {
            id: login_button
            anchors.left:  parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(20)
            anchors.rightMargin: dpW(100)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: dpH(45)

            color: "#1eb9f2"
            height: dpH(30)
            radius: dpX(6)
            border.color: "#1eb9f2"
            border.width: dpX(1)

            Text {
                id: login_button_text
                anchors.centerIn:  parent    //放在父部件正中间
                color: "white"
                font.family: "微软雅黑"
                text: qsTr("登  录")
            }

            MouseArea
            {
                anchors.fill:  parent
                onClicked:
                {
                    if(username_TF.text == "")
                    {
                        openMsgWindow("用户名不能为空！")
                    }
                    if(password_TF.text == "")
                    {
                        openMsgWindow("密码不能为空！")
                    }
                    else
                    {
                        login_button_text.text = "正在登陆..."
                        userLogin.loginToBikeClient_sig(username_TF.text, password_TF.text)    //发送UserLogin类信号
                    }
                }
            }
        }

        /*                 注册账号按钮                 */
        Rectangle
        {
            id: register_button
            anchors.left:  parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(230)
            anchors.rightMargin: dpW(20)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: dpH(45)

            color: "#1eb9f2"
            height: dpH(30)
            radius: dpX(6)
            border.color: "#1eb9f2"
            border.width: dpX(1)

            Text {
                id: register_button_text
                anchors.centerIn:  parent    //放在父部件正中间
                color: "white"
                font.family: "微软雅黑"
                text: qsTr("注册账号")
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    enterToRegPage();        //main.qml内函数
                }
            }
        }
    }

}
