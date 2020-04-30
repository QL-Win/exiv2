mkdir x64
cd x64

conan install ../.. --build missing --profile msvc2019Release64
cmake -G "Visual Studio 16 2019" -A x64 -S ../.. -B .
cmake --build . --config Release

cd ..
