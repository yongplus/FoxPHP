### FoxPHP

一个轻量级的Nginx+PHP8环境，下载后安装即可运行。  
A lightweight PHP development environment built on Nginx+PHP8 for windows, Can be run directly after installation without any third-party environment or dependencies.  

该软件适用于本地开发，线上数据库，带HOST管理功能，支持快捷键切换，也是本人每天日常工作在使用的开发环境；  
将项目中的数据库配置host，在本地走服务器外网，在服务器上走内网IP，同时通过绑定线上域名host为127.0.0.1省去配置开发域名，通过快捷键快速切换线上与本地；  
切换的host需要配置在#BeginFoxPHP 到 #EndFoxPHP区间(初次使用，在软件中切换一次host管理会自动创建标识)。  

[Click here to download](https://github.com/yongplus/FoxPHP/releases)
  
Check phpinfo: http://localhost/phpinfo.php



##### 统托盘Icon颜色  
灰色：服务未启动   
蓝色：部分服务启动  
黄色：Nginx和PHPCGI均运行中 

默认启动软件将启动所有服务


##### Screenshots
托盘：  
<img src="./Screenshots/img2.png" />  
主界面：  
<img src="./Screenshots/img1.png" width="584px" />  
安装访问：  
<img src="./Screenshots/img3.png" width="578px" />

##### 集成扩展
```cli
PS D:\workspace> php -m
[PHP Modules]
apcu
bcmath
bz2
calendar
Core
ctype
curl
date
dom
exif
FFI
fileinfo
filter
ftp
gd
gettext
gmp
hash
iconv
imagick
imap
intl
json
ldap
libxml
mbstring
mongodb
mysqli
mysqlnd
mcrypt
odbc
openssl
pcre
PDO
pdo_mysql
Phar
readline
redis
Reflection
session
SimpleXML
sockets
SPL
standard
tokenizer
xml
xmlreader
xmlwriter
xsl
zip
zlib

[Zend Modules]
opcache
xdebug
```

软件最初内部用的，适合开发线上项目本地调试的，省去每次提交到服务器调试，所以不带MYSQL



