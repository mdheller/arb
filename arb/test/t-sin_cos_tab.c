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

    Copyright (C) 2014 Fredrik Johansson

******************************************************************************/

#include "arb.h"

int main()
{
    slong i, which;

    mpfr_t tabx, sinx, y1, y2;
    mpz_t tt;

    printf("sin_cos_tab....");
    fflush(stdout);

    {
        slong prec, bits, num;

        prec = ARB_SIN_COS_TAB1_LIMBS * FLINT_BITS;
        bits = ARB_SIN_COS_TAB1_BITS;
        num = ARB_SIN_COS_TAB1_NUM;

        mpfr_init2(tabx, prec);
        mpfr_init2(sinx, prec);
        mpfr_init2(y1, prec);
        mpfr_init2(y2, prec);

        for (which = 0; which < 2; which++)
        {
            for (i = 0; i < num; i++)
            {
                tt->_mp_d = (mp_ptr) arb_sin_cos_tab1[2 * i + which];
                tt->_mp_size = prec / FLINT_BITS;
                tt->_mp_alloc = tt->_mp_size;

                while (tt->_mp_size > 0 && tt->_mp_d[tt->_mp_size-1] == 0)
                    tt->_mp_size--;

                mpfr_set_z(tabx, tt, MPFR_RNDD);
                mpfr_div_2ui(tabx, tabx, prec, MPFR_RNDD);

                mpfr_set_ui(sinx, i, MPFR_RNDD);
                mpfr_div_2ui(sinx, sinx, bits, MPFR_RNDD);
                if (which)
                    mpfr_cos(sinx, sinx, MPFR_RNDD);
                else
                    mpfr_sin(sinx, sinx, MPFR_RNDD);

                mpfr_mul_2ui(y1, tabx, prec, MPFR_RNDD);
                mpfr_floor(y1, y1);
                mpfr_div_2ui(y1, y1, prec, MPFR_RNDD);

                mpfr_mul_2ui(y2, sinx, prec, MPFR_RNDD);
                mpfr_floor(y2, y2);
                mpfr_div_2ui(y2, y2, prec, MPFR_RNDD);

                if (which && i == 0) /* special case */
                {
                    mpfr_set_ui(y2, 1, MPFR_RNDD);
                    mpfr_mul_2ui(y2, y2, prec, MPFR_RNDD);
                    mpfr_sub_ui(y2, y2, 1, MPFR_RNDD);
                    mpfr_div_2ui(y2, y2, prec, MPFR_RNDD);
                }

                if (!mpfr_equal_p(y1, y2))
                {
                    printf("FAIL: which = %wd, i = %wd, bits = %wd, prec = %wd\n", which, i, bits, prec);
                    mpfr_printf("y1 = %.1500Rg\n", y1);
                    mpfr_printf("y2 = %.1500Rg\n", y2);
                    abort();
                }
            }
        }

        mpfr_clear(tabx);
        mpfr_clear(sinx);
        mpfr_clear(y1);
        mpfr_clear(y2);
    }

    {
        slong prec, bits, num;

        prec = ARB_SIN_COS_TAB2_LIMBS * FLINT_BITS;
        bits = ARB_SIN_COS_TAB21_BITS;
        num = ARB_SIN_COS_TAB21_NUM;

        mpfr_init2(tabx, prec);
        mpfr_init2(sinx, prec);
        mpfr_init2(y1, prec);
        mpfr_init2(y2, prec);

        for (which = 0; which < 2; which++)
        {
            for (i = 0; i < num; i++)
            {
                tt->_mp_d = (mp_ptr) arb_sin_cos_tab21[2 * i + which];
                tt->_mp_size = prec / FLINT_BITS;
                tt->_mp_alloc = tt->_mp_size;

                while (tt->_mp_size > 0 && tt->_mp_d[tt->_mp_size-1] == 0)
                    tt->_mp_size--;

                mpfr_set_z(tabx, tt, MPFR_RNDD);
                mpfr_div_2ui(tabx, tabx, prec, MPFR_RNDD);

                mpfr_set_ui(sinx, i, MPFR_RNDD);
                mpfr_div_2ui(sinx, sinx, bits, MPFR_RNDD);
                if (which)
                    mpfr_cos(sinx, sinx, MPFR_RNDD);
                else
                    mpfr_sin(sinx, sinx, MPFR_RNDD);

                mpfr_mul_2ui(y1, tabx, prec, MPFR_RNDD);
                mpfr_floor(y1, y1);
                mpfr_div_2ui(y1, y1, prec, MPFR_RNDD);

                mpfr_mul_2ui(y2, sinx, prec, MPFR_RNDD);
                mpfr_floor(y2, y2);
                mpfr_div_2ui(y2, y2, prec, MPFR_RNDD);

                if (which && i == 0) /* special case */
                {
                    mpfr_set_ui(y2, 1, MPFR_RNDD);
                    mpfr_mul_2ui(y2, y2, prec, MPFR_RNDD);
                    mpfr_sub_ui(y2, y2, 1, MPFR_RNDD);
                    mpfr_div_2ui(y2, y2, prec, MPFR_RNDD);
                }

                if (!mpfr_equal_p(y1, y2))
                {
                    printf("FAIL: i = %wd, bits = %wd, prec = %wd\n", i, bits, prec);
                    mpfr_printf("y1 = %.1500Rg\n", y1);
                    mpfr_printf("y2 = %.1500Rg\n", y2);
                    abort();
                }
            }
        }

        mpfr_clear(tabx);
        mpfr_clear(sinx);
        mpfr_clear(y1);
        mpfr_clear(y2);
    }

    {
        slong prec, bits, num;

        prec = ARB_SIN_COS_TAB2_LIMBS * FLINT_BITS;
        bits = ARB_SIN_COS_TAB21_BITS + ARB_SIN_COS_TAB22_BITS;
        num = ARB_SIN_COS_TAB22_NUM;

        mpfr_init2(tabx, prec);
        mpfr_init2(sinx, prec);
        mpfr_init2(y1, prec);
        mpfr_init2(y2, prec);

        for (which = 0; which < 2; which++)
        {
            for (i = 0; i < num; i++)
            {
                tt->_mp_d = (mp_ptr) arb_sin_cos_tab22[2 * i + which];
                tt->_mp_size = prec / FLINT_BITS;
                tt->_mp_alloc = tt->_mp_size;

                while (tt->_mp_size > 0 && tt->_mp_d[tt->_mp_size-1] == 0)
                    tt->_mp_size--;

                mpfr_set_z(tabx, tt, MPFR_RNDD);
                mpfr_div_2ui(tabx, tabx, prec, MPFR_RNDD);

                mpfr_set_ui(sinx, i, MPFR_RNDD);
                mpfr_div_2ui(sinx, sinx, bits, MPFR_RNDD);
                if (which)
                    mpfr_cos(sinx, sinx, MPFR_RNDD);
                else
                    mpfr_sin(sinx, sinx, MPFR_RNDD);

                mpfr_mul_2ui(y1, tabx, prec, MPFR_RNDD);
                mpfr_floor(y1, y1);
                mpfr_div_2ui(y1, y1, prec, MPFR_RNDD);

                mpfr_mul_2ui(y2, sinx, prec, MPFR_RNDD);
                mpfr_floor(y2, y2);
                mpfr_div_2ui(y2, y2, prec, MPFR_RNDD);

                if (which && i == 0) /* special case */
                {
                    mpfr_set_ui(y2, 1, MPFR_RNDD);
                    mpfr_mul_2ui(y2, y2, prec, MPFR_RNDD);
                    mpfr_sub_ui(y2, y2, 1, MPFR_RNDD);
                    mpfr_div_2ui(y2, y2, prec, MPFR_RNDD);
                }

                if (!mpfr_equal_p(y1, y2))
                {
                    printf("FAIL: i = %wd, bits = %wd, prec = %wd\n", i, bits, prec);
                    mpfr_printf("y1 = %.1500Rg\n", y1);
                    mpfr_printf("y2 = %.1500Rg\n", y2);
                    abort();
                }
            }
        }

        mpfr_clear(tabx);
        mpfr_clear(sinx);
        mpfr_clear(y1);
        mpfr_clear(y2);
    }

    flint_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}

