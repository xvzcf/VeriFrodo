# Installing

For MacOS:

1. Install easycrypt:

* Install ocaml and opam:

```
brew install ocaml opam
opam init
eval $(opam env)
```

(note that on some versions you don't need to execute `eval $(opam env)` as it
will add it for you in your bash).

* Pin easycrypt: `opam pin -yn add easycrypt https://github.com/EasyCrypt/easycrypt.git`
* Install dependencies:

```
opam install --deps-only easycrypt
opam install alt-ergo
```

* Configure why3: `why3 config detect`
* Install easycrypt: `opam install easycrypt`

Note that on many occasions, you will need to add Z3, which you can do as:

```
brew install z3
opam install alt-ergo.2.3.1 (in case it cannot find the prover)
```

2. Install jasmin:

* Clone the repository:

```
git clone git@github.com:jasmin-lang/jasmin.git
cd jasmin
```

* Install nix: `curl -L https://nixos.org/nix/install | sh`, and follow
  the specific instructions.

* Type:

```
cd compiler
make CIL
make
make check
exit
```
