# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# This script generates jit/CachetGenerate.h and jit/CachetGenerated.inc
# CacheIR.cachet

import buildconfig
import yaml
import six
import os
import subprocess
from pathlib import Path
from tempfile import TemporaryDirectory
from collections import OrderedDict
from mozbuild.preprocessor import Preprocessor

def generate_cachet_impl(c_out, cachet_src):
    output_ext = Path(c_out.name).suffix
    with TemporaryDirectory() as dir:
        h_tmp = os.path.join(dir, "CachetGenerated.h")
        inc_tmp = os.path.join(dir, "CachetGenerated.inc")

        script_file_path = os.path.abspath(__file__)
        jit_dir_path = os.path.dirname(script_file_path)
        cachet_compiler_exe_path = os.path.abspath(os.path.join(jit_dir_path,
            "..", "..", "..", "..", "cachet", "target", "release", "cachet-compiler"))
        p = subprocess.Popen([cachet_compiler_exe_path, cachet_src, "--cpp-decls", h_tmp,
            "--cpp-defs", inc_tmp], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        stdout, stderr = p.communicate()
        stdout = six.ensure_text(stdout)
        stderr = six.ensure_text(stderr)
        if p.returncode != 0:
            print(stdout)
            print(stderr)


        out_file = h_tmp if output_ext == ".h" else inc_tmp

        with open(out_file) as f:
            c_out.write(f.read())
