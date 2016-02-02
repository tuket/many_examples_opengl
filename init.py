import os
from subprocess import call

os.chdir("externals/glew/auto")
call("make")

os.chdir("..")
call("make")
