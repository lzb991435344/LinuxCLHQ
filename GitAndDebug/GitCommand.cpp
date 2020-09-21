
(1)git init 初始化项目

(2)ssh -T git@github.com 连接远程的github
生成公私钥，将公钥传到github  具体步骤看git官网教程使用方法
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
  
  git log --查看之前提交的修改分支
  git show + commitid --查看所有的修改

  合并分支基本命令
  git checkout -b slave01

  等价于下面两条命令
  git branch + git checkout + slave01

  git commit -m "xxxx"
  git checkout master 
  git merge slave01

  查看所有分支
  ps：先转到特定的目录下
  git branch -a 


  删除分支
  git branch -d hotfix

  目的：将master分支的修改转移到另一个分支
  （1）git stash 暂存
  （2）切换到另一个分支，执行 git stash pop ,在这个分支进行提交
  （3）切换回主分支，进行merge 分支


  撤销修改的几个场景
  （1）已修改，未add
  git checkout .  //清除修改，重新检出
  （2）已修改 + add
   git reset
   (3)清除未跟踪的文件
   git clean -fdx
   (4)已经提交到远程仓库的处理,重新后退版本
   git reset --soft HEAD^  ,则意味着将HEAD从顶端的commit往下移动到上一个commit
   (i).git log查看提交记录，确定自己的提交内容和要撤回的内容；
   (ii).git reset --soft HEAD^  ,则意味着将HEAD从顶端的commit往下移动到上一个commit
   (iii).git status   查看当前的修改状态，可以发现上次提交已经被撤回。
   (iiii).git reset HEAD xxx/xxx/xx.c    将你不想提交的文件进行撤回。
   (iiiii).再次git status  确认不想提交文件已经不在stage上。
   (iiiiii). 再次进行commit即可。



  git下载子模块
  （1）在根目录下执行
    git submudule init 
   (2)下载子模块
   git submodule update --recursive

  PS：子模块中提交的代码，需要在项目外再提交一次才能在拉取代码的时候拉取成功




  (6)其他linux下常用命令
   ps -ef | grep + 进程名字
   查看特定端口号  netstat -anp | grep + 端口号
   查询端口号内运行的文件目录 (ps -ef | grep 9014) ---- !ps  
   查看进程的目录 ll/proc/进程pid

   (7)vim 一些命令
  删除特定行--- :1,20d  删除1到20行
  粘贴模式-- :set paste  使用这个命令粘贴不会乱
  找到特定行-- :number
  通配符： ?+特定的字符 -----？error 
  
  （8）sz,rz的安装和配置 --上传合计下载服务器的文件工具
   配置网址：https://qq52o.me/2697.html





  





