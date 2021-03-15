(* SPDX-FileCopyrightText: 2021 The VeriFrodo Authors *)
(* SPDX-License-Identifier: MIT *)

require import AllCore.
from Jasmin require import JModel.
require import Array64.
require import Util.

lemma add_is_correct _a _b:
      hoare[ Util.M.add :
           a = _a /\
           b = _b
           ==>
           forall k, 0 <= k < 64 =>
              res.[k] = (_a.[k] + _b.[k]) `&` W16.of_int(32767) ].
proof.
    proc. simplify. sp.
    while (aux = 64 /\ 0 <= i  /\ (forall (k: int), 0 <= k < i => out.[k] = (a.[k] + b.[k]) `&` W16.of_int(32767))).
    + wp. skip => />. smt.
    + skip => />. move => *. split. smt. smt.
qed.
