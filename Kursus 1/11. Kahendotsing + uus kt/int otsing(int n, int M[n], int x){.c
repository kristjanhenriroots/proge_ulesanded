int otsing(int n, int M[n], int x){
    int index, max, min;
    max = n - 1;
    min = 0;
    index = n / 2 - 1;
    while(max - min >= 0) {
        if(M[index] == x) {
        printf("%d on massiivis olemas\n", x);
        return index;
        }
        else if(M[index] < x) {
            min = index + 1;
        }
        else if(M[index] > x) {
            max = index - 1;
        }
        index = (min + max) / 2;
    }

    return -1;
}