#!/usr/bin/env python
import re
import subprocess

subprocess.call(['./ExampleApplication',])

print '\n::BEGIN EXCEPTIONS::'
for line in open('Ogre.log'):
  if re.search(r'EXCEPTION', line):
    print line
print '::END EXCEPTIONS::'

