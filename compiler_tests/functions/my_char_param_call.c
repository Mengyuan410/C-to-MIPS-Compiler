int g(char x, char y, char z){
    x = 'r';
    y = 'e';
    z = y;
    return z;
}


int f() {
    char a = 'w';
    char b = 's';
    return g(a,'e',b);
}