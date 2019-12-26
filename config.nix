{ lib
, keybindings ? {}
, options ? {}
}:
let
  # escape a value for usage in the CC argument list
  mkEscapedValue = name: value:
    let
      # escape the value for the macro defintion
      # int's and floats aren't quoted but converted to string instead
      type = builtins.typeOf value;
      v =
        if lib.elem type [ "float" "int" ] then
          toString value
        else lib.escapeShellArg ''"${value}"'';
    in v;

  listOfKeybindings = lib.mapAttrsToList (key: value:
    let
      keyList = lib.reverseList (lib.splitString "+" value);
      modifiers = builtins.tail keyList;
      keyValue = builtins.head keyList;
    in {
      "${key}_KEYVAL" = mkEscapedValue "" keyValue;
      "${key}_MODIFIER_MASK" = ''"${lib.concatStringsSep " | " (map (m: "GDK_${lib.toUpper m}_MASK") modifiers)}"'';
    }) keybindings;

  keybindingDefines = lib.foldr (accu: val: accu // val) {} listOfKeybindings;
  optionDefines = lib.mapAttrs mkEscapedValue options;

  mkCFlag = key: value: "-D${key}=${value}";
in
  lib.concatStringsSep " " (
    lib.mapAttrsToList mkCFlag (keybindingDefines // optionDefines)
  )
