{ nixpkgs ? import <nixpkgs> { } }:
let
  mkTest = config: nixpkgs.callPackage ./default.nix { config.ate = config; };
  ate = mkTest {};
in {
  default = mkTest {
    keybindings = {
      # INCREMENT_FONT = "underscore"; Not supported!
      DECREMENT_FONT = "mod1+bla";
      PIPECMD = "control+shift+u";
    };
  };
  checkLinting = nixpkgs.runCommand "check-liniting.sh" {
    nativeBuildInputs = [ nixpkgs.clang-tools nixpkgs.gnumake ];
  } ''
    set -e
    cp -rv ${ate.src} formatted
    chmod +rw -R formatted
    cd formatted
    make format
    cd ..
    # diff exists with !=0 in case there is a diff
    diff -ru ${ate.src} formatted
    touch $out
  '';
}
