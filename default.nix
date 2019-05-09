{ stdenv, pkgs, pkgconfig, gnome3, gnumake }:
stdenv.mkDerivation {
  name = "at-0.0.0";

  buildInputs = [ gnome3.vte or pkgs.vte ];
  nativeBuildInputs = [ pkgconfig gnumake pkgs.gdb pkgs.clang-tools ];

  src = ./.;

  installPhase = ''
    mkdir -p $out/bin
    cp at $out/bin

    # for compat
    ln -s $out/bin/at $out/bin/st
  '';
}
