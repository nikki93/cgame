with (import <nixpkgs> {});

let
  pname = "cgame";
  version = "0.0.1";
in

stdenv.mkDerivation rec {
  name = "${pname}-${version}";
  src = ./.;

  nativeBuildInputs = [ cmake ];
  buildInputs = [
    libGLU_combined
    xorg.libX11 xorg.libXrandr xorg.libXi xorg.libXxf86vm xorg.libXext
    openal
  ];
}
