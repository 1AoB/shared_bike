import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick 2.2
import QtQuick.Controls 1.1

import QtQuick 2.9
import QtQuick.Window 2.2    //这个模块包含用于创建顶级窗口和访问屏幕信息的类型
import QtQuick.Controls 2.0  //这个模块提供了大量类似Qt Widgets模块那样的可重用组件

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls 2.12

//import QtWebView 1.0
import QtWebView 1.1


ApplicationWindow     //对应QMainWindow，提供顶层应用程序窗口
{
    property bool isphone: Screen.width < Screen.height ? 1 : 0   //如果宽小于高那么就是手机端

    property real multiplierW: window.width / 320      //宽高系数
    property real multiplierH: window.height / 480
    property string mapWindowUrl: ""

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

    id : window
    color : "white"
    visible : true
    width : (Qt.platform.os == "ios" || Qt.platform.os == "android") ? Screen.width : 320  //Qt.platform.os 返回操作系统名称
    height : (Qt.platform.os == "ios" || Qt.platform.os == "android") ? Screen.height : 480

    MsgDialog        //消息提示框
    {
        id : msgWindow
    }

    Component   //Component类型本质上允许在一个QML文档内内联定义QML组件(QML组件由组件文件(.qml文件)定义)  在QML中嵌入组件
    {
        id : loginPage
       // LoginPage{}
    }

    Component
    {
        id : mapPage
       // MapWindow{}
    }

    Component
    {
        id : registerPage
       // RegisterPage{}
    }

    StackView     //基于堆栈的导航模型
    {
        id : myStackView
        anchors.fill: parent     //锚定位 //锚点布局的填充对象是父对象
        initialItem: loginPage   //初始推送的项
    }

    function enterToMainPage()   //切换显示主页面
    {
        mapWindowUrl = "file:///android_asset/map.html"  //File协议  //地图资源
        myStackView.push(mapPage)
    }

    function enterToBikeManagePage()    //切换显示管理员页面
    {
        mapWindowUrl = "file:///android_asset/bikeManage.html"
        myStackView.push(mapPage)
    }

    function returnToLoginPage()    //转回到登陆页面
    {
        myStackView.pop()
    }

    function enterToRegPage()       //切换显示注册页面
    {
        myStackView.push(registerPage)
    }

    function refreshLoginPage()     //刷新登录页面
    {
        myStackView.replace(loginPage, loginPage)
    }

    function openMsgWindow(msgText)   //打开消息提示框
    {
        msgWindow.open(msgText)
    }
}

