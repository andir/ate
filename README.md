# ate - awesome terminal emulator

Is my personal terminal emulator tailor made for what I need and just what I need.

Goals:
  - Work on X11
  - Work on Wayland without XWayland
  - Scrollback
  - Reflow
  - Emoji support
  - Font resizing with ctrl-shift-plus/ctrl-shift-minus
  - Compile time configuration

Non-Goals:
   - Parsing configuration files

The application is trivial and maintaing your custom patch on top of this is
likewise. I do not intend to change a lot nor do I promisve compatibility with
any of your changes.

## Configuration

You can change the configuration by setting `nixpkgs.config.ate` or passing an attribute set named `config.ate` to `callPackage`.

Example:
```
ate = pkgs.callPackage path/to/this/repo/default.nix {
    config.ate = {
      options = {
        BLACK = "#000000";
        RED = ...;
        GREEN = ...;
        YELLOW = ...;
        BLUE = ...;
        MAGENTA = ...;
        CYAN = ...;
        WHITE = ...;
        BRIGHT_BLACK = ...;
        BRIGHT_RED = ...;
        BRIGHT_GREEN = ...;
        BRIGHT_YELLOW = ...;
        BRIGHT_BLUE = ...;
        BRIGHT_MAGENTA = ...;
        BRIGHT_CYAN = ...;
        BRIGHT_WHITE = ...;
        FOREGROUND_COLOR = ...;
        BACKGROUND_COLOR = ...;
        PIPECMD = path/to/a/command/which/processes/terminal/content/on/stdin;
      };
      keybindings = {
        RESET_FONT = "control+0";
        DECREMENT_FONT =
          "control+shift+underscore"; # Works like Control+Shift+Minus on qwerty and qwertz
        INCREMENT_FONT = "control+shift+plus;
        PASTE = "control+mod1+y"; # Mod1 == Alt
        PIPECMD = "control+mod1+u";
      };
    };
  };

