/* Name 我的MYSQL之旅
 * Author 王小晖
 * Date 20180707
 * Description 记录学习MYSQL时的遇到的问题。
 */
 
 http://www.runoob.com/mysql/mysql-tutorial.html
 
/* 完整的卸载Centos7下的MySQL */
{
	rpm -qa | grep -i mysql		/* 1.首先执行查看命令，看都安装了哪些东西 */
	
	yum remove mysql-community mysql-community-server mysql-community-libs mysql-community-common		/* 2.卸载命令 */
	
	rpm -qa | grep -i mysql 	/* 3.确认安装部分已完全卸载掉 */
	
	find / -name 'mysql'		/* 4.删除有关文件 */
}
 
 /* Centos7安装MySQL */
{
	/* 1.确认系统未安装mysql */

	/* 2.创建目录 */
	cd /usr/local
	mkdir -p tools
	mkdir -p mysql
	cd tools/
	
	/* 3.下载YUM源 */
	wget http://repo.mysql.com/mysql57-community-release-el7-8.noarch.rpm
	
	/* 4.安装.rpm */
	rpm -ivh mysql57-community-release-el7-8.noarch.rpm
	
	/* 5.安装mysql ,安装成功后通过 grep "password" /var/log/mysqld.log 查询root用户密码*/
	yum install mysql-community-server
	
	/* 6.在Linux里面安装mysql之后，找不到inlcude文件mysql.h */
	yum install mysql-devel
	
	/* 7.启动mysql服务 */
	systemctl start mysqld.service
}

/* 更改密码复杂性，将密码设置为简单 */
{
	/* step 1: 修改两个全局参数 */
	mysql> set global validate_password_policy=0;
	mysql> set global validate_password_length=1;
	/* step 2 */
	SET PASSWORD = PASSWORD('NEW PASSWORD');
	/* step 3 */
	ALTER USER 'root'@'localhost' PASSWORD EXPIRE NEVER;
	/* step 4 */
	flush privileges;
}
 
 /* 最基本操作 */
{
	mysql> CREATE DATABASE wxh	/* 创建数据库 */
	
	/* 使用数据库 */
	mysql> use wxh;
	$ mysql -u wxh -p wxh
		
	/* 创建表格 */
	mysql> CREATE TABLE children (
	-> childno INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,
	-> fname VARCHAR(30),
	-> age INTEGER
	-> );
	
	/* 查询表结构 */
	mysql> DESC table_name;
	
	/* 往表格中插入数据 */
	mysql> INSERT INTO children(fname,age) VALUES("Jenny",17);
	
	/* 数据查询 */
	SELECT column_name,column_name FROM table_name;
	/* 返回唯一(不重复)查询结果 */
	SELECT DISTINCT column_name,column_name FROM table_name;
	
}

/* 使用grant命令来创建用户并赋予权限 */
{
	/* 创建本地登录 - 用户名 wxh - 密码 1234 */
	mysql> GRANT ALL ON *.* TO wxh@localhost IDENTIFIED BY '1234';
	
	/* 创建来自C类子网192.168.0.0登录 */
	mysql> GRANT ALL ON *.* TO wxh@'192.168.0.0/255.255.255.0' IDENTIFIED BY '1234';	
	
	/* 创建能从wiley.com域中的任何机器登录 */
	mysql> GRANT ALL ON *.* TO wxh@'%.wiley.com' IDENTIFIED BY '1234';

	/* 刷新权限 */		
	mysql> FLUSH PRIVELEGES;
}

/* mysql的几个目录 */
{
	/* 1.数据库目录 */
	/var/lib/mysql | /var/lib64/mysql
	
	/* 2.配置文件 */
	/usr/share/mysql	/* mysql5.5之后的默认安装路径 */
	/usr/local/mysql	/* mysql5.5之前的默认安装路径 */
	
	/* 3.相关命令(mysqladmin、mysqldump等命令) */
	/usr/bin
	
	/* 4.启动脚本 */
	/etc/rc.d/init.d
}

/* 在已知MySQL数据库的ROOT用户密码的情况下，修改密码的方法 */
{
	/* 1.在SHELL环境下，使用mysqladmin命令设置 */
	mysqladmin -u root -p password "新密码"		/* 回车后要求输入旧密码 */
	
	/* 2.在 mysql> 环境中，使用update命令，直接更新mysql库user表的数据 */
	update mysql.user set password=password("新密码") where user='root';
	flush privileges;
	
	/* 3.在 mysql> 环境中，使用grant命令，修改root用户的授权权限 */
	grant all on *.* to root@'localhost' IDENTIFIED BY '新密码'；
}

/* 忘记MySQL数据库的ROOT用户密码 */
{
	/* 1.关闭当前运行的mysqld服务程序 */
	service mysqld stop
	
	/* 2.使用mysqld_safe脚本以安全模式(不加载授权表)启动mysqld服务 */
	/usr/local/mysql/bin/mysql_safe --skip-grant-table &
	
	/* 3.使用空密码的root用户登录数据库，重置ROOT用户的密码 */
	mysql -u root
	mysql> update mysql.user set password=password('新密码') where user='root';
	mysal> flush privileges;
	
}

