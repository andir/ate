{ stdenv, pkgs, pkgconfig, gnome3, gnumake, lib, makeWrapper, xurls, rofi, config }:
#
# Pass config like
#
# nixpkgs.config.ate.options = { BACKGROUND_COLOR = "#000000"; }
# nixpkgs.config.ate.keybindings = { INCREMENT_FONT = "control+plus"; }
# 
# See https://developer.gnome.org/gdk3/stable/gdk3-Windows.html#GdkModifierType for possible modifiers.
# The format is "(<modifier>\+)+<keyname>" and the <modifier> will be used in the form GDK_<modifier>_MASK.
# The keyname will be passed to https://developer.gnome.org/gdk3/stable/gdk3-Keyboard-Handling.html#gdk-keyval-from-name
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
    mkEscapedValue = name: value: lib.escapeShellArg ''"${value}"'';
    stringDefines = lib.mapAttrs mkEscapedValue (defaultConfig // (config.ate.options or {}));
    keybindings = (config.ate.keybindings or {});
    listOfKeybindings = lib.mapAttrsToList (key: value: let
      keyList = lib.reverseList (lib.splitString "+" value);
      modifiers = builtins.tail keyList;
      keyValue = builtins.head keyList;
    in {
      "${key}_KEYVAL" = mkEscapedValue "" keyValue;
      "${key}_MODIFIER_MASK" = lib.concatStringsSep " | " (map (m: "GDK_${lib.toUpper m}_MASK") modifiers);
    }) keybindings;
    keybindingDefines = lib.foldr (accu: val: accu // val) {} listOfKeybindings;
    mkCFlag = key: value: "-D${key}=${value}";
    configFlags = lib.mapAttrsToList mkCFlag (keybindingDefines // stringDefines);
  in lib.concatStringsSep " " configFlags;

  installPhase = ''
    mkdir -p $out/bin
    cp ate $out/bin

    # for compat
    ln -s $out/bin/ate $out/bin/st
  '';
}
