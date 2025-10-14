#!/usr/bin/env python3
import os
import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PYDIR = ROOT
BINDIR = PYDIR / 'bin'
BINDIR.mkdir(parents=True, exist_ok=True)

artifacts = ['chess_engine.exe','libchess.dll','libchess.a','engine_out.txt']
for name in artifacts:
    src = PYDIR / name
    if src.exists():
        dst = BINDIR / name
        shutil.move(str(src), str(dst))

for src in PYDIR.glob('*.o'):
    dst = BINDIR / src.name
    shutil.move(str(src), str(dst))

print('Build artifacts moved to', BINDIR)
