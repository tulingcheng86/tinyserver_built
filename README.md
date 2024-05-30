# tinyserver_built

基于https://github.com/qinguoyi/TinyWebServer

复刻TinyWebServer记录



[个人对这个项目的学习理解记录](LEAREN.md)



# 运行

```
sudo apt-get install mysql-server
sudo mysql_secure_installation
```

配置项如下

```shell
#1
VALIDATE PASSWORD PLUGIN can be used to test passwords...
Press y|Y for Yes, any other key for No: N (我的选项)
#2
Please set the password for root here...
New password: (输入密码)
Re-enter new password: (重复输入)
#3
By default, a MySQL installation has an anonymous user,
allowing anyone to log into MySQL without having to have
a user account created for them...
Remove anonymous users? (Press y|Y for Yes, any other key for No) : N (我的选项)
#4
Normally, root should only be allowed to connect from
'localhost'. This ensures that someone cannot guess at
the root password from the network...
Disallow root login remotely? (Press y|Y for Yes, any other key for No) : Y (我的选项)
#5
By default, MySQL comes with a database named 'test' that
anyone can access...
Remove test database and access to it? (Press y|Y for Yes, any other key for No) : N (我的选项)
#6
Reloading the privilege tables will ensure that all changes
made so far will take effect immediately.
Reload privilege tables now? (Press y|Y for Yes, any other key for No) : Y (我的选项)
```



检查mysql状态

```shell
systemctl status mysql.service
```

进入mysql

```shell
sudo mysql -uroot -p
```



```shell
// 建立yourdb库
create database yourdb;

// 创建user表
USE yourdb;
CREATE TABLE user(
    username char(50) NULL,
    passwd char(50) NULL
)ENGINE=InnoDB;

// 添加数据
INSERT INTO user(username, passwd) VALUES('name', 'passwd');
```

修改main.cpp中的数据库初始化信息

```shell
   //数据库登录名,密码,库名
    string user = "root";
    string passwd = "sbim123";
    string databasename = "yourdb";
```



```
mkdir build 
cd build
cmake ..
make 
```



```shell
./Webserver
浏览器输入localhost:9006
```

