/* Solve a 2nd order polynomial */
#include <stdio.h>
#include <math.h>
/* function declarations */
float discriminant(float, float, float);
int solve(float a, float b, float c);
/* function definitions */
int main(void) {
	float a, b, c;
	printf("Please type 3 numbers\n");
	scanf("%f %f %f",&a,&b,&c);
	printf("Quadratic equation: %.2fx^2 + (%.2f)x + (%.2f) = 0\n", a, b, c);
	if (a == 0) {
		printf("This is a 1st degree equation\n");
 	} else {
	solve(a, b, c);
	}
}

float discriminant(float a, float b, float c) {
	return (b * b) -(4 * a * c);
}
int solve(float a, float b, float c) {
	float D, x1, x2;
	D = discriminant(a, b, c);
	if(D < 0) {
		printf("Discriminant is negative." "There are no real solutions\n");
		return 0;
	} else if(D == 0) {
		x1 = -b / (2 * a);
		printf("One solution: %f\n", x1);
		return 1;
	} else {
		x1 = (-b + sqrt(D)) / (2 * a);
		x2 = (-b - sqrt(D)) / (2 * a);
		printf("Two solutions: %f, %f\n", x1, x2);
		return 2;
	}
}

