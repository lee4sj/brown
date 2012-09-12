#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------
# Copyright (C) 2011
# Chris Siden <chris (at) cs.brown.edu>
# Marcelo Martins <martins (at) cs.brown.edu>
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
# -----------------------------------------------------------------------

from repotools import cutCode, dbg_msg, dbgPrint, setVerbose, srcCutdir
import argparse
import os
import os.path
import re
import stat
import sys
import shutil
import subprocess
import types

## ==============================================
## binKeepdir
## ==============================================
def binKeepdir(root_dir, dir_list, keep_exts=[], keep_files=[]):
    """Performs binary keeping of directory list from root. 
       Keep object files based on extensions provided in keep_exts
       from dir_list
    """
    assert type(root_dir) == types.StringType
    assert type(dir_list) == types.ListType or type(dir_list) == types.TupleType
    assert type(keep_exts) == types.ListType or type(keep_exts) == types.TupleType
    assert type(keep_files) == types.ListType or type(keep_exts) == types.TupleType
    # Assumes we have compiled the tree before working on it.
    # Compiling the tree is necessary if we want to submit the .a
    # libraries to non-lab students.


    # Keeps object files from subdir of dir_list based on file
    # extension pattern. Non-lab students must only have access to
    # libraries, but not be permitted to view source code. This prevents
    # lab students from retrieving the non-lab repo and peeking at the
    # library source code they should be implementing.
    for directory in dir_list:
        path = os.path.join(root_dir, directory)
        if not os.path.isdir(path):
            dbgPrint("\t%s is not a valid directory\n" % path, dbg_msg.WARNING)
            continue

        # Let's start by ignoring all files. We will add exceptions
		# later. This guarantees that no source code is kept on our new
		# git repo

		# WARNING: relies on git. If not using git, we need to remove
		# the file
        with open(os.path.join(path, ".gitignore"), "a") as fin:
            fin.write("*\n")

		# Now let's include our wanted files based on their extensions.
		# We can use this to keep the static libraries.
		# Note that we force its inclusion with git add since the files
		# might be ignore by .gitignore the root dir

        filenames = os.listdir(path)
        for file in filenames:
            basename, ext = os.path.splitext(file)
            if ext in keep_exts:
                filename = os.path.join(path, file)
                dbgPrint("Keeping file %s\n" % filename, dbg_msg.DEBUG)
                subprocess.call(["git", "add", "-f", filename])

                # Make sure file won't be compiled again by redo
				# If you don't use redo, just remove the code block below
                keep_do = filename + ".do"
                with open(keep_do, "a") as fin:
                    fin.write("#!/usr/bin/env bash\n\n# Nothing to do here, move along\n\nreturn 0")
                dbgPrint("Keeping file %s\n" % keep_do, dbg_msg.DEBUG)
                subprocess.call(["git", "add", "-f", keep_do])

        # Finally, let's keep files based on their names
        for file in keep_files:
           if file in filenames:
               dbgPrint("\tKeeping %s\n" % file, dbg_msg.DEBUG)
               subprocess.call(["git", "add", "-f", os.path.join(path, file)])

## ==============================================
## findRoot
## ==============================================
def findRoot(base, max_depth=5):
    """Find root of to-be-modified source tree"""
    assert type(base) == types.StringType
    assert os.path.isdir(base)

    depth = 0
    root_dir = ""

    # Keep going up the tree until we find the root
    while depth < max_depth:
        filenames = os.listdir(base)
        
		# Root for Weenix repo contains weenix and fsmaker files
        if ("weenix" in filenames) and ("fsmaker" in filenames):
            root_dir = os.path.abspath(base)
            break
        base = os.path.join(base, "..")
        depth += 1

    return root_dir

## ==============================================
## listModules
## ==============================================
def listModules(prefix="", modules=[]):
    assert type(prefix) == types.StringType
    assert type(modules) == types.ListType or type(modules) == types.TupleType

    sys.stdout.write(prefix)
    print " ".join(m for m in modules)

remove_files = [ "PROPOSAL","TAREADME","tools/167-diff" ]
keep_files = [ "Submodules" ]
executable_files = [ "conf-check","fsmaker","system-check","weenix","tools/conf-diff","tools/conf-vars","tools/make-weenix-repo.py","tools/redo-delocalize","tools/redo-localize","tools/repotools.py","tools/setup","user/stagemap" ]

remove_extensions = [ ".bin",".dbg",".files",".gdbcomm",".img",".o",".out" ]
src_extensions = [ ".h",".c",".py" ]
lib_extensions = [ ".a",".gdb" ]

