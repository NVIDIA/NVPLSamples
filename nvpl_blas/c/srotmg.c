/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_srotmg
 *
 ******************************************************************************/
#include "example_helper.h"
#include <string.h> // memset

int main() {
    float d1 = 2.3f;
    float d2 = 4.6f;
    float b1 = 1.0f;
    float b2 = 2.0f;
    float P[5];

    printf("\nExample: cblas_srotmg for computing the parameters for a Givens rotation\n");
    printf("\n#### args: d1=%f, d2=%f, b1=%f, b2=%f\n", d1, d2, b1, b2);

    memset(P, 0, 5 * sizeof(float));

    // call cblas_srotmg
    cblas_srotmg(&d1, &d2, &b1, b2, P);

    // print result
    printf("\nThe result of cblas_srotmg:\n");
    printf("d1=%f, d2=%f, b1=%f\n", d1, d2, b1);
    print_svector(P, 5, 1, "P");

    return 0;
}
