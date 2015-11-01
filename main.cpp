#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <fstream>

using namespace std;

const int Gallop = 7;

void TimSort(int N, int * A);
int GetMinrun(int n);
void merge(int i, int j, int k, int * A);

int main()
{
    FILE * fin = fopen("input.txt", "r");
    FILE * fout = fopen("output.txt", "w");
    int elem, n = 0;
    int * A;
    fscanf(fin, "%d", &elem);
    A = (int *) malloc(sizeof(int) * 100000);
    while(fscanf(fin, "%d", &elem) == 1){
        A[n] = elem;
        ++n;
    }
    TimSort(n, A);
    for (int i = 0; i < n; ++i){
        fprintf(fout, "%d ", A[i]);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}

int GetMinrun(int n){
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

void merge(int i, int j, int k, int * A){
    int * H;
    int g = 0;
    int last;
    int L, R, M;
    if (j - i <= k - j){
        H = (int *) malloc(sizeof(int) * (j - i));
        for (int c = i; c < j; ++c){
            H[c - i] = A[c];
        }
        int a = 0, b = 0;
        last = -1;
        while (a < j - i && b < k - j){
            if (H[a] <= A[b + j]){
                A[i + a + b] = H[a];
                ++a;
                if (last == 1)
                    ++g;
                else
                    g = 1;
                last = 1;
                if (g == Gallop){
                    L = a;
                    R = j - i;
                    while (R - L > 1){
                        M = (R + L) / 2;
                        if (H[M] <= A[b + j])
                            L = M;
                        else
                            R = M;
                    }
                    while (a < L){
                        A[i + a + b] = H[a];
                        ++a;
                    }
                }
            }
            else{
                A[i + a + b] = A[b + j];
                ++b;
                if (last == 2)
                    ++g;
                else
                    g = 1;
                last = 2;
                if (g == Gallop){
                    L = b + j;
                    R = k;
                    while (R - L > 1){
                        M = (R + L) / 2;
                        if (A[M] <= H[a])
                            L = M;
                        else
                            R = M;
                    }
                    while (b + j < L){
                        A[i + a + b] = A[b + j];
                        ++b;
                    }
                }
            }
        }
        while (a < j - i){
            A[i + a + b] = H[a];
            ++a;
        }
        while (b < k - j){
            A[i + a + b] = A[b + j];
            ++b;
        }
    }
    else{
        H = (int *) malloc(sizeof(int) * (k - j));
        for (int c = j; c < k; ++c){
            H[c - j] = A[c];
        }
        last = -1;
        int a = j - i - 1, b = k - j - 1, ab = k - 1;
        while (a >= 0 && b >= 0){
            if (H[b] >= A[a + i]){
                A[ab] = H[b];
                --b;
                --ab;
                if (last == 1)
                    ++g;
                else
                    g = 1;
                last = 1;
                if (g == Gallop){
                    L = -1;
                    R = b;
                    while (R - L > 1){
                        M = (R + L) / 2;
                        if (H[M] >= A[a + i])
                            R = M;
                        else
                            L = M;
                    }
                    while (b > R){
                        A[ab] = H[b];
                        --b;
                        --ab;
                    }
                }
            }
            else{
                A[ab] = A[a + i];
                --a;
                --ab;
                if (last == 2)
                    ++g;
                else
                    g = 1;
                last = 2;
                if (g == Gallop){
                    L = a - 1;
                    R = a + i;
                    while (R - L > 1){
                        M = (R + L) / 2;
                        if (H[b] < A[M])
                            R = M;
                        else
                            L = M;
                    }
                    while (a > R){
                        A[ab] = A[a + i];
                        --a;
                        --ab;
                    }
                }
            }
        }
        while (a >= 0){
            A[ab] = A[a + i];
            --a;
            --ab;
        }
        while (b >= 0){
            A[ab] = H[b];
            --b;
            --ab;
        }
    }
    free(H);
}

void TimSort(int N,int * A){
    int now = 0, i, j;
    int minrun = GetMinrun(N);
    int * L = (int *) malloc(sizeof(int) * (N / minrun + 1));
    int * R = (int *) malloc(sizeof(int) * (N / minrun + 1));
    int top = 0;
    int * Sl;
    int * Sr;
    int X, Y, Z;
    int s = 0, _s = 0, a, b, c;
    int run_l = 0, run_r;
    while (run_l < N){
        run_r = run_l + 1;
        if (run_r < N){
            if (A[run_l] <= A[run_r]){
                while (run_r < N && (A[run_r - 1] <= A[run_r])){
                    ++run_r;
                }
                while (run_r < N && run_r - run_l <= minrun){
                    ++run_r;
                }
            }
            else{if (A[run_l] > A[run_r]){
                while (run_r < N && (A[run_r - 1] > A[run_r])){
                    ++run_r;
                }
                for (int _ = 0; 2 * _ < run_r - run_l; ++_){
                    swap(A[run_l + _], A[run_r - _ - 1]);
                }
                while (run_r < N && run_r - run_l <= minrun){
                    ++run_r;
                }
            }}
        }
        for (a = run_l; a < run_r; a++){
            b = a;
            while (b > run_l && A[b] < A[b - 1]){
                swap(A[b], A[b - 1]);
                b--;
            }
        }
        L[now] = run_l;
        R[now] = run_r;
        ++now;
        while (now > 2){
            X = R[now - 1] - L[now - 1];
            Y = R[now - 2] - L[now - 2];
            Z = R[now - 3] - L[now - 3];
            if (X > Y + Z && Y > Z)
                break;
            if (X <= Z){
                merge(L[now - 2], L[now - 1], R[now - 1], A);
                R[now - 2] = R[now - 1];
                --now;
            }
            else{
                merge(L[now - 3], L[now - 2], R[now - 2], A);
                R[now - 3] = R[now - 2];
                L[now - 2] = L[now - 1];
                R[now - 2] = L[now - 1];
                --now;
            }
        }

        run_l = run_r;
    }
    while (now > 1){
        merge(L[now - 2], L[now - 1], R[now - 1], A);
        R[now - 2] = R[now - 1];
        --now;
    }
}
