import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import UserLogin.module 1.0      //使用main.cpp中注册的C++类型UserLogin

Rectangle
{
    property bool isphone: Screen.width < Screen.height ? 1 : 0   //如果宽小于高那么就是手机端

    property real multiplierW: registerWindow.width / 320      //宽高系数
    property real multiplierH: registerWindow.height / 480
    property int count: 10

    property int isDebug: 1

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

    MsgDialog
    {
        id: msgWindow
    }

    UserLogin          //UserLogin 类的实例
    {
      id : userLogin
    }

    function openMsgWindow(msg)
    {
        msgWindow.open(msg)
    }

    id: registerWindow
    color : "white"
    visible : false
    width : (Qt.platform.os == "ios" || Qt.platform.os == "android") ? Screen.width : 320  //Qt.platform.os 返回操作系统名称
    height : (Qt.platform.os == "ios" || Qt.platform.os == "android") ? Screen.height : 480

    Rectangle
    {
        id: bg_rect
        anchors.fill: parent
        color: "#ecedf1"
        z: 0
    }

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
        height: dpH(200)
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
                font.pixelSize: 40     //字体像素大小
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
            placeholderText: "在此输入密码"
            maximumLength: 16

            validator: RegExpValidator
            {
                regExp: /^[0-9a-zA-Z_.@]+$/
            }

            style: TextFieldStyle
            {
                font.pixelSize: 40     //字体像素大小
                background: Rectangle
                {
                    color: "transparent"
                    border.color: "grey"
                    radius: dpX(6)
                }
            }
        }

        /*                确认密码文本框                */
        TextField
        {
            id: confirm_password
            x: 0
            y: dpH(70)
            z: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(20)
            anchors.rightMargin: dpW(20)
            width: dpW(parent.width)
            height: dpH(30)

            echoMode: TextInput.Password       //密码输入时显示*
            placeholderText: "再次输入密码"
            maximumLength: 16

            validator: RegExpValidator
            {
                regExp: /^[0-9a-zA-Z_.@]+$/
            }

            style: TextFieldStyle
            {
                font.pixelSize: 40     //字体像素大小
                background: Rectangle
                {
                    color: "transparent"
                    border.color: "grey"
                    radius: dpX(6)
                }
            }
        }

        /*                验证码文本框                */
        TextField
        {
            id: ver_code
            x: 0
            y: dpH(105)
            z: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(20)
            anchors.rightMargin: dpW(200)
            width: dpW(parent.width)
            height: dpH(30)

            placeholderText: "在此输入验证码"
            maximumLength: 6
            validator: RegExpValidator
            {
                regExp: /^[0-9]+$/
            }
            style: TextFieldStyle
            {
                font.pixelSize: 40     //字体像素大小
                background: Rectangle
                {
                    color: "transparent"
                    border.color: "grey"
                    radius: dpX(6)
                }
            }
        }

        /*                获取验证码按钮                */
        Rectangle
        {
            id: getVerCode_button
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(125)
            anchors.rightMargin: dpW(120)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: dpH(66)

            color: "#1eb9f2"
            width: dpW(70)
            height: dpH(30)
            radius: dpX(6)
            border.color: "#1eb9f2"
            border.width: dpX(1)

            Text
            {
                id: getVerCode_button_text
                anchors.centerIn: parent
                font.pixelSize: 35
                color: "white"
                font.family: "微软雅黑"
                text: "获取验证码"
            }

            MouseArea
            {
                id: btnClickArea
                anchors.fill: parent
                onClicked:       //处理点击信号
                {
                    if(username_TF.text == "")
                    {
                        openMsgWindow("请输入用户名")
                    }
                    else
                    {
                        //if(isDebug) openMsgWindow("获取验证码按钮被点击")
                        timer.start()        //定时器
                        btnClickArea.enabled = false      //获取验证码按钮不可用
                        userLogin.getVerificationCode_sig(username_TF.text)     //发送UserLogin类信号
                    }
                }
            }
        }

        /*                注册按钮                */
        Rectangle
        {
            id: reg_button
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: dpW(205)
            anchors.rightMargin: dpW(20)
            anchors.bottom: parent.bottom
            anchors.bottomMargin: dpH(66)

            color: "#1eb9f2"
            width: dpW(50)
            height: dpH(30)
            radius: dpX(6)
            border.color: "#1eb9f2"
            border.width: dpX(1)

            Text
            {
                id: reg_button_text
                anchors.centerIn: parent
                color: "white"
                font.family: "微软雅黑"
                text: "注    册"
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:       //处理点击信号
                {
                    console.log("reg_button onClicked...")
                    if(username_TF.text == "")
                    {
                        openMsgWindow("用户名不能为空！")
                    }
                    else if(password_TF.text == "" || confirm_password.text == "")
                    {
                        openMsgWindow("密码不能为空！")
                    }
                    else if(ver_code.text == "")
                    {
                        openMsgWindow("请输入验证码！")
                    }
                    else if(password_TF.text != confirm_password.text)
                    {
                        openMsgWindow("两次输入的密码不一致！")
                    }
                    else
                    {
                        userLogin.registerUser_sig(username_TF.text, password_TF.text, ver_code.text);   //发送UserLogin类信号
                    }
                }
            }
        }

        /*                返回按钮                */
        Text
        {
            id: register_text
            text: qsTr("返 回")
            anchors.right: parent.right
            anchors.rightMargin: dpW(15)
            anchors.bottom: parent.bottom
            color: "#1768e1"
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    console.log("myStackView.depth: " + myStackView.depth)  //打印当前压入堆栈的项数
                    if(myStackView.depth > 1) myStackView.pop()
                    else myStackView.push(loginPage)
                }
            }
        }
    }          //end Column

    //定时器的调用方式，restart()、start()、stop()
    Timer
    {
        id: timer
        interval: 1000  //定时器时间间隔  1s
        repeat: true    //是否重复定时
        running: false  //是否开启定时
        onTriggered:
        {
            if(count == 10)
            {
                getVerCode_button.color = "#808080"
                getVerCode_button.border.color = "#808080"
                getVerCode_button_text.font.pointSize = dpW(7)
            }

            //if(isDebug) openMsgWindow("定时器启动")
            getVerCode_button_text.text = Number(count)
            count--
            if(count == 0)
            {
                count = 10
                timer.stop()
                getVerCode_button.color = "#1eb9f2"
                getVerCode_button.border.color = "#1eb9f2"
                getVerCode_button_text.font.pointSize = dpW(4)
                getVerCode_button_text.text = "获取验证码"
                btnClickArea.enabled = true
            }
        }
    }
}
