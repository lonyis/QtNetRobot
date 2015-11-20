#include "boyermoore.h"

void BoyerMoore::PreBmBc(char *pattern, int m, int bmBc[])
{
    int i;
    for (i = 0; i < MAX_CHAR; i++) {
        bmBc[i] = m;
    }

    for (i = 0; i < m - 1; i++) {
        bmBc[pattern[i]] = m - 1 - i;
    }
}

void BoyerMoore::suffix_old(char *pattern, int m, int suff[])
{
    int i, j;

    suff[m - 1] = m;

    for (i = m - 2; i >= 0; i--) {
        j = i;
        while (j >= 0 && pattern[j] == pattern[m - 1 - i + j]) j--;

        suff[i] = i - j;
    }
}

void BoyerMoore::suffix(char *pattern, int m, int suff[])
{
    int f, g, i;

    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + m - 1 - f])
                --g;
            suff[i] = f - g;
        }
    }
    //   print(suff, m, "suff[]");
}

void BoyerMoore::PreBmGs(char *pattern, int m, int bmGs[])
{
    int i, j;
    int suff[SIZE];

    // 计算后缀数组
    suffix(pattern, m, suff);

    // 先全部赋值为m，包含Case3
    for (i = 0; i < m; i++) {
        bmGs[i] = m;
    }
    // Case2
    j = 0;
    for (i = m - 1; i >= 0; i--) {
        if (suff[i] == i + 1) {
            for (; j < m - 1 - i; j++) {
                if (bmGs[j] == m)
                    bmGs[j] = m - 1 - i;
            }
        }
    }
    // Case1
    for (i = 0; i <= m - 2; i++) {
        bmGs[m - 1 - suff[i]] = m - 1 - i;
    }
}

int *BoyerMoore::BoyerMoores(char *pattern, int m, char *ntext, int n)
{
    char *text = (char *)calloc(MAX_CHAR, sizeof(char *));
    sprintf(text," %s", ntext);
    int i, j, bmBc[MAX_CHAR], bmGs[SIZE];

    // Preprocessing
    PreBmBc(pattern, m, bmBc);
    PreBmGs(pattern, m, bmGs);

    // Searching
    int *find = (int *)calloc(20, sizeof(int));
    int len = 0;
    //
    j = 0;
    while (j <= n - m) {
        for (i = m - 1; i >= 0 && pattern[i] == text[i + j]; i--);
        if (i < 0) {
            //printf("Find it, the position is %d\n", j);

            // next find
            find[len] = j;
            len ++;

            j += bmGs[0];
        } else {
            j += MAX(bmBc[text[i + j]] - m + 1 + i, bmGs[i]);
        }
    }
    free(text);
    return find;
}
