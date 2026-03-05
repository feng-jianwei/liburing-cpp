# liburing-cpp
```shell
git clone https://github.com/feng-jianwei/liburing-cpp.git && cd liburing-cpp
conan install . --build=missing --profile:all=profiles/clang-release --deployer=full_deploy --deployer-folder=third_party

cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -S . -B build/Release -G Ninja

cmake --build build/Release --parallel 16 --target all
```