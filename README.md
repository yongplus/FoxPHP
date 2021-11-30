### FoxPHP

一个轻量级的Nginx+PHP8环境，下载后安装即可运行，运行占用资源少。 



[点击下载](./FoxPHPInstaller.exe?raw=true)

安装后访问：http://localhost/   
能正常打开并看到FoxPHP字样则表示安装成功  
查看PHPINFO: http://localhost/phpinfo.php

##### HOST用法
本人开发是用的线上数据库，将数据库服务器IP改为HOST连接，并绑定IP这样既可实现在公司电脑上是外网IP，线上服务器上用内网IP，另外对于小程序有域名限制，切到开发域名上线容易忘记改，可以将线上域名绑定HOST，通过快捷键快速切换域名走本地。  
软件只会操作#BeginFoxJky 到 #EndFoxJky区间的HOST

##### 统托盘Icon颜色  
灰色：服务未启动   
蓝色：部分服务启动  
黄色：Nginx和PHPCGI均运行中 

默认启动软件将启动所有服务

##### Screenshots
托盘：  
![截图](./Screenshots/img2.png)  
主界面：  
![截图](./Screenshots/img1.png)  
安装访问：  
![截图](./Screenshots/img3.png)  


软件最初只是自己用的，本人开发用的线上数据库，所以不带MYSQL（这样还能节省本地CPU和内存资源消耗）  

