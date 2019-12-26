{ pkgs ? import <nixpkgs> {}}:
(pkgs.callPackage ./default.nix {}).overrideAttrs (old: {
  nativeBuildInputs = old.nativeBuildInputs ++ [
    pkgs.gdb
    pkgs.clang-tools
  ];
})
