{ pkgs ? import <nixpkgs> { }, use_revision ? builtins.getEnv "NIX_SOUL_TALES_REVISION"
, spdlog, zeromq, flatbuffers, flatcc, fmt, allegro5, fil, nlohmann_json, tmxlite, cppzmq}: rec {
  version = "1.0.0-DEV";

  pname = "soul_tale";

  src = if (builtins.isNull use_revision || use_revision == "") then
    pkgs.nix-gitignore.gitignoreSource [ ".git" ] ./../..
  else
    builtins.fetchGit {
      url = "https://github.com/FreeYourSoul/Soul_Tale.git";
      rev = use_revision;
    };

    buildInputs = [
      zeromq
      allegro5
      flatcc
      cppzmq
      flatbuffers
      spdlog
      fmt
      nlohmann_json
      tmxlite
    ];

  qb = {
    email = "ballandFyS@protonmail.com";
    github = "FreeYourSoul";
    githubId = 11722712;
    name = "Quentin Balland";
  };
  meta = with pkgs.stdenv.lib; {
    maintainers = [ qb ];
    homepage = "http://freeyoursoul.online";
    description = "Allegro Client for FyS Online (Soul Tales MMORPG)";
    licences = licenses.mit;
    platforms = platforms.linux;
  };
}