# Map between module names and their source-code location
modules = {
    "DRIVERS": { "dirs": { "src": [ "kernel/drivers","kernel/drivers/disk","kernel/drivers/tty" ], "lib": [ "kernel/drivers","kernel/drivers/disk","kernel/drivers/tty" ] } },\
    "PROCS": { "dirs": { "src": [ "kernel/main","kernel/proc" ], "lib": [ "kernel/proc" ] } }, \
    "S5FS": { "dirs": { "src": [ "kernel/fs/s5fs","kernel/mm" ], "lib": [ "kernel/fs/s5fs" ] } }, \
    "VFS":  { "dirs": { "src": [ "kernel/fs" ], "lib": [ "kernel/fs" ] } }, \
    "VM":   { "dirs": { "src": [ "kernel/api","kernel/drivers","kernel/fs/s5fs","kernel/proc","kernel/vm" ], "lib": [ "kernel/vm" ] } }
}

# Map between extra features and their source-code location
extras = {
    "GETCWD":   { "dirs": [ "kernel/fs" ] }, \
	"MOUNTING": { "dirs": [ "kernel/fs" ] }, \
	"MTP":      { "dirs": [ "kernel/proc" ] }, \
	"SHADOWD":  { "dirs": [ ] }, \
	"UPREEMPT": { "dirs": [ "kernel/util" ] }
}

