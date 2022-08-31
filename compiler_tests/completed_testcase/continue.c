int f(){
    int x = 0;
    int y = 0;
    int i = 0;
    int j = 0;
    for (j = 0; j < 10; j++) {
        for (i = 0; i < 10; i++) {
            if (i == 4) {
                continue;
            }
            x += 1;
        }
        y+=1;
    }
    return x;
}