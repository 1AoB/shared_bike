package an.qt.qtWithJava;    //package类似一个文件夹，文件夹内有各种文件；package存在的意义是防止命名冲突造成使用不便

//单类型导入，仅仅导入一个 public 类或者接口
import java.io.OutputStream;    //java标准库提供的最基本的输出流，和java.io.InputStream相反
import java.util.Iterator;      //迭代器（对集合进行遍历）
import java.io.File;            //File类是java.io包中唯一代表磁盘文件本身对象，定义了与平台无关的方法来操作文件，可以新建，删除，访问属性等操作，但不能访问文件内容

import android.app.Notification;             //自定义消息通知
import android.app.NotificationManager;      //消息通知管理器
import android.app.PendingIntent;            //实现了一种委托授权发送Intent进行组件间通信的机制
import android.app.NotificationChannel;      //应用于主题相似的通知集合的设置表示形式  //通知渠道，以便为管理通知行为和设置提供更轻松和更统一的方式
import android.graphics.Color;               //包含颜色值

import android.app.Activity;                 //用于包含和用户交互的组件
import android.content.Intent;               //Intent可以在当前Activity中指定想要完成的动作，还可以在不同组件间进行数据传递
import android.content.ContentValues;        //ContentValues只能存储基本类型的数据，像string、int之类的，不能存储对象
import android.content.res.Configuration;    //通过该类，可以修改语言参数，实现对应用语言的变更
import android.hardware.Camera;              //引用了管道的概念将android设备和摄像头之间联通起来
import android.net.Uri;                      //不可变的URI引用
import android.os.Bundle;                    //Bundle类用作携带数据，它类似于Map，用于存放key-value名值对形式的值
import android.provider.MediaStore;          //MediaStore这个类是Android系统提供的一个多媒体数据库，android中多媒体信息都可以从这里提取
import android.view.SurfaceHolder;           //SurfaceHolder是一个接口，类似于一个surace的监听器
import android.view.SurfaceView;             //SurfaceView其中内嵌了一个专门用于绘制的Surface，SurfaceView可以控制这个Surface的格式和尺寸，以及Surface的绘制位置
import android.view.View;                    //在Android系统中，任何可视化控件都需要从android.view.View类继承
import android.view.KeyEvent;                //按键消息事件
import android.widget.LinearLayout;          //线性布局
import android.os.Environment;               //提供访问环境变量
import android.content.pm.PackageManager;    //用于检索安装到设备上的应用程序的各种信息

import android.Manifest;
import android.content.Context;
import android.os.Build;                     //获取安卓的一些系统信息
import android.os.Parcelable;                //Android提供的序列化的接口
import android.text.TextUtils;               //字符串处理
import android.view.Window;
import android.view.WindowManager;

import androidx.core.app.ActivityCompat;             //报错:
import androidx.core.content.ContextCompat;
import androidx.core.app.NotificationCompat;

//zxing二维码第三方库
import com.yzq.zxinglibrary.android.CaptureActivity; //报错:
import com.yzq.zxinglibrary.common.Constant;
import com.yzq.zxinglibrary.bean.ZxingConfig;

public class MyJavaClass extends org.qtproject.qt5.android.bindings.QtActivity   //这个QtActivity是固定的写好的
{
        private static NotificationManager m_notificationManager;
        private static MyJavaClass m_instance;
        public static native void SendDataToQt(String data);   //一个native方法就是一个Java调用非Java代码的接口，这里是Java调用Qt方法(C++) //自定义networkInterface类内

        private static int num;
        private static final int REQUEST_CODE_SCAN = 1;     //被final修饰的变量一旦获得初始值就不可改变，不能被重新赋值
        private static int REQUEST_PERMISSION_CODE = 3;

        public static String[] permissions = new String[]   //Android 动态申请权限   //Manifest.permission（权限类）
        {   //权限数组
                Manifest.permission.CAMERA,                    //允许访问摄像头进行拍照
                Manifest.permission.WRITE_EXTERNAL_STORAGE,    //写入外部存储
                Manifest.permission.ACCESS_FINE_LOCATION,      //通过GPS获取精确位置
                Manifest.permission.ACCESS_COARSE_LOCATION     //通过WIFI或移动基站获取粗略位置
        };

        public MyJavaClass()
        {
                m_instance = this;
        }

        public static MyJavaClass getInstance()
        {
                return m_instance;
        }

