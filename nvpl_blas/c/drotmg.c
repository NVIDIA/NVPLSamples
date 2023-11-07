/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_drotmg
 *
 ******************************************************************************/
#include "example_helper.h"
#include <string.h> // memset

int main() {
    double d1 = 2.3f;
    double d2 = 4.6f;
    double b1 = 1.0f;
    double b2 = 2.0f;
    double P[5];

    printf("\nExample: cblas_drotmg for computing the parameters for a Givens rotation\n");
    printf("\n#### args: d1=%f, d2=%f, b1=%f, b2=%f\n", d1, d2, b1, b2);

    memset(P, 0, 5 * sizeof(double));

    // call cblas_drotmg
    cblas_drotmg(&d1, &d2, &b1, b2, P);

    // print result
    printf("\nThe result of cblas_drotmg:\n");
    printf("d1=%f, d2=%f, b1=%f\n", d1, d2, b1);
    print_dvector(P, 5, 1, "P");

    return 0;
}
