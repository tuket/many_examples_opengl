```
git clone --recursive https://github.com/tuket/many_examples_opengl.git many_examples
cd many_examples
mkdir build
python init.py
cd build
cmake ..
make -j3
cd ..
./ex0
```

The examples have to be run from the root directory.

If you want to debug the examples run cmake like this:
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
