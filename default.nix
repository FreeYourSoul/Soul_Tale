let pkgs = import <nixpkgs> { };
in with pkgs; rec {

  stdenv = pkgs.overrideCC pkgs.stdenv pkgs.gcc10;

  # External Dependencies (personal)
  fil = (callPackage (builtins.fetchurl
      "https://raw.githubusercontent.com/FreeYourSoul/FiL/d0a29fb955f513799696e29154a8b9c09c7e718e/fil-recipee.nix") {
      rev = "9a38ffe32a7cd709efe56bf7f05255259acb95a0";
    });

  # External Dependencies (not default nixpkg)
  nlohmann_json = (callPackage ./nix/dependency/nlohmann_json.nix) { };
  tmxlite = (callPackage ./nix/dependency/tmxlite.nix) { };
  cppzmq = (callPackage ./nix/dependency/cppzmq.nix) { };
  widgetz = (callPackage ./nix/dependency/widgetz.nix) { };

  # Soul_Tale Dependencies
  soul_tale = (callPackage ./nix/recipes/soul_tales.nix) {
    inherit  fil nlohmann_json tmxlite cppzmq widgetz;
  };
}
