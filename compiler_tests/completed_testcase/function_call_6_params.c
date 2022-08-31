int g(int x, int y, int z, int m, int n, int r){
    int a = 0;
    a = x+y+z+m+n+r;
    return a;
}
int f(){
    int x = 1;
    int y = 2;
    int z = 3;
    int m = 4;
    int n = 3;
    int r = 0;
    r = g(x,y,z,m,n,r);
    return r;
}

