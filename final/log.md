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

- [ ] 多线程加快读入

- [ ] 二分查找grid
- [ ] 支持后期加入删除数据集

### 开发中

#### 模块列表

| 名称          | 作用                               | 函数                     |
| ------------- | ---------------------------------- | ------------------------ |
| MainWindow    | 主界面                             |                          |
| WelcomeWidget | 欢迎界面，选择数据，调用LoadThread | getPath/setBarValue/load |
| LoadThread    | 分支线程，读取数据，完成后切换窗口 | run                      |
| DataBase      | 存储数据的类                       |                          |
| SpatialPlot   | 用于生成绘制空图案的widget         | setupLayouts             |
| TimePlot      | 绘制时间相关内容                   |                          |
| InfoPlot      | 绘制其它重要信息                   |                          |
|               |                                    |                          |
|               |                                    |                          |

#### Ver 0.1

`welcome widget`

欢迎界面，可以选择数据导入，有进度条显示进度

`mainwindow`

导入数据完成关闭`welcome`，打开`mainwindow`

#### Ver 0.11

导入数据支持使用另一个线程

#### Ver 0.2

刚完成简陋的布局，后端逻辑没有任何进度

主界面使用`QTabWidget`管理多个窗口，画图区域使用xxxPlot类生成布局

空间地图：左侧筛选数据，右侧空间地图

时间绘图：上部给出一些按钮，下部绘图

重要信息绘图：左侧选择信息，右侧绘制

#### Ver 0.21

`todo`

- [ ] 重写Ver0.11引入的另一个线程类，并入DateBase

#### Ver 0.3

`todo`

- [ ] 完成线图和饼图

#### Ver 0.4

`todo`

- [ ] 完成其它信息绘制











---

### 开发完成

#### Ver1.0

