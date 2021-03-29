{ stdenv, callPackage, cmake }:

stdenv.mkDerivation rec {
  version = "1.2.1";
  pname = "tmxlite";

  src = builtins.fetchGit {
    url = "https://github.com/fallahn/tmxlite.git";
    rev = "849bcd9d6f8418f975a8a2257dc924c36fa91cfc";
  };
  sourceRoot = "source/tmxlite";

  buildInputs = [ ];
  nativeBuildInputs = [ cmake ];

  cmakeFlags = [ "-DBUILD_TESTING=OFF" ];

  enableParallelBuilding = true;
  doCheck = false;
}