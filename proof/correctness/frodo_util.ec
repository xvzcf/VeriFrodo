(* SPDX-FileCopyrightText: 2021 The VeriFrodo Authors *)
(* SPDX-License-Identifier: MIT *)

require import AllCore ZModP.
from Jasmin require import JWord.
require import Array64.
require import Kem.

clone import ZModRing as Frodo640Ring with op p <- 2^15.

lemma add_is_correct _a _b:
      hoare[ Kem.M.add :
           a = _a /\
           b = _b
           ==>
           forall k, 0 <= k < 64 =>
              Frodo640Ring.inzmod(W16.to_uint(res.[k])) = Frodo640Ring.inzmod(W16.to_uint(_a.[k] + _b.[k])) ].
proof.
    proc. simplify. sp.
    while (aux = 64 /\ 0 <= i  /\ (forall (k: int), 0 <= k < i => Frodo640Ring.inzmod(W16.to_uint(out.[k])) = Frodo640Ring.inzmod(W16.to_uint(a.[k] + b.[k]))) ).
  + wp. skip => />. move => &mem *. split.
    + smt.
    + move => k H1 H2.
      case (k = i{mem}). move => H3. apply Frodo640Ring.eq_inzmod. have ->: 32767 = 2^15 - 1 by trivial. rewrite  W16.and_mod. trivial. smt.
      smt.
  + skip => />. move => *. split. smt. smt.
qed.