## ==============================================
## main
## ==============================================
def main():
    global remove_files
    global executable_files
    global verbose_mode

    global remove_extensions
    global src_extensions
    global lib_extensions
    global modules
    global extras

    list_modules = False
    list_extra = False
    binary_keep = False
    src_strip = False
    extra_strip = False

    aparser = argparse.ArgumentParser(description="Weenix repository generator from TA code")
    aparser.add_argument("--list-modules", action="store_true",	help="list available Weenix modules")
    aparser.add_argument("--list-extra", action="store_true", help="list available extra Weenix features")
    aparser.add_argument("--verbose", action="store_true", help="enable verbose output mode")
    aparser.add_argument("--binary", type=str, metavar="module_list", help="enable binary keeping for Weenix modules")
    aparser.add_argument("--cutsource", type=str, metavar="module_list", help="enable source stripping for Weenix modules")
    aparser.add_argument("--cutextra", type=str, metavar="extra_list", help="enable source stripping for extra Weenix features")

    if len(sys.argv) < 2:
        aparser.print_help()
        sys.exit(1)

    args = vars(aparser.parse_args())

    if args["verbose"]:
        verbose_mode = True
        setVerbose(True)
    if args["list_modules"]: list_modules = True
    if args["list_extra"]: list_extra = True
    if args["binary"]:
        binary_keep = True
        if args["binary"] == "all":
            binary_list = set(modules.keys())
        else:
            binary_list = set(args["binary"].split(","))
    if args["cutsource"]:
        src_strip = True
        if args["cutsource"] == "all":
            src_list = set(modules.keys())
        else:
            src_list = set(args["cutsource"].split(","))
    if args["cutextra"]:
        extra_strip = True
        if args["cutextra"] == "all":
            extra_list = set(extras.keys())
        else:
            extra_list = set(args["cutextra"].split(","))

    root_dir = findRoot(".")
    os.umask(0007)
    os.chdir(root_dir)

    # List modules
    if list_modules or list_extra:
        if binary_keep or src_strip:
            sys.stderr.write("ERROR: Listing, binary-keeping, and source-stripping are mutually-exclusive operations.\n")
            sys.exit(1)
        if list_modules:
            listModules("Modules: ", modules.keys())
        if list_extra:
            listModules("Extras: ", extras.keys())
        sys.exit()
    elif not binary_keep and not src_strip:
        sys.stderr.write("ERROR: Please define operation: module-listing, binary-keeping or source-stripping.\n")
        sys.exit(1)

    dbgPrint("Generating student tree...\n")

    # Make sure we are only working with supported modules
    if src_strip:
        diff = src_list - set(modules.keys())
        if len(diff) > 0:
            sys.stderr.write("ERROR: The following modules are not supported: %s\n" % str(diff))
            sys.exit(1)

    # Make sure we are only working with supported extras
    if extra_strip:
        diff = extra_list - set(extras.keys())
        if len(diff) > 0:
            sys.stderr.write("ERROR: The following extras are not supported: %s\n" % str(diff))
            sys.exit(1)

    # Make sure we are only working with supported modules
    if binary_keep:
        diff = binary_list - set(modules.keys())
        if len(diff) > 0:
            sys.stderr.write("ERROR: The following modules are not supported: %s\n" % str(diff))
            sys.exit(1)

    # Finally make sure we are not applying multiple operations to the
	# same module
    if (binary_keep and src_strip) and len(set(binary_list).intersection(src_list)) > 0:
        sys.stderr.write("ERROR: We can't apply multiple operations to the same modules\n")
        sys.exit(1)

    # Compile code first to make sure that there are no errors. Also
	# needed for keeping binaries
    dbgPrint("Compiling code...\n")

    # If you don't use redo, substitute the following clode block
    subprocess.call([os.path.join(root_dir, "tools/setup")])
    subprocess.call(["redo", "-j", "8"])

    dbgPrint("Removing .git...\n")
    shutil.rmtree(os.path.join(root_dir, ".git"))

    dbgPrint("Removing TA-only files...\n")
    for rfile in remove_files:
        path = os.path.join(root_dir, rfile)
        if (os.path.isdir(path)):
            dbgPrint("\tRemoving %s\n" % path, dbg_msg.DEBUG)
            shutil.rmtree(path)
        else:
            dbgPrint("\tRemoving %s\n" % path, dbg_msg.DEBUG)
            os.remove(path)
 
    dbgPrint("Creating student weenix git repository...\n")
    subprocess.call(["git", "init"])

    if src_strip:
        diff = set(modules.keys()) - src_list

        # Remove TA code and produce stencil for students
        dbgPrint("Generating stencil code for modules %s...\n" % args["cutsource"])
        modules_dirs = []
        for m in src_list:
            if m in modules.keys(): modules_dirs.extend(modules[m]["dirs"]["src"])
        srcCutdir(root_dir, modules_dirs, diff.union(extras.keys()))

        if len(diff) > 0:
            dbgPrint("The following modules were left intact: %s\n" % (", ".join(m for m in diff)), dbg_msg.WARNING)

    if extra_strip:
        diff = set(extras.keys()) - extra_list

        # Remove TA code and produce stencil for students
        dbgPrint("Generating stencil code for extras %s...\n" % args["cutextra"])
        extras_dirs = []
        for m in extra_list:
            if m in extras.keys(): extras_dirs.extend(extras[m]["dirs"])
        srcCutdir(root_dir, extras_dirs, diff.union(modules.keys()))

        if len(diff) > 0:
            dbgPrint("The following extras were left intact: %s\n" % (", ".join(m for m in diff)), dbg_msg.WARNING)

    if binary_keep:
        # Do not include library source code for non-lab students
        dbgPrint("Ignoring source from modules %s...\n" % args["binary"])
        modules_dirs = []
        for m in binary_list:
            if m in modules.keys(): modules_dirs.extend(modules[m]["dirs"]["lib"])
        binKeepdir(root_dir, modules_dirs, lib_extensions)

        diff = set(modules.keys()) - binary_list
        if len(diff) > 0:
            dbgPrint("The following modules were left intact: %s\n" % (", ".join(m for m in diff)), dbg_msg.WARNING)

    # For those modules that only have binaries available, let's modify
	# redo rules, so that the .a files are not deleted when 'redo clean' is
	# called from the new repository
    if binary_keep:
        dbgPrint("Modifying redo files...\n")
        with open(os.path.join(root_dir, "kernel/Kernel.sh"), "r") as fin:
            lines = fin.readlines()
			# A line starting with CLEAN_MODULES= defines the list of
			# subdirectories available in kernel. Let's remove those who
			# have a binary-only file, so that it is not deleted.
            for n, line in enumerate(lines):
                match = re.match(r"\ACLEAN_MODULES=.*", line)
                if match:
                    for m in binary_list:
                        # I hate doing this, but it's necessary
                        if m == "VFS" or m == "S5FS": m = "fs"
                        line = re.sub(r"\s%s\s" % m.lower(), " ", line)
                        lines[n] = line
                    break

        with open(os.path.join(root_dir, "kernel/Kernel.sh"), "w") as fin:
            fin.writelines(lines)

    # Clean compile-created files. This is a necessary step to create a
	# working repo. If not, things won't compile next again (blame redo
	# for this). If you don't use redo, erase the code block below
    dbgPrint("Cleaning object files...\n")
    subprocess.call(["redo", "clean"])

    dbgPrint("Setting correct file/dir permissions...\n")
    os.chmod(root_dir, 0770)
    for root, dirs, files, in os.walk(root_dir):
        for file in files:
            dbgPrint("\tSetting %s permissions to 0664\n" %(os.path.join(root, file)), dbg_msg.DEBUG)
            os.chmod(os.path.join(root, file), 0664)
        for dir in dirs:
            dbgPrint("\tSetting %s permissions to 0775\n" %(os.path.join(root, dir)), dbg_msg.DEBUG)
            os.chmod(os.path.join(root, dir), 0775 | stat.S_ISGID)

    dbgPrint("Setting executable files...\n")
    for efile in executable_files:
        path = os.path.join(root_dir, efile)
        dbgPrint("\tMaking %s executable\n" % path, dbg_msg.DEBUG)
        os.chmod(path, 0775)

    subprocess.call(["git", "add", "."])
    subprocess.call(["git", "commit", "-m", "Created student weenix repository"])
    dbgPrint("DONE.\n")

if __name__ == "__main__":
    sys.exit(main())
