/*
    Copyright (C) 2018 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_mat.h"

void
acb_mat_solve_triu_classical(acb_mat_t X, const acb_mat_t U,
    const acb_mat_t B, int unit, slong prec)
{
    slong i, j, k, n, m;
    acb_ptr tmp;
    acb_t s;

    n = U->r;
    m = B->c;

    tmp = _acb_vec_init(n);
    acb_init(s);

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            acb_set(tmp + j, acb_mat_entry(X, j, i));

        for (j = n - 1; j >= 0; j--)
        {
            acb_zero(s);
            for (k = 0; k < n - j - 1; k++)
                acb_addmul(s, U->rows[j] + j + 1 + k, tmp + j + 1 + k, prec);

            acb_sub(s, acb_mat_entry(B, j, i), s, prec);
            if (!unit)
                acb_div(s, s, acb_mat_entry(U, j, j), prec);

            acb_set(tmp + j, s);
        }

        for (j = 0; j < n; j++)
            acb_set(acb_mat_entry(X, j, i), tmp + j);
    }

    _acb_vec_clear(tmp, n);
    acb_clear(s);
}

void
acb_mat_solve_triu_recursive(acb_mat_t X,
        const acb_mat_t U, const acb_mat_t B, int unit, slong prec)
{
    acb_mat_t UA, UB, UD, XX, XY, BX, BY, T;
    slong r, n, m;

    n = U->r;
    m = B->c;
    r = n / 2;

    if (n == 0 || m == 0)
        return;

    /*
    Denoting inv(M) by M^, we have:
    [A B]^ [X]  ==  [A^ (X - B D^ Y)]
    [0 D]  [Y]  ==  [    D^ Y       ]
    */

    acb_mat_window_init(UA, U, 0, 0, r, r);
    acb_mat_window_init(UB, U, 0, r, r, n);
    acb_mat_window_init(UD, U, r, r, n, n);
    acb_mat_window_init(BX, B, 0, 0, r, m);
    acb_mat_window_init(BY, B, r, 0, n, m);
    acb_mat_window_init(XX, X, 0, 0, r, m);
    acb_mat_window_init(XY, X, r, 0, n, m);

    acb_mat_solve_triu(XY, UD, BY, unit, prec);

    /* acb_mat_submul(XX, BX, UB, XY); */
    acb_mat_init(T, UB->r, XY->c);
    acb_mat_mul(T, UB, XY, prec);
    acb_mat_sub(XX, BX, T, prec);
    acb_mat_clear(T);

    acb_mat_solve_triu(XX, UA, XX, unit, prec);

    acb_mat_window_clear(UA);
    acb_mat_window_clear(UB);
    acb_mat_window_clear(UD);
    acb_mat_window_clear(BX);
    acb_mat_window_clear(BY);
    acb_mat_window_clear(XX);
    acb_mat_window_clear(XY);
}

void
acb_mat_solve_triu(acb_mat_t X, const acb_mat_t U,
                                    const acb_mat_t B, int unit, slong prec)
{
    if (B->r < 8 || B->c < 8)
        acb_mat_solve_triu_classical(X, U, B, unit, prec);
    else
        acb_mat_solve_triu_recursive(X, U, B, unit, prec);
}

