
(1)git init 初始化项目

(2)ssh -T git@github.com 连接远程的github

ssh  user@ip

(3)其他重要命令
  git config --global alias.d diff 比较命令
  git config --global alias.st status 查看状态
  git config --global alias.ds 'diff --staged' 查看staged状态
  git reset /src/scanner.l 重置文件
  git add + 文件名
  git commit -m 'add Makefile and .gitignore' 添加提交注释
  git log查看提交的日志
  git show查看提交的修改
  tree 查看目录及文件的树形结构
  
  (4) 打版本
  git pull
  git commit -m "message"
  git push
  git tag 查看版本
  git tag -a t2.2.2 -m "message"
  git push --tags

  (5)分支操作
  git branch test //新建分支
  git checkout test //切换分支
  git remote add origin  + 文件名.git  //和远程的分支关联
  git push origin test //上传分支
  (6)其他linux下常用命令
   ps -ef | grep + 进程名字
   查看特定端口号  netstat -anp | grep + 端口号
   查看进程的目录 ll/proc/进程pid

   (7)vim 一些命令
  删除特定行--- :1,20d  删除1到20行
  粘贴模式-- :set paste  使用这个命令粘贴不会乱



  





