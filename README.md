# DBoS-Desktop  ![](https://camo.githubusercontent.com/a7dfab63cad83ead05e6a425453d985c264de1061186ceba09bad749d2f6c058/68747470733a2f2f692e696d6775722e636f6d2f666538356156522e706e67)


![](https://img.shields.io/badge/license-GNU%20GPLV3-blue) ![](https://img.shields.io/badge/Linux-4.9.88-blue) ![](https://img.shields.io/badge/Qt-5.14.2-blue) ![](https://img.shields.io/badge/npm-v1.0.2-blue) ![](https://img.shields.io/badge/circleci-passing-green) ![](https://img.shields.io/badge/PRs-welcome-green)

[B站演示视频](https://www.bilibili.com/video/BV1xL4y137Qz)

![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/all.png?raw=true)
![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/desktop1.png?raw=true)
![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/desktop2.png?raw=true)

[B站演示视频](https://www.bilibili.com/video/BV1xL4y137Qz)

# 项目背景

我是以单片机开发入门，后续又做了 Qt 相关工作，有时间后开始进行 linux 相关的学习，恰巧在二一年十一月份，百问网的韦东山老师进行了三个月的 linux 驱动直播，于是有了开发 DBoS 的念头

# 项目介绍

这是一个基于 100ASK_IMX6ULL_PRO 开发板的综合性展示项目，包含上层应用和 linux 设备驱动两大部分。其中上层应用由 Qt 实现（5.14.2, qwidget, qss）, 并在代码结构上对每个 app 进行了界限分明的切割，很容易完成移植。linux 驱动也采用了最简洁明了的方式进行编写

```
应用程序包含：相机、音乐播放器、计算器、天气、系统信息、录音机（语音识别）、背光控制、视频播放器、OLED显示（数字手写体识别）、红外遥控器、超声波测距、光敏（ADC）、热红外、光照（环境传感器）、按键、地图、温湿度
```

 `PS：欢迎大家将 DBos 移植到其他开发板`

# 文档结构

`"1. 快速试运行"`: 包含 DBoS 快速上手运行所需要的资源及配置文件

`"2. GUI 源码"`: 包含 Qt 应用程序的完整源码，可依此文件进行二次开发

`"3. 驱动源码"`: 包含 linux 设备驱动的完整源码，可依此进行二次开发


# 快速上手【开发板运行环境搭建】

> DBoS 基于 100ASK_IMX6ULL_PRO， 要成功运行，那么需要一块带有屏幕的开发板，同时假定已经完成 [Linux 系列教程（第一篇） - 开发板基础操作](https://www.100ask.net/detail/p_60ff69a7e4b0a27d0e363587/8) 中所提到的内容
> 
> ps : app 退出请按开发板左下角物理按键

使用 `"1. 快速试运行"` 中提供的文件进行以下操作后重启开发板

* 替换文件 `/boot/100ask_imx6ull-14x14.dtb`

![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/1.png?raw=true)

* 替换文件 `/etc/profile`

![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/3.png?raw=true)

* 替换文件 `/etc/init.d/S99myirhmi2`
* 替换文件 `/etc/init.d/S09modload`

![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/4.png?raw=true)

* 复制以下文件夹到开发板 `/` 目录

```
DBoS : 可执行文件
video : mp4 视频文件
music : mp3 音乐文件
driver: 驱动文件
```

![](https://github.com/cocowts/DBoS-Desktop/blob/main/image/2.png?raw=true)

# 二次开发

* [100ASK_IMX6ULL_PRO开发板QT移植过程说明](https://segmentfault.com/a/1190000040786250)
* 开发板支持鼠标，但需要修改 `/etc/profile/S99myirhmi2` 文件（查看提供的 profile 中注释即可），同时需要注意在 Qt 启动前插入鼠标
* 天气、录音机、地图应用程序使用的百度API，请申请自己的KEY和ID在Qt工程中进行替换（查看对应的 `.h` 文件中注释即可）

```
关于页面卡顿：
1. IMX6ULL单核A7,主频800M,无GPU
2. Qt 不能只算一个GUI， 而是一个功能齐全的开发框架，这也带来了较大的资源开销，是LVGL等不能全等比较的
3. 以上两条导致在播放视频以及页面组件较多的时候，卡顿更为明显
4. 正点原子7寸LCD分辨率 1024*600 ,野火7寸LCD分辨率800*480，视频中展示所用的百问网LCD分辨率1200*600
```

# 联系方式

邮箱：1508539502@qq.com 
微信：18805171729（请备注：DBoS）

# 最后的话

> **眼有星辰大海，归来仍是少年，祝大家都学有所成**。 附上 DBoS 涉及的主要参考资料

* 视频
  * [零基础 C 语言入门课程](https://www.bilibili.com/video/BV1F4411275J?spm_id_from=333.999.0.0)
  * [C 语言进阶课程](https://www.bilibili.com/video/BV1mE411v7sp?spm_id_from=333.999.0.0)
  * [C++ 入门到进阶全过程](https://www.bilibili.com/video/BV17E411e7Mp?spm_id_from=333.999.0.0)
  * [C++ 新标准 C++11/14](https://segmentfault.com/a/1190000022130585)
  * [Qt 实验分析教程](https://www.bilibili.com/video/BV1SJ411S72H?spm_id_from=333.999.0.0)
  * [Linux 系列教程（第一篇） - 开发板基础操作](https://www.100ask.net/detail/p_60ff69a7e4b0a27d0e363587/8)
  * [Linux 系列教程（第五篇） - 嵌入式Linux驱动开发基础知识](https://www.100ask.net/detail/p_5f1aa2dde4b0df48afbd775f/8)
  * [70天30节Linux驱动开发快速入门系列课程](https://www.bilibili.com/video/BV1Yb4y1t7Uj?spm_id_from=333.999.0.0)

* 文档
  * 《嵌入式Linux应用开发完全手册V4.0_韦东山全系列视频文档-IMX6ULL开发板》
  * [Qss 基础资料整理](https://segmentfault.com/a/1190000023364609)
  * [100ASK_IMX6ULL_PRO开发板QT移植过程说明](https://segmentfault.com/a/1190000040786250)

