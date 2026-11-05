{
  description = "C++ Framework Dev Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      nativeBuildInputs = with pkgs; [
        cmake
        ninja
        pkg-config
        git
      ];

      buildInputs = with pkgs; [
        gcc
        gnutar
        curl
        zip
        unzip
        mesa
        libGL
        libx11
        libxcursor
        libxinerama
        libxi
        libxrandr
        libxext
        libxfixes
      ];

      shellHook = ''
        export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [
          pkgs.stdenv.cc.cc.lib
          pkgs.mesa
          pkgs.libGL
          pkgs.libx11
          pkgs.libxcursor
          pkgs.libxinerama
          pkgs.libxi
          pkgs.libxrandr
          pkgs.libxext
          pkgs.libxfixes
        ]}:$LD_LIBRARY_PATH"

          echo "C++ Dev Shell Loaded Successfully"
      '';
    };
  };
}
