mkdir x32
cd x32

conan install ../.. --build missing --profile msvc2019Release32
cmake -G "Visual Studio 16 2019" -A Win32 -S ../.. -B .
cmake --build . --config Release

cd ..
