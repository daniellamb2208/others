#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int n, i, j, k, index = 0;
    scanf("%d", &n);

    int data[n][2], step[n];
    for (i = 0; i < n; i++)
        scanf("%d%d", &data[i][0], &data[i][1]);

    int pm[2];
    pm[0] = data[0][0];
    pm[1] = data[0][1];
    for (i = 1; i < n; i++)
    {
        if (data[i][0] > pm[0] || (data[i][0] == pm[0] && data[i][1] > pm[1]))
        {
            pm[0] = data[i][0];
            pm[1] = data[i][1];
            index = i;
        }
    }step[0] = index;

    long long v[2] = {0, 1};
    long long vmin[2] = {0, -1};
    long long w[2];

    long long p[2], q[2], pi[2];
    p[0] = pi[0] = pm[0];
    p[1] = pi[1] = pm[1];

    for (i = 0; i < n; i++)         //step2 repeate O(n^2)
    {
        index = 0;
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < i; k++)
                if (step[k] == j)
                    continue;

            q[0] = data[j][0];
            q[1] = data[j][1];
            w[0] = q[0] - pi[0];
            w[1] = q[1] - pi[1];

            if ((v[0] * w[1] - v[1] * w[0]) > 0)
            {
                if ((w[0] * vmin[1] - w[1] * vmin[0]) > 0)
                {
                    p[0] = q[0];
                    p[1] = q[1];
                    vmin[0] = w[0];
                    vmin[1] = w[1];
                    index = j;
                }
                else if ((w[0] * vmin[1] - w[1] * vmin[0]) == 0)
                {
                    if (sqrt(w[0] * w[0] + w[1] * w[1]) > sqrt(vmin[0] * vmin[0] + vmin[1] * vmin[1]))
                    {
                        p[0] = q[0];
                        p[1] = q[1];
                        vmin[0] = w[0];
                        vmin[1] = w[1];
                        index = j;
                    }
                }
            }
        }
        v[0] = p[0] - pi[0];            //reset
        v[1] = p[1] - pi[1];
        vmin[0] = v[0]*-1;
        vmin[1] = v[1]*-1;
        pi[0] = p[0];
        pi[1] = p[1];
        step[i + 1] = index;
        if (index == step[0])
            break;
    }
    printf("%d\n",i+2);             //times
        for (k = 0; k <= i; k++)
            printf("%d ", step[k] + 1);
    printf("%d", step[0]+1);        //origin
    return 0;
}