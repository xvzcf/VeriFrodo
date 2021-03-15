(* SPDX-FileCopyrightText: 2021 Alley Stoughton *)
(* SPDX-License-Identifier: TODO *)

(* if EasyCrypt is able to successfully check this file, then
   it and the SMT solvers Alt-Ergo and Z3 are properly installed
   and configured *)

require import AllCore.  (* load the "core" EasyCrypt theories, in
                            particular giving us the relation < on the
                            type int of integers *)

prover quorum=2 ["Alt-Ergo" "Z3"].  (* the smt tactic will only
                                       succeed if both Alt-Ergo and
                                       Z3 are able to solve the goal *)

lemma foo (x y : int) :
  x < y => x + 1 < y + 1.
proof.
smt().
qed.
