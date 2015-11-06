/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "arb_poly.h"

int main()
{
    slong iter;
    flint_rand_t state;

    printf("mullow_block....");
    fflush(stdout);

    flint_randinit(state);

    /* compare with fmpq_poly */
    for (iter = 0; iter < 10000; iter++)
    {
        slong qbits1, qbits2, rbits1, rbits2, rbits3, trunc;
        fmpq_poly_t A, B, C;
        arb_poly_t a, b, c, d;

        qbits1 = 2 + n_randint(state, 1000);
        qbits2 = 2 + n_randint(state, 1000);
        rbits1 = 2 + n_randint(state, 1000);
        rbits2 = 2 + n_randint(state, 1000);
        rbits3 = 2 + n_randint(state, 1000);
        trunc = n_randint(state, 100);

        fmpq_poly_init(A);
        fmpq_poly_init(B);
        fmpq_poly_init(C);

        arb_poly_init(a);
        arb_poly_init(b);
        arb_poly_init(c);
        arb_poly_init(d);

        fmpq_poly_randtest(A, state, 1 + n_randint(state, 100), qbits1);
        fmpq_poly_randtest(B, state, 1 + n_randint(state, 100), qbits2);
        fmpq_poly_mullow(C, A, B, trunc);

        arb_poly_set_fmpq_poly(a, A, rbits1);
        arb_poly_set_fmpq_poly(b, B, rbits2);
        arb_poly_mullow_block(c, a, b, trunc, rbits3);

        if (!arb_poly_contains_fmpq_poly(c, C))
        {
            printf("FAIL\n\n");
            printf("bits3 = %wd\n", rbits3);
            printf("trunc = %wd\n", trunc);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("B = "); fmpq_poly_print(B); printf("\n\n");
            printf("C = "); fmpq_poly_print(C); printf("\n\n");

            printf("a = "); arb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); arb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); arb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        arb_poly_set(d, a);
        arb_poly_mullow_block(d, d, b, trunc, rbits3);
        if (!arb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 1)\n\n");
            abort();
        }

        arb_poly_set(d, b);
        arb_poly_mullow_block(d, a, d, trunc, rbits3);
        if (!arb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 2)\n\n");
            abort();
        }

        /* test squaring */
        arb_poly_set(b, a);
        arb_poly_mullow_block(c, a, b, trunc, rbits3);
        arb_poly_mullow_block(d, a, a, trunc, rbits3);
        if (!arb_poly_overlaps(c, d))  /* not guaranteed to be identical */
        {
            printf("FAIL (squaring)\n\n");

            printf("a = "); arb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); arb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); arb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        arb_poly_mullow_block(a, a, a, trunc, rbits3);
        if (!arb_poly_equal(d, a))
        {
            printf("FAIL (aliasing, squaring)\n\n");

            printf("a = "); arb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); arb_poly_printd(b, 15); printf("\n\n");
            printf("d = "); arb_poly_printd(d, 15); printf("\n\n");

            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);
        fmpq_poly_clear(C);

        arb_poly_clear(a);
        arb_poly_clear(b);
        arb_poly_clear(c);
        arb_poly_clear(d);
    }

    for (iter = 0; iter < 3000; iter++)
    {
        slong rbits1, rbits2, rbits3, trunc;
        arb_poly_t a, b, c, ab, ac, bc, abc, abc2;

        rbits1 = 2 + n_randint(state, 300);
        rbits2 = 2 + n_randint(state, 300);
        rbits3 = 2 + n_randint(state, 300);
        trunc = n_randint(state, 100);

        arb_poly_init(a);
        arb_poly_init(b);
        arb_poly_init(c);
        arb_poly_init(ab);
        arb_poly_init(ac);
        arb_poly_init(bc);
        arb_poly_init(abc);
        arb_poly_init(abc2);

        arb_poly_randtest(a, state, 1 + n_randint(state, 100), rbits1, 1 + n_randint(state, 100));
        arb_poly_randtest(b, state, 1 + n_randint(state, 100), rbits2, 1 + n_randint(state, 100));
        arb_poly_randtest(c, state, 1 + n_randint(state, 100), rbits2, 1 + n_randint(state, 100));

        /* check a*(b+c) = a*b + a*c */
        arb_poly_mullow_block(ab, a, b, trunc, rbits3);
        arb_poly_mullow_block(ac, a, c, trunc, rbits3);
        arb_poly_add(abc, ab, ac, rbits3);

        arb_poly_add(bc, b, c, rbits3);
        arb_poly_mullow_block(abc2, a, bc, trunc, rbits3);

        if (!arb_poly_overlaps(abc, abc2))
        {
            printf("FAIL (a*(b+c) = a*b + a*c) \n\n");
            printf("bits3 = %wd\n", rbits3);
            printf("trunc = %wd\n", trunc);

            printf("a = "); arb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); arb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); arb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        /* check (b+c)^2 = b^2 + 2bc + c^2 */
        arb_poly_mullow_block(a, b, c, trunc, rbits3);
        arb_poly_scalar_mul_2exp_si(a, a, 1);
        arb_poly_mullow_block(abc, b, b, trunc, rbits3);
        arb_poly_mullow_block(abc2, c, c, trunc, rbits3);
        arb_poly_add(abc, abc, a, rbits3);
        arb_poly_add(abc, abc, abc2, rbits3);

        arb_poly_mullow_block(abc2, bc, bc, trunc, rbits3);

        if (!arb_poly_overlaps(abc, abc2))
        {
            printf("FAIL ((b+c)^2 = b^2 + 2bc + c^2) \n\n");
            printf("bits3 = %wd\n", rbits3);
            printf("trunc = %wd\n", trunc);

            printf("b = "); arb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); arb_poly_printd(c, 15); printf("\n\n");

            printf("abc  = "); arb_poly_printd(abc, 15); printf("\n\n");
            printf("abc2 = "); arb_poly_printd(abc2, 15); printf("\n\n");

            abort();
        }

        arb_poly_clear(a);
        arb_poly_clear(b);
        arb_poly_clear(c);
        arb_poly_clear(ab);
        arb_poly_clear(ac);
        arb_poly_clear(bc);
        arb_poly_clear(abc);
        arb_poly_clear(abc2);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

