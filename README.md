# OI

嘛, 这个是senioria打题的配置(纯Vim)以及一些源代码... 包括题目的解, 一些板子什么的...
push时间大概随缘唔...

## 对拍

直接改`check.cpp`, 加了shebang可以直接运行. 不过是*NIX限定.
如果要SPJ的话可以给`DataGen::SPJ`赋值.

Windows上可以用`check.py`, 好久没有改过了...

## Windows下的特殊设置

由于VS的编译器没有设置环境变量, 必须有python,
同时make.py下的`vcvars.bat`路径可能需要修改.

目前还没有办法自动停止服务器, 为了调试需要, 服务器需要手动启动.

