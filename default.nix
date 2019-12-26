{ stdenv
, config
, gnome3
, gnumake
, lib
, makeWrapper
, pkgconfig
, pkgs
, rofi
, xurls
}:
let
  pipecmd = pkgs.writeScript "pipecmd.sh" ''
    #! ${stdenv.shell}
    ${xurls}/bin/xurls | sort | uniq | ${rofi}/bin/rofi -dmenu | xargs -r firefox
  '';

  ateConfig = lib.recursiveUpdate {
    options = { PIPECMD = toString pipecmd; BACKGROUND_OPACITY = 0.8; };
    keybindings = {};
  } (config.ate or {});
in
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

  buildInputs = [ gnome3.vte or pkgs.vte ];
  nativeBuildInputs = [ pkgconfig gnumake makeWrapper ];

  # filter the .nix files from the repo
  src = lib.cleanSourceWith {
    filter = name: _type:
      let
        baseName = baseNameOf (toString name);
      in ! (lib.hasSuffix ".nix" baseName);
    src = lib.cleanSource ./.;
  };

  CONFIG_CFLAGS = import ./config.nix ({ inherit lib; } // ateConfig);

  installPhase = ''
    mkdir -p $out/bin
    cp ate $out/bin
  '';
}