        //请求权限
        public static void getPermission()
        {
                for(int i = 0; i < permissions.length; i++)
                {
                        if(ActivityCompat.checkSelfPermission(m_instance, permissions[i]) != PackageManager.PERMISSION_GRANTED)    //判断权限是否已经被授权
                        {
                                ActivityCompat.requestPermissions(m_instance, permissions, REQUEST_CODE_SCAN);       //请求用户授权几个权限
                        }
                }
        }

        //选择图片
        public static void choosePhoto()
        {
                Intent intentToPickPic = new Intent(Intent.ACTION_PICK, null);
                // 如果限制上传到服务器的图片类型时可以直接写如："image/jpeg 、 image/png等的类型" 所有类型则写 "image/*"
                intentToPickPic.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/jepg");

                //startActivityForResult主要用来从FirstActivity跳转到SecondActivity然后返回FirstActivity并且获取从SecondActivity传回来的参数
                m_instance.startActivityForResult(intentToPickPic, 1);
        }

        public static void openSetting(String action)
        {
                m_instance.startActivity(new Intent(action));     //启动Activity   //Activity是一个应用程序组件，提供一个屏幕，用户可以用来交互为了完成某项任务
        }

        //扫描二维码
        public static void scanQRCode(String action)
        {
                Intent intent = new Intent(m_instance, CaptureActivity.class);
                /* ZxingConfig是配置类  可以设置是否显示底部布局，闪光灯，相册，是否播放提示音、震动等动能
         * 也可以不传这个参数
         * 不传的话  默认都为默认不震动  其他都为true
                 */
                getPermission();
                ZxingConfig config = new ZxingConfig();
                config.setShowbottomLayout(true);      //底部布局，报错闪光灯和相册
                config.setPlayBeep(true);              //是否播放震动音
                config.setShake(true);                 //是否震动
                config.setShowAlbum(true);             //是否显示相册
                config.setShowFlashLight(true);        //是否显示闪光灯
                intent.putExtra(Constant.INTENT_ZXING_CONFIG, config);    //putExtra("A",B)中，AB为键值对，第一个参数为键名，第二个参数为键对应的值
                m_instance.startActivityForResult(intent, REQUEST_CODE_SCAN);  //主要用来从FirstActivity跳转到SecondActivity然后返回FirstActivity并且获取从SecondActivity传回来的参数
        }


        @Override
        protected void onActivityResult(int requestCode, int resultCode, Intent data)
        {
                super.onActivityResult(requestCode, resultCode, data);

                //扫描二维码/条码回传
                if(requestCode == REQUEST_CODE_SCAN && resultCode == RESULT_OK)
                {
                        if(data != null)  //这个null必须是小写
                        {
                                String content = data.getStringExtra(Constant.CODED_CONTENT);
                                SendDataToQt(content);
                        }
                }
        }

        //手机在应用程序按下返回键时退出界面,但不销毁，程序仍在后台运行
        @Override
        public boolean onKeyDown(int keyCode, KeyEvent event)       //boolean: 类似C++中的bool类型
        {
                if(keyCode == KeyEvent.KEYCODE_BACK)
                {
                        //返回键退出界面,但不销毁，程序仍在后台运行
                        moveTaskToBack(false);
                        return true;
                }
                return super.onKeyDown(keyCode, event);
        }

        //通知
        public static void notify(String msg)
        {
                if(m_notificationManager == null)
                {
                        m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
                }

                NotificationChannel channel = new NotificationChannel("1", "重要通知", NotificationManager.IMPORTANCE_HIGH);
                m_notificationManager.createNotificationChannel(channel);      //创建通知渠道

                //通常创建一个通知的流程是通过NotificationCompat.Builder对象创建一个Build，然后调用build()方法返回一个Notification对象，最后使用NotificationManager.notify()显示一个通知
                NotificationCompat.Builder builder = new NotificationCompat.Builder(m_instance, "1");

                builder.setSmallIcon(R.drawable.icon)           //一个小图标
                           .setContentTitle("【奇牛科技】")           //一个标题
                           .setAutoCancel(true)                     //通过点击系统通知栏移除通知
                           .setWhen(System.currentTimeMillis());    //发送时间
                builder.setStyle(new NotificationCompat.BigTextStyle().bigText(msg));
                m_notificationManager.notify(12, builder.build());  //第一个参数是id，需要保证每个通知指定的id都是不同的，第二个参数是Notification对象
        }


}
