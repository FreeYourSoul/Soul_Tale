{ stdenv, lib, nix-gitignore
, use_revision ? builtins.getEnv "NIX_SOUL_TALES_REVISION", spdlog, pkgconfig
, widgetz, zeromq, pngpp, fmt, allegro5, fil, nlohmann_json, tmxlite, cppzmq
, cmake, catch2 }:

stdenv.mkDerivation rec {
  version = "0.0.1-DEV";

  pname = "soul_tale";

  src = if (builtins.isNull use_revision || use_revision == "") then
    nix-gitignore.gitignoreSource [ ".git" ] ./../..
  else
    builtins.fetchGit {
      url = "https://github.com/FreeYourSoul/Soul_Tale.git";
      rev = use_revision;
    };

  buildInputs = [
    cmake
    zeromq
    allegro5
    widgetz
    fil
    cppzmq
    spdlog
    pngpp
    fmt
    nlohmann_json
    catch2
    tmxlite
  ];

  nativeBuildInputs = [ cmake pkgconfig ];

  meta = with lib; {
    homepage = "http://freeyoursoul.online";
    description = "Allegro Client for FyS Online (Soul Tales MMORPG)";
    licences = licenses.mit;
    platforms = platforms.linux;
  };
}
