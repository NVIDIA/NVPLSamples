/******************************************************************************
 * Content: 
 *     This example demonstrates use of API as below:
 *     cblas_srotg
 *
 ******************************************************************************/
#include "example_helper.h"

int main() {
    float a = 2.3f;
    float b = 4.6f;
    float c = 0.0f;
    float s = 0.0f;

    printf("\nExample: cblas_srotg for computing the parameters for a Givens rotation\n");
    printf("\n#### args: a=%f, b=%f, c=%f, s=%f\n",a, b, c, s);

    // call cblas_srotg
    cblas_srotg(&a, &b, &s, &c);

    // print result
    printf("\nThe result of cblas_srotg:\n");
    printf("a=%f, b=%f, c=%f, s=%f\n",a, b, c, s);

    return 0;
}
