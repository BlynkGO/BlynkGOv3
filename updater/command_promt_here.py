import subprocess
import os

# Get the current working directory
cwd = os.getcwd()

# Open a command prompt window at the current working directory
subprocess.call(['cmd.exe', '/k', 'cd', cwd])
