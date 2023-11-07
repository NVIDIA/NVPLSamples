/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_drotg
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    double a = 2.3f;
    double b = 4.6f;
    double c = 0.0f;
    double s = 0.0f;

    printf("\nExample: cblas_drotg for computing the parameters for a Givens rotation\n");
    printf("\n#### args: a=%f, b=%f, c=%f, s=%f\n",a, b, c, s);

    // call cblas_drotg
    cblas_drotg(&a, &b, &s, &c);

    // print result
    printf("\nThe result of cblas_drotg:\n");
    printf("a=%f, b=%f, c=%f, s=%f\n",a, b, c, s);

    return 0;
}
