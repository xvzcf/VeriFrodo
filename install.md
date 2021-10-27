# Installing

For MacOS:

## Installing easycrypt:

1. Install ocaml and opam:

	```
	brew install ocaml opam
	opam init
	eval $(opam env)
	```

	(note that on some MacOS versions you don't need to execute `eval $(opam env)` as it
 it for you in your terminal).

2. Pin easycrypt: `opam pin -yn add easycrypt https://github.com/EasyCrypt/easycrypt.git`.
3. Install the needed dependencies:

	```
	opam install --deps-only easycrypt
	opam install alt-ergo
	```

4. Configure why3: `why3 config detect`.
5. Install easycrypt: `opam install easycrypt`.

	Note that on many occasions, you will need to add Z3, which you can do as:

	```
	brew install z3
	opam install alt-ergo.2.3.1 (in case it cannot find the prover)
	```
6. Check your installation:

	```
	Clone this repo.
	Run: ./ec.native check-easycrypt-install.ec
	```

## Installing jasmin

1. Clone the repository:

	```
	git clone git@github.com:jasmin-lang/jasmin.git
	cd jasmin
	```

2. Install nix: `curl -L https://nixos.org/nix/install | sh`, and follow the specific instructions.

3. Run:

	```
	cd compiler
	make CIL
	make
	make check
	exit
	```
