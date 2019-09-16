{ stdenv, pkgs, pkgconfig, gnome3, gnumake, makeWrapper, xurls, rofi}:
stdenv.mkDerivation {
  name = "ate-0.0.0";

  buildInputs = [ gnome3.vte or pkgs.vte makeWrapper ];
  nativeBuildInputs = [ pkgconfig gnumake pkgs.gdb pkgs.clang-tools ];

  src = ./.;

  makeFlags = [
    "PIPECMD=\\\"${pkgs.writeScript "pipecmd.sh" ''
      #! /bin/sh
      ${xurls}/bin/xurls | sort | uniq | ${rofi}/bin/rofi -dmenu | xargs -r firefox
    ''}\\\""
  ];

  installPhase = ''
    mkdir -p $out/bin
    cp ate $out/bin

    # for compat
    ln -s $out/bin/ate $out/bin/st
  '';
}
