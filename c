#!/bin/bash
. /etc/init.d/functions
MMM=`rpm -qa | grep mariadb`
cat <<END 
+----------------------------------+
|                                  |
|         This  is  a LNMP         |
|                                  |
|         1.安装Nginx              |
|         2.安装MySQL              |
|         3.安装PHP                |
|         4.配置LNMP环境           |
+----------------------------------+
END
#node 1. 
  read  -p "请你输入一个数字:" NUM
  expr $NUM + 1 &> /dev/null
  if [ "$?" -ne 0 ];then
    action "对不起，请你输入整数！！！" /bin/false
    exit 1
  elif [ "$NUM" -eq 0 ];then
    action "对不起，请你输入比0大的数字！！！" /bin/false
    exit 1
  fi
#node 2.
 [ "$NUM" = "1" ] && {
  echo  "开始安装Nginx服务，请稍后.........."
  yum install gcc gcc-c++ pcre-devel zlib-devel openssl-devel -y &>/dev/null && useradd -M -s /sbin/nologin nginx && cd /usr/local/src && wget http://nginx.org/download/nginx-1.12.2.tar.gz &>/dev/null && tar zxf nginx-1.12.2.tar.gz && \
cd /usr/local/src/nginx-1.12.2 && ./configure --prefix=/usr/local/nginx --with-http_dav_module   --with-http_stub_status_module --with-http_addition_module --with-http_sub_module --with-http_flv_module --with-http_mp4_module --with-http_ssl_module --with-http_gzip_static_module --user=nginx --group=nginx &>/dev/null && make &>/dev/null && make install &>/dev/null
   if  [ -f /usr/local/nginx/sbin/nginx ];then
           ln -s /usr/local/nginx/sbin/nginx /usr/local/sbin
   fi
   if [ "$?" -eq 0 ];then
 
          action "恭喜你，成功安装Nginx！！！" /bin/true && /usr/local/nginx/sbin/nginx
          else
          action "对不起，安装Nginx失败! ! !" /bin/false
          exit 1
   fi
}
#node 3.
 [ "$NUM" = "2" ] && {
    echo "开始安装MySQL服务，请稍后........"
	yum -y install  autoconf automake zlib* libxml* ncurses-devel libtool-ltdl-devel* make cmake
   if [ -n $MMM ];then
     rpm  -e mariadb-libs --nodeps 2&> /dev/null
    else
     action "mariadb卸载失败！！！" /bin/false
     exit 1
   fi
cd /usr/local/src/ && wget http://mirrors.sohu.com/mysql/MySQL-5.7/mysql-5.7.25-linux-glibc2.12-x86_64.tar.gz &>/dev/null  && tar zxfm  mysql-5.7.25-linux-glibc2.12-x86_64.tar.gz && mv mysql-5.7.25-linux-glibc2.12-x86_64 /usr/local/mysql && cd /usr/local/mysql/ && mkdir data && mkdir log && echo "export PATH=$PATH:/usr/local/mysql/bin" >> /etc/profile && source /etc/profile && groupadd mysql && useradd -r -g mysql mysql
cat << EOF > /etc/my.cnf
[client]      
socket=/usr/local/mysql/mysql.sock      
[mysqld]      
basedir=/usr/local/mysql      
datadir=/usr/local/mysql/data      
pid-file=/usr/local/mysql/data/mysqld.pid      
socket=/usr/local/mysql/mysql.sock      
log_error=/usr/local/mysql/log/mysql.err 
EOF
   if [ -f /etc/my.cnf ];then
    chmod 750 data/ && chown -R mysql . && chgrp -R mysql . &&  bin/mysqld --initialize --user=mysql && cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysqld && service mysqld start &> /dev/null
   else
     echo "MySQL安装失败！！！！"
     exit 1
  fi
ln -s /usr/local/mysql/bin/mysql /usr/local/sbin
ln -s /usr/local/mysql/bin/mysqladmin /usr/local/sbin
oo=`cat /usr/local/mysql/log/mysql.err | grep password | awk '{print $11}'`
mysqladmin -uroot -p${oo} password pwd123 &>/dev/null
 if [ "$?" -eq 0 ];then
   action "MySQL数据库安装成功,密码为:pwd123" /bin/true
   else
  action "MySQL数据库安装失败密码初始化失败！" /bin/false
  exit 1
 fi
}
#node 4.
[ "$NUM" = "3" ] && {
    echo "开始安装PHP依赖包................"
 yum -y install libxml2-devel libcurl-devel openssl-devel bzip2-devel libjpeg-devel libpng libpng-devel freetype-devel libmcrypt libmcrypt-devel &>/dev/null
  if [ "$?" -eq 0 ];then
    action "恭喜你，成功安装PHP依赖包！！！！" /bin/true
    else
    action "对不起,PHP依赖包安装失败！！！！" /bin/false
    exit 1
  fi
cd /usr/local/src && {
   echo  "开始安装PHP请稍后,过程稍微有点长..........."   
   wget https://www.php.net/distributions/php-7.1.31.tar.gz &> /dev/null &&  tar zxf php-7.1.31.tar.gz  && cd php-7.1.31/ && ./configure --prefix=/usr/local/php --with-config-file-path=/usr/local/php/ --enable-fpm  --with-mysqli=mysqlnd --with-pdo-mysql=mysqlnd --with-iconv-dir --with-freetype-dir --with-jpeg-dir --with-png-dir --with-zlib --with-libxml-dir=/usr --enable-xml --disable-rpath --enable-bcmath --enable-shmop --enable-sysvsem --enable-inline-optimization --with-curl --enable-mbregex --enable-mbstring --with-mcrypt --enable-ftp --with-gd --enable-gd-native-ttf --with-openssl --with-mhash --enable-pcntl --enable-sockets --with-xmlrpc --enable-zip --enable-soap --without-pear --with-gettext --disable-fileinfo --enable-maintainer-zts  &>/dev/null && make &>/dev/null && make install &>/dev/null
}
if [ "$?" -eq 0 ];then
     action "恭喜你，PHP安装成功！！！！" /bin/true
   else
     action "PHP安装失败,请检查环境..." /bin/false
     exit 1
fi
cd /usr/local/src/php-7.1.31/ && {
echo "开始配置PHP，请稍后..............."       
        cp /usr/local/src/7.1.31/php.ini-production /usr/local/php/php.ini
		cp /usr/local/src/7.1.31/sapi/fpm/init.d.php-fpm /etc/init.d/php-fpm
       chmod +x /etc/init.d/php-fpm
       chkconfig --add php-fpm
       chkconfig php-fpm on
}
   if [ "$?" -eq 0 ];then
      cp /usr/local/php/etc/php-fpm.d/www.conf.default /usr/local/php/etc/php-fpm.conf
      else
      action "对不起，PHP配置出现严重错误！！！！" /bin/false
      exit 1
   fi
   if [ ! -d /usr/local/php/etc/php-fpm.conf ];then
   sed -i 's/user = nobody/user = nginx/g' /usr/local/php/etc/php-fpm.conf
   sed -i 's/group = nobody/group = nginx/g' /usr/local/php/etc/php-fpm.conf
    else
     action "对不起，PHP环境配置出错！！！！" /bin/false
     exit 1
   fi
   if [ "$?" -eq 0 ];then
    action "恭喜你PHP配置启动成功！！！！" /bin/true &&  systemctl start php-fpm
    else
    action  "对不起，PHP配置启动失败！！！" /bin/false
    exit 1
  fi
}
#node 5.
[ "$NUM" = "4" ] && {
    echo "开始配置Nginx解析PHP，请稍后.............." 
    if [ -f /usr/local/nginx/conf/nginx.conf ];then
       sed -i 's/index  index.html index.htm;/index  index.php index.html index.htm;/g' /usr/local/nginx/conf/nginx.conf
       sed -i 's/#    root           html;/    root           html;/g' /usr/local/nginx/conf/nginx.conf
       sed -i 's/#    fastcgi_pass   127.0.0.1:9000;/    fastcgi_pass   127.0.0.1:9000;/g' /usr/local/nginx/conf/nginx.conf
       sed -i 's/#    fastcgi_index  index.php;/    fastcgi_index  index.php;/g' /usr/local/nginx/conf/nginx.conf
       sed -i 's*#    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;*    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;*g' /usr/local/nginx/conf/nginx.conf
       sed -i 's/#    include        fastcgi_params;/    include        fastcgi.conf;/g' /usr/local/nginx/conf/nginx.conf
       sed -i '71d' /usr/local/nginx/conf/nginx.conf && sed -i 'N;70a}' /usr/local/nginx/conf/nginx.conf
       sed -i '65d' /usr/local/nginx/conf/nginx.conf && sed -i 'N;64alocation ~ \\.php$ {' /usr/local/nginx/conf/nginx.conf
     else
       action "Nginx解析PHP失败！！！" /bin/false
       exit 1
   fi
echo "开始创建PHP测试页................"       
cat << EOF >/usr/local/nginx/html/dog1.php 
<?php  
phpinfo();  
?>  
EOF
echo "开始创建MySQL测试页................"  
cat << EOF >/usr/local/nginx/html/dog2.php
<?php
$link=mysql_connect('127.0.0.1','root','pwd123');
if($link)echo "connection success......";
mysql_close();
?>
EOF
/usr/local/nginx/sbin/nginx -s reload
if [ "$?" -eq 0 ];then
   action "恭喜你LNMP环境搭建成功！！！" /bin/true
   else
   action "对不起，您的LNMP环境搭建失败！！！！" /bin/false
   exit 1
fi
}