/* MySQL删除表数据的方法 */
{
	/* 1.删除数据表 */
	mysql> DROP TABLE tableName;
	
	/* 2.清空表中的所有记录 */
	mysql> DELETE FROM tableName;		/* 方法一：返回被删除的记录数、删除速度慢 */
	mysql> TRUNCATE TABLE tableName;	/* 方法二：返回0、不记录mysql日志，不可以恢复数据、执行快 */
		/* 如果表中有自增字段，则删除所有记录后该字段值恢复为1 */
		mysql> DELETE FROM tableName WHERE 1; /* 保留自增字段值 */
	
	/* 3.删除表中的部分记录，只能是有DELETE语句 */
	DELETE FROM tableName WHERE ...;
		mysql> DELETE FROM users WHERE name = 'Mike' ORDER BY id DESC LIMIT 6;
}

/* 修改表中的数据 */
{
	/* update 表名 set 字段=新值,..where 条件 */
	mysql> UPDATE tableName SET name='Mary' WHERE id = 1;	
}

/* 查看数据库中所有表的创建时间--数据库(foo) */
{
	select * from INFORMATION_SCHEMA.TABLES where TABLE_SCHEMA = 'foo' order by create_time desc;
}

/* 调用.sql脚本 */
{
	source ".sql路径";	/* 路径不加引号 */
}

/* 从数据库导出数据 */
{
	/* 1.导出整个数据库 */
	mysqldump -u用户名 -p 数据库名 > target.sql;
	
	/* 2.导出一个表（包括表结构和数据） */
	mysqldump -u用户名 -p 数据库名 表名 > target.sql;
	
	/* 3.只导出表结构 */
	mysqldump -u用户名 -p -d 数据库名 表名 > target.sql;
}

/* DATE_FORMAT(date,format) 函数用法 */
{
	DATE_FORMAT(date,format) 函数根据format字符串格式化date值。
	/* 把字符串转为日期格式 */
	SELECT DATE_FORMAT('2018-08-29 08:30:45','%Y-%m-%d %H:%i:%s');
	/* 把日期转为字符串格式 */
	SELECT DATE_FORMAT(NOW(),'Y-%m-%d %H:%i:%s');
}

/* 查询结果导出 */
{
	/* 导出到文件（该文件应不存在） */
	SELECT * FROM tableName INTO OUTFILE "FilePath";
	/* 导出到其它表 */
	INSERT INTO tableName1 SELECT * FROM tableName2;	/* tableName1必须存在 */
	SELECT * INTO tableName1 FROM tableName2;	/* tableName1不存在 */
}

/* event自动任务 */
{
	https://www.cnblogs.com/qlqwjy/p/7954175.html
	https://blog.csdn.net/lixia755324/article/details/53923856
	
	/* 1.查看event是否启用 */
	SELECT @@event_scheduler;
	/* 2.启用定时任务 */
	SET GLOBAL event_scheduler = ON;
	/* 3.建立定时任务 */
	CREATE EVENT eventName
		ON SCHEDULE EVERY 5 MINUTE
	DO
		SELECT * FROM tableName;
	/* 4.查看事件 */
	SHOW events;	/* 查看当前所在库的事件 */
	SELECT * FROM mysql.event;	/* 查看所有事件 */
	/* 5.修改事件 */
	ALTER 
		[DEFINER = { user | CURRENT_USER }] 
		EVENT event_name 
		[ON SCHEDULE schedule] 
		[ON COMPLETION [NOT] PRESERVE] 
		[RENAME TO new_event_name] 
		[ENABLE | DISABLE | DISABLE ON SLAVE] 
		[COMMENT 'comment'] 
		[DO event_body]
	/* 设置事件为不活动和再次活动 */
	ALTER EVENT eventName DISABLE;
	ALTER EVENT eventName ENABLE;
	/* 6.删除事件 */
	DROP EVENT [IF EXISTS] eventName;
}

/* 事务 */
{
	/* 事务的四种特性：原子性、一致性、隔离性、持久性 */
	https://blog.csdn.net/chosen0ne/article/details/10036775
}

/* 数据更新 ON DUPLICATE KEY UPDATE */
{
	/* 如果在insert语句后面带上ON DUPLICATE KEY UPDATE 子句，而要插入的行与表中现有记录的惟一索引或主键中产生重复值，那么就会发生旧行的更新；如果插入的行数据与现有表中记录的唯一索引或者主键不重复，则执行新纪录插入操作。 */
	INSERT INTO tableName(column_name1,column_name2) values(value1,value2) ON DUPLICATE KEY UPDATE column_name2 = column_name2 + 1;
}

/* 修改表结构 */
{
	/* 添加字段 */
	ALTER TABLE tableName ADD COLUMN column_name varchar(64);
}

/* 连接MySQL的时候报1130的错误解决办法 */
{
	mysql > use mysql;
	mysql > select 'host' from user where user='root';
	mysql > update user set host='%' where user='root';
	mysql > flush privileges;
	mysql > select 'host' from user where user='root';
}



