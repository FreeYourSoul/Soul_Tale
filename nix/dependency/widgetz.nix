{ stdenv, callPackage, cmake, allegro5, freetype }:

stdenv.mkDerivation rec {
    version = "4.8.0";
    pname = "widgetz";

    src = builtins.fetchGit {
    url = "https://github.com/SiegeLord/WidgetZ.git";
    rev = "5fe5fd67ef4c1a87a5eb6fefc1aa8d0c54a7d3e6";
    };

    buildInputs = [ cmake allegro5 freetype ];

    cmakeFlags = [ "-DCPPZMQ_BUILD_TESTS=OFF" ];

    enableParallelBuilding = true;
    doCheck = false;
}