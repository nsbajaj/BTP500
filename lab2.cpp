unsigned int factorial (unsigned int n){
	if(n <= 1){
		return 1;
	}
	else{
		return n*factorial(n-1);
	}
}

double power (double base, unsigned int n){
	if(n < 1){
		return 1;
	}
	else{
		return base*power(base, n-1);
	}
}
unsigned int fibonacci (unsigned int n){
	if(n <= 1){
		return n;
	}
	else{
		return fibonacci(n-1) + fibonacci(n-2);
	}		
}
