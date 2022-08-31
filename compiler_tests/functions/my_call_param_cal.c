int g (int c, int d, int e){
    return c*d+e;
}

int f()
{
    int a = 8;
    int b = 3;
    int c =12;
    return g(a+b,a-b, c+a*b);
}