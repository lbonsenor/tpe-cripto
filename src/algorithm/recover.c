#include "algorithm.h"

/*
TODO Step 0. Recover seed/indexes
TODO Step 1. Set the current processing section j to 1. 
TODO Step 2. Take a non-processed pixel of position j from each of the r shadow images. 
TODO Step 3. Use these r pixels fj(1), fj(2), fj(3),..., fj(r) and Lagrange’s interpolation to solve the coefficients a0, a1, a2 ,..., ar-1 in Eq. 5. They are exactly the r pixel values of the j-th section in Q. 
TODO Step 4. Increase j by 1. 
TODO Step 5. Repeat Steps 2 to 4 until all pixels of the shadow images are processed. 
TODO Step 6. Apply the XOR operation to the predefined random image R and Q to get the secret image O’.
*/

int
recover() {
}