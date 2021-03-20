/** 
 * @Author : Lu7fer
 * @Date: 2020-04-17 22:20:25
 * @LastEditTime: 2020-04-17 22:54:34
 * @FilePath: \C-language-study-homework\zhihu\06_erweishuzu.c
 * @Stu_ID: 2019X....X229_Lu7fer
 * @Github: https://github.com/Lu7fer/C-language-study-homework
 * @Copyright
-------------------------------------------
Copyright (C) 2020 - Lu7fer
C-language-study-homework is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.
 
You should have received a copy of the GNU General Public License along with C-language-study-homework. 
If not, see <http: //www.gnu.org/licenses/>.
-------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>

int main()
{
    int i, j, n;
    //可输入的分数上限和下限由min与max决定,不然可能有bug
    double sum, ave[2][5], x[5], tmp, max, min;
    printf("校园歌手大赛计分系统\n");
    for (n = 0; n < 10; n++)
    {
        max = 0.0, min = 100.0, sum = 0.0;
        printf("请输入评委的分数\n");
        for (i = 0; i <= 4; i++)
        {
            scanf("%lf", &x[i]);
            sum += x[i];
            max = (max < x[i]) ? x[i] : max;
            min = (min > x[i]) ? x[i] : min;
        }

        ave[n / 5][n % 5] = (sum - max - min) / 3.0;
        printf("该选手的平均分%6.2lf\n", ave[n / 5][n % 5] );
    }
    /* 转成一维数组引用*/
    double *aver = (double *)ave;
    /* 仔细一看就会发现二维数组实际上还是连续排列的,就比如这里用的ave[2][5]
            排列方式是
            [0][0]  [0][1]  [0][2]  [0][3]  [0][4]  [1][0]  [1][1]  [1][2]  [1][3]  [1][4] 
            转成一维数组的引用方式时:
            [0]  [1]  [2]  [3]  [4]  [5]  [6]  [7]  [8]  [9],
            而且一维数组更易排序
            二维数组在内存中的排列就是首位相连的一维数组可以将 [2][5]这样一个二维数组 看成两个 [5] 一维数组 依次连续排开
    */
    for (i = 0; i < 10; i++)
    {
        for (j = 1; j < 10 - i; j++)
        {
            if (aver[j - 1] < aver[j])
            {
                tmp = aver[j];
                aver[j] = aver[j - 1];
                aver[j - 1] = tmp;
            }
        }
    }

    /* 或者不那么做,当然代码变得很难看*/
    /*
    for (i = 0; i < 10; i++)
    {
        for (j = 1; j < 10 - i; j++)
        {
            n=j-1;
            if (ave[n / 5][n % 5] < ave[j / 5][j % 5])
            {
                tmp = ave[j / 5][j % 5];
                ave[j / 5][j % 5] = ave[n / 5][n % 5];
                ave[n / 5][n % 5] = tmp;
            }
        }
    }
*/
    printf("每位选手的得分:\n");
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 5; j++)
        {
            printf("%8.2lf  ", ave[i][j]);
        }
        printf("\n");
    }
    system("pause");
    return 0;
}
