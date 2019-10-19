{ stdenv, pkgs, pkgconfig, gnome3, gnumake, lib, makeWrapper, xurls, rofi, config ? {} }:
#
# Pass config like
#
# config = { BLACK = "#000000"; }
#
# For possible options see config.default.h
#
stdenv.mkDerivation {
  name = "ate-0.0.0";

  buildInputs = [ gnome3.vte or pkgs.vte makeWrapper ];
  nativeBuildInputs = [ pkgconfig gnumake pkgs.gdb pkgs.clang-tools ];

  src = ./.;

  CONFIG_CFLAGS = let
    pipecmd = pkgs.writeScript "pipecmd.sh" ''
      #! /bin/sh
      ${xurls}/bin/xurls | sort | uniq | ${rofi}/bin/rofi -dmenu | xargs -r firefox
    '';
    defaultConfig = { PIPECMD = toString pipecmd; };
    mkEscapedValue = value: lib.escapeShellArg ''"${value}"'';
    mkCFlag = key: value: "-D${key}=${mkEscapedValue value}";
    configFlags = lib.mapAttrsToList mkCFlag (defaultConfig // config);
  in lib.concatStringsSep " " configFlags;

  installPhase = ''
    mkdir -p $out/bin
    cp ate $out/bin

    # for compat
    ln -s $out/bin/ate $out/bin/st
  '';
}
