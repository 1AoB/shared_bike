import QtQuick 2.2
import QtQuick.Controls 1.1
import QtWebView 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.0

Rectangle
{
    visible: true

    WebView
    {
        id: webView
        anchors.fill: parent
        url: window.mapWindowUrl    //main.qml内
        onLoadingChanged:     //Signals: loadingChanged(loadRequest)
        {
            //loadRequest参数保存请求的url和状态，以及一个errorString，其中包含请求失败的错误消息。
            if(loadRequest.errorString) console.error(loadRequest.errorString)     //将调试信息打印到控制台
        }
    }
}
