int f(){
    int a;
    int b = 3;
    a = 127;
    a &= b*2+b;
    return a;
}