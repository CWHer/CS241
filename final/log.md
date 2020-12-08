## 版本记录

- [ ] 美化欢迎界面
- [ ] 图标
- [ ] 支持后期删除和加入数据

- [ ] 主界面布局

- [ ] 数据选择

- [ ] 数据可视化
- [ ] 线图/饼图
- [ ] 插值
- [ ] 地图绘制
- [ ] elective：thermal/direction
- [ ] elective：route plan (单源最短路)
- [ ] 多线程计算

- [ ] 多线程读入

- [ ] 二分查找grid
- [ ] thread笔记加入notes

### 开发中

#### 模块列表

| 名称          | 作用                               | 函数                     |
| ------------- | ---------------------------------- | ------------------------ |
| MainWindow    | 主界面                             |                          |
| WelcomeWidget | 欢迎界面，选择数据，调用LoadThread | getPath/setBarValue/load |
| LoadThread    | 分支线程，读取数据，完成后切换窗口 | run                      |
| DataBase      | 存储数据的类                       |                          |
|               |                                    |                          |
|               |                                    |                          |
|               |                                    |                          |
|               |                                    |                          |
|               |                                    |                          |

#### Ver 0.1

`welcome widget`

欢迎界面，可以选择数据导入，有进度条显示进度

`mainwindow`

导入数据完成关闭`welcome`，打开`mainwindow`

#### Ver 0.11

导入数据支持使用另一个线程



主界面左侧筛选数据

右侧画图，分为空间地图+线图+数据分析试图三个部分

画图区域使用setxxxPlot生成









---

### 开发完成

#### Ver1.0

