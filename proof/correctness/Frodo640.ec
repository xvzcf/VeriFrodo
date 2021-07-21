(* SPDX-FileCopyrightText: 2021 The VeriFrodo Authors *)
(* SPDX-License-Identifier: MIT *)

require import AllCore Distr Int.
from Jasmin require import JWord.
require import Array5120.

theory Frodo640.

op ddiscr_gauss: int distr.
axiom supp_ddiscr_gauss (x : int) : -12 <= x <= 12 <=> x \in ddiscr_gauss.
axiom ddiscr_gauss1E_not_in_support x : (x \notin ddiscr_gauss) => (mu1 ddiscr_gauss x = 0%r).
axiom ddiscr_gauss1E_0 : mu1 ddiscr_gauss 0 = 9288%r / 65536%r.
axiom ddiscr_gauss1E_1 : mu1 ddiscr_gauss 1 = 8720%r / 65536%r.
axiom ddiscr_gauss1E_2 : mu1 ddiscr_gauss 2 = 7216%r / 65536%r.
axiom ddiscr_gauss1E_3 : mu1 ddiscr_gauss 3 = 5264%r / 65536%r.
axiom ddiscr_gauss1E_4 : mu1 ddiscr_gauss 4 = 3384%r / 65536%r.
axiom ddiscr_gauss1E_5 : mu1 ddiscr_gauss 5 = 1918%r / 65536%r.
axiom ddiscr_gauss1E_6 : mu1 ddiscr_gauss 6 = 958%r / 65536%r.
axiom ddiscr_gauss1E_7 : mu1 ddiscr_gauss 7 = 422%r / 65536%r.
axiom ddiscr_gauss1E_8 : mu1 ddiscr_gauss 8 = 164%r / 65536%r.
axiom ddiscr_gauss1E_9 : mu1 ddiscr_gauss 9 = 56%r / 65536%r.
axiom ddiscr_gauss1E_10 : mu1 ddiscr_gauss 10 = 17%r / 65536%r.
axiom ddiscr_gauss1E_11 : mu1 ddiscr_gauss 11 = 4%r / 65536%r.
axiom ddiscr_gauss1E_12 : mu1 ddiscr_gauss 12 = 1%r / 65536%r.
axiom ddiscr_gauss1E_neg x : x < 0 => mu1 ddiscr_gauss (-x) = mu1 ddiscr_gauss (x).
axiom  ddiscr_gauss_ll : is_lossless ddiscr_gauss.

op dunif : int distr.
axiom supp_dunif x : 0 <= x <= 65536 <=> x \in dunif.
axiom dunif1E x : mu1 dunif x = 1%r / 65536%r.
axiom dunif_ll : is_lossless dunif.

module M = {
  proc discr_gauss_sample() : int = {
  var sample : int;
  sample <$ ddiscr_gauss;
  return sample;
  }

      proc uniform_sample () : int = {
      var sample : int;
      sample <$ dunif;
      return sample;
  }
}.


end Frodo640.
