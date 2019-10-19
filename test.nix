{ nixpkgs ? import <nixpkgs> {}}:
let
  mkTest = config: nixpkgs.callPackage ./default.nix { config.ate = config; };
in
{
  default = mkTest {};
}
