#include "lagrange.h"

static int
mod257(int value);

static int
inv_mod257(int value);

static void
copy_int_array(int dst[], const int src[], int len);

static int
lagrange_at_zero(const int y_values[], int count);

int
lagrange_interpolation(const uint8_t shadow_values[], int k, uint8_t out_coeffs[]) {
    // residual[i] stores R_d(x_i), with x_i = i+1 and R_0(x)=P(x).
    int residual[k];
    for (int i = 0; i < k; i++) {
        residual[i] = shadow_values[i];
    }

    for (int d = 0; d < k; d++) {
        // Version 2: a_d = R_d(0) = sum_i L_i(0) * R_d(x_i)
        int ad = lagrange_at_zero(residual, k - d);
        if (ad < 0 || ad > 255) {
            return -1;
        }
        out_coeffs[d] = (uint8_t)ad;

        if (d == k - 1) {
            break;
        }

        // Build next residual: R_{d+1}(x_i) = (R_d(x_i) - a_d) / x_i
        int next_residual[k];
        for (int i = 0; i < k - d; i++) {
            int xi = i + 1;
            int inv_xi = inv_mod257(xi);
            if (inv_xi < 0) {
                return -1;
            }
            next_residual[i] = mod257((residual[i] - ad) * inv_xi);
        }
        copy_int_array(residual, next_residual, k - d - 1);
    }

    return 0;
}

static void
copy_int_array(int dst[], const int src[], int len) {
    for (int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}

static int
lagrange_at_zero(const int y_values[], int count) {
    int acc = 0;

    for (int i = 0; i < count; i++) {
        int xi = i + 1;
        int yi = y_values[i];
        int num = 1;
        int den = 1;

        for (int j = 0; j < count; j++) {
            if (j == i) {
                continue;
            }

            int xj = j + 1;
            num = mod257(num * mod257(-xj));
            den = mod257(den * mod257(xi - xj));
        }

        int inv_den = inv_mod257(den);
        if (inv_den < 0) {
            return -1;
        }

        int li0 = mod257(num * inv_den);
        acc = mod257(acc + mod257(yi * li0));
    }

    return acc;
}

static int
mod257(int value) {
    int r = value % 257;
    if (r < 0) {
        r += 257;
    }
    return r;
}

static int
inv_mod257(int value) {
    int a = mod257(value);
    if (a == 0) {
        return -1;
    }

    int t = 0;
    int new_t = 1;
    int r = 257;
    int new_r = a;

    while (new_r != 0) {
        int q = r / new_r;

        int tmp_t = t - q * new_t;
        t = new_t;
        new_t = tmp_t;

        int tmp_r = r - q * new_r;
        r = new_r;
        new_r = tmp_r;
    }

    if (r != 1) {
        return -1;
    }

    return mod257(t);
}
