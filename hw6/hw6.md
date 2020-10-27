Given that

> quarter:25/dime:10/nickel:5/penny:1

#### a. 

```c++
INPUT:money
    
int p[]={25,10,5,1};
int ans[4];

for i=0 to 3
{
    ans[i]=money/p[i];
    money-=ans[i]*p[i];
}    

OUTPUT:ans[4]
```

**proof**:

For the optimal solution, we can guarantee there's no more than 4 pennies, otherwise one more nickel is more efficient. 

Similarly, we can find there's no more than 1 nickel($10=2\times5$) and no more than 2 dimes($3\times10\ >25+5,40=25+5\times3,5\times10>25\times2$). Besides, 1 nickel and 2 dimes can't exist at the same time or a quarter will be fine. **In conclusion**, the sum of all dimes, nickels and pennies will not exceed a quarter.

Thus, we can assume $money=k\times25+r$, where k is the number of quarters and r ranges from 0 to 24.

For each r in [0,24], we can easily prove that greedy algorithm gives an optimal solution. Thus we can guarantee that greedy algorithm always yields an optimal solution.

#### b.

**proof**:

For the optimal solution, we can guarantee there's no more than $c$ same coin $c^i$  (excluding $c^{k}$) or one more coin $c^{i+1}$  is enough.

When $money>c^{k}$, as there is no better alternative that take $c^{k}$ , we can first take at much $c^{k}$ as possible.

Now we have $money<c^{k}$. With the numbers of each rest coins less than c, it turns out that the optimal solution is just $money$ **represented in c-base field**. While greedy algorithm does the same thing as calculating $money$'s c-base representation, it always yields the optimal solution.

#### c.

denominations:1,5,7, with input=10

The greedy algorithm would result in {3,0,1}, while {0,2,0} is the optimal solution which we can calculate using dynamic programming.

