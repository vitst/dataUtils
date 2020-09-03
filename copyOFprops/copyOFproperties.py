#!/usr/bin/env python3

import os, sys
import shutil

args = sys.argv
sourceDir = args[1]
targetDir = args[2]

listInDir = os.listdir(targetDir)


targSub = [os.path.join(targetDir, sub) for sub in os.listdir(targetDir)
                    if os.path.isdir(os.path.join(targetDir,sub))]
targSubSor = sorted(targSub)

for tsdir in targSubSor:
    print("Copying data from {} to {}".format(sourceDir, tsdir))
    if 'constant' in os.listdir(tsdir):
        shutil.copytree(sourceDir, tsdir, dirs_exist_ok=True)
    else:
        answ = input('There is no constant directory in the current' \
                'subdirectory. Do you still want to copy files? (y/n): ')
        while (answ!='y' and answ!='n'):
            answ = input('There is no constant directory in the current' \
                    'subdirectory. Do you still want to copy files? (y/n): ')
        if answ == 'y':
            shutil.copytree(sourceDir, tsdir, dirs_exist_ok=True)


exit(0)
for root, dirs, files in os.walk("."):
  for filename in files:
    if '|' in filename:
      #print(root)
      #print(filename.replace("|",""))
      oldf = os.path.join(root, filename)
      newf = os.path.join(root, filename.replace("|",""))

      os.rename(oldf, newf)

print('end')


