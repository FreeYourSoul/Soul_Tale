# Allegro TMX

This plugin for allegro aims to display TMX formatted map (produced by the map editor [Tiled](https://www.mapeditor.org/) which is well maintained and improved over time).

#### Again another tmx displayed...

Many project did aim to print tmx map (as shown in [Tiled documentation](https://doc.mapeditor.org/en/stable/reference/support-for-tmx-maps/)). The issue is that most of those project stopped being under development and maintained.. but Tiled didn't and continued to have more and more feature and improve its TMX format. Some of the solution listed in the documentation unfortunately force you to use a old version of TMX.

#### So why this one would be any different ? 

This project doesn't provide any guarantee that it will be maintained to the most up to date version of tmx all the time, like any non-founded open source project. But it appears that the main issue of the other tmx render library out there are actually trying to implement their own TMX parser (certainly because it's actually fun to do, I think so too :p).  
 But it has the issue that, if not maintained, it becomes obsolete after improvement of TMX format.  
 
allegro_tmx is based on [tmxlite](https://github.com/fallahn/tmxlite) which is developed by the developer and maintainer of Tiled, Matt Styles aka [fallahn](https://github.com/fallahn). This is the parser that is used by Tiled itself in order to parse TMX maps, and thus we can ensure by upgrading this parsing library to always be up to date with the latest version of TMX format. 

The way allegro_tmx display the map is based on the SFML example given by fallahn in tmxlite. Which means it most likely is the way to go to display this kind of map.

## Dependency

[tmxlite](https://github.com/fallahn/tmxlite) : Official TMX parser  
[allegro5](https://github.com/liballeg/allegro5) : Well, hard to be a plugin for allegro without dependency on it.

## Nix support

A nix recipe (derivation) is provided. it can be easily used inside your own project by adding the following line to your nix package (and then used in your derivation `buildInput`) and replacing <latest_commit-hash> by the proper sha:

```nix
let pkgs = import <nixpkgs> { };
in with pkgs; rec {
  #...
  allegro_tmx = (callPackage (builtins.fetchurl
    "https://raw.githubusercontent.com/FreeYourSoul/allegro_tiled/<latest_commit-hash>/allegro_tmx-recipe.nix") {
      rev = "<latest_commit-hash>";
    });
  #...
}
``` 

This may not be the cleaner way to use Nix (it's the way I use for my private packages to not copy the same nix derivation code using fetchFromGithub everywhere). You can just copy the allegro_tmx-recipe.nix file into your project and callPackage it as follow:

```nix
let pkgs = import <nixpkgs> { };
in with pkgs; rec {
  #...
  allegro_tmx = (callPackage allegro_tmx-recipe.nix) { };
  #...
}
``` 