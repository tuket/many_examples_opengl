import os
from subprocess import call

os.chdir("externals/glew")

call(["make", "extensions"])

