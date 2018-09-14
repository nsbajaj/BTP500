unsigned int factorial (unsigned int n){
    int temp = 1;
    if(n <= 1){
        return temp;
    }
    else{
        for(int i = n; i >= 1; i--){
            temp = temp * i;
        }
    }
    return temp;
}

double power (double base, unsigned int n){
    int temp = base;
    if(n < 1){
        temp = 1;
    }
    else{
        for(int i = n; i > 1; i--){
            temp = base * temp;
        }
    }
    return temp;
}
unsigned int fibonacci (unsigned int n){
    int temp = 0;
    int prev1 = 0;
    int prev2 = 1;
    for(int i = 0; i < n; i++){
        temp = prev1 + prev2;
        prev2 = prev1;
        prev1 = temp;
    }
    return temp;
}
