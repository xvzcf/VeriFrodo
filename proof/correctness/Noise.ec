(* SPDX-FileCopyrightText: 2021 The VeriFrodo Authors *)
(* SPDX-License-Identifier: MIT *)

require import AllCore IntDiv.
require import Distr.
from Jasmin require import JWord.
require import Array13 Array5120.
require import Kem.

require import Params.

lemma cdf_table_data_bounds : forall (k : int), 0 <= k <= 12 => 4643 <= W16.to_uint(CDF_TABLE_DATA.[k]) < 2^15.
proof.
    move => k H1.
    case (k = 0). move => H2. rewrite H2. trivial.
    case (k = 1). move => H2. rewrite H2. trivial.
    case (k = 2). move => H2. rewrite H2. trivial.
    case (k = 3). move => H2. rewrite H2. trivial.
    case (k = 4). move => H2. rewrite H2. trivial.
    case (k = 5). move => H2. rewrite H2. trivial.
    case (k = 6). move => H2. rewrite H2. trivial.
    case (k = 7). move => H2. rewrite H2. trivial.
    case (k = 8). move => H2. rewrite H2. trivial.
    case (k = 9). move => H2. rewrite H2. trivial.
    case (k = 10). move => H2. rewrite H2. trivial.
    case (k = 11). move => H2. rewrite H2. trivial.
    case (k = 12). move => H2. rewrite H2. trivial.
    smt.
qed.

lemma cdf_table_shift : forall (k : W16.t), 0 <= W16.to_uint(k) < 2^15 => k `>>` W8.of_int(15) = W16.of_int(0).
proof.
    move => k H1.
    rewrite to_uint_eq. simplify.
    rewrite shr_div. simplify.
    rewrite divz_small. apply H1. reflexivity.
qed.

lemma w16_positive (w : W16.t) : 0 <= W16.to_uint(w) => w `>>` W8.of_int(15) = W16.zero.
proof.
admitted.

lemma w16_gt_msb : forall (lhs rhs: W16.t), W16.to_uint(lhs) <=  W16.to_uint(rhs) => (rhs - lhs) `>>` W8.of_int(15) = W16.zero.
proof.
admitted.

lemma andw_with_1 (w : W16.t) : w `&` W16.one = W16.zero \/ w `&` W16.one = W16.one.
    have ->: W16.one = W16.of_int(2^1 - 1) by trivial. rewrite and_mod. trivial.
    smt.
qed.

lemma sample_N_by_NBAR_outputs_zero : hoare[ Kem.M.sample_N_by_NBAR : 0 <= W16.to_uint(s.[0]) < 4643 ==> W16.to_uint(res.[0]) = 0].
proof.
    proc. sp.
    while (cdf_table_ptr = CDF_TABLE_DATA /\ 0 <= W16.to_uint(s.[0]) < 4643 /\ (0 < W64.to_uint(i) => s.[0] = W16.zero) /\ 0 <= W64.to_uint(i)).
    sp. wp.
    while (0 <= W64.to_uint(j) /\ cdf_table_ptr = CDF_TABLE_DATA /\ i = W64.of_int(0) => (0 <= W16.to_uint(t1) < 4643 /\ sample = W16.of_int(0))).
    wp. skip. move => &m H1 H2.
    split. smt.
    have ->: sample{m} = W16.zero. smt. simplify. apply w16_gt_msb. smt.
    skip.
    move => &m [H1 [H2 [H3]]] H4. split. move => H5. smt.
    move => j sample H6 H7. split. smt. split. rewrite get_setE. smt.
    case (i{m} = W64.zero). move => H9. rewrite H9.
    simplify. have ->: sample = W16.zero. smt. simplify.
    split. smt. move => H10. case(s{m}.[0] `&` W16.one = W16.zero). smt. smt.
    smt. split. move => H8. case (i{m} = W64.zero). move => H9. rewrite H9. simplify.
    have ->: sample = W16.zero by smt. simplify. case (s{m}.[0] `&` W16.one = W16.zero). move => H10. rewrite H10.
    simplify. trivial. move => H10. have ->: s{m}.[0] `&` W16.one = W16.one by smt. simplify. trivial.
    move => H9. smt.
    smt.
    skip. move => &m [H1 [H2 [H3]]] H4. split. smt.
    move => i' s' H11. rewrite H1. simplify. smt.
qed.


lemma sample_N_by_NBAR_is_correct:
    phoare[ Kem.M.sample_N_by_NBAR : 
            mu1 Params.dunif (W16.to_uint s.[0]) = 1%r / 65536%r
            ==> (W16.to_uint res.[0]) = 0 ] = (9288%r / 65536%r).
proof.
admitted.
