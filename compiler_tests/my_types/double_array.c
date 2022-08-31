double f(){

    double x[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double *p = &x[0];
    p=p+1;

    return *p;
}