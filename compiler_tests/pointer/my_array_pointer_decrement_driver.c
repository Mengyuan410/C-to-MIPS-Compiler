int f();

int main()
{
    int x[3];
    x[1]=13;
    return !(f(x+2)==13);
}