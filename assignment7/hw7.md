[TOC]

### 遗传算法

注：计算$f=-x\cdot sin(x)$的最大值

函数值直接作为fitness

- 编码方式

  16bit的unsigned保存编码

  通过
  $$
  \frac{x}{2^{12}}-1
  $$
  转化为[-1,15]的double

- 初始条件：随机生成40组16bit的随机数
- 终止条件：1000轮迭代

- 选择策略

  先将所有个体按照fitness降序排序

  记max_fit为所有迭代中出现的最大值，若个体符合以下条件则被淘汰
  $$
  sigmoid(cur\_fit/max\_fit)<0.55
  $$
  被淘汰的个体会随机填充为一个未被淘汰的个体/历史中最优的个体

- 交叉策略

  在经过一遍选择后按照值的大小$x$重新排序

  （值相差较大的两个个体产生后代是不合理的，因为两者区间的值内很可能里极值相去甚远。例如如果0和15杂交，那么几乎等于重新随机一个个体。因此我选择将值较为相近的个体组合）

  排序后将$2k$和$2k+1$组合，按照如下方式产生新个体

  - 设x和y杂交，分别记fitness为px,py

    对于新个体的每一位，以$\frac{px}{px+py}$的概率选择$x$的相应位，否则选择$y$的相应位

- 变异策略

  按照unsigned的规则，记最右侧为第0位，往左依次增加

  第k位以如下概率变异为一个相反的位
  $$
  p=\frac{1}{2^{k/2}+5}
  $$
  这样的规则可以保证低位容易变异而高位难以变异

**运行结果**

经过1000轮迭代，耗时大约500ms，一般可以获得11.085左右的解



### 编辑距离

$f[i][j]$为x已经处理完第i位，y/z已经匹配到第j位的最小cost

因为f[i]表示已经处理完第i位，所以f[m]即表示已经处理完x串

$s[i][j]$用于记录生成$f[i][j]$的操作，$pre[i][j]$用于记录生成$f[i][j]$前一个操作的位置

初始化$f[0][0]=0$，其余为$\infin$

**求最优解**

```c++
for i=1 to m
    for j=1 to n
        if y[j]==x[i]	//copy
            if f[i-1][j-1]+3<f[i][j]
                f[i][j]=f[i-1][j-1]+3
                s[i][j]="copy",pre[i][j]=(i-1,j-1)
            
        if f[i-1][j-1]+1<f[i][j]	//replace
        	f[i][j]=f[i-1][j-1]+1
            s[i][j]="replace",pre[i][j]=(i-1,j-1)
            
        if f[i-1][j]+1<f[i][j]		//delete
            f[i][j]=f[i-1][j]+1
            s[i][j]="delete",pre[i][j]=(i-1,j)
            
		if f[i][j-1]+4<f[i][j]		//insert
            f[i][j]=f[i][j-1]+4
            s[i][j]="insert",pre[i][j]=(i,j-1)
	
        if i>=2&&j>=2	//twiddle
            if x[i-1]==y[j]&&x[i]==y[j-1]
                if f[i-2][j-2]+1<f[i][j]
       				f[i][j]=f[i-2][j-2]+1
                    s[i][j]="twiddle",pre[i][j]=(i-2,j-2)

for i=1 to m 	//kill
    if f[i][n]+1<f[m][n]
        f[m][n]=f[i][n]+1
        s[m][n]="kill",pre[i][j]=(i,n)
             
OUTPUT:f[m][n]
```

**打印操作过程** 

通过倒序递归来顺序打印

```c++
print(m,n)
    
void print(int i,int j)
{
    if i==0&&j==0 
        return;
    print(pre[i][j].first,pre[i][j].second)
    cout << s[i][j];
}
```

时间和空间复杂度均为$O(N\times M)$