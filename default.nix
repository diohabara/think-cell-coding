with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "my-app";
  src = ./.;

  buildInputs = [
    gcc
    llvm
  ];

  buildPhase = "c++ -std=c++17 -o main main.cc";

  installPhase = ''
    mkdir -p $out/bin
    cp main $out/bin/
  '';
}
