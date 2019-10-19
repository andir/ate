{ nixpkgs ? import <nixpkgs> { } }:
let mkTest = config: nixpkgs.callPackage ./default.nix { config.ate = config; };
in {
  default = mkTest {
    keybindings = {
      # INCREMENT_FONT = "underscore"; Not supported!
      DECREMENT_FONT = "mod1+bla";
      PIPECMD = "control+shift+u";
    };
  };
}
