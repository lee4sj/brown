#!/usr/bin/python

import sys
import os
import string
import shutil
import time
import md5

#takes ht (hashtable), dir1(directory path), base(base to add to the key)
def add_dir_contents_to_ht(ht, dir1, base):
    l = os.listdir(dir1);

    for name in l:
        if name[0] == '.':
            pass;

        elif os.path.isdir(os.path.join(dir1, name)):
            add_dir_contents_to_ht(ht, os.path.join(dir1, name),
                                   os.path.join(base, name));
        else:
            key = os.path.join(base, name);
            if ht.has_key(key):
                print "ERROR: the key for already exists in the ht.";
                exit();
            else:
                ht[key] = os.path.getmtime(os.path.join(dir1, name));

    return;

def create_new_metadata(dir_put, dir_hash, metadata):
    h = string.replace(dir_hash, '/', '_');

    try:
        fname = dir_put + "/.unison" + h;
        f = open(fname, 'w');
        t = time.time();

        f.write("%.100f\n" % t);
        
        for i in metadata:
            f.write(i + "\n");

        f.close();
    except:
        print "could not create a new metadata";
        for i in sys.exc_info():
            print i;
        exit();

    return;

def get_mdata(dir1, dir2):
    #replace '/' with '_' for the purpose of finding metadata.
    h = string.replace(dir2, '/', '_');
    metadata = {};

    #first, find the metadata; if none, create one.
    try:
        fname = dir1 + "/.unison" + h;
        f = open(fname, 'r');
        mod_time = f.readline();
        if mod_time == "":
            print "ERROR: the metadata is not in correct format!";
            exit();

        mod_time = float(mod_time);
        line = f.readline();
        while len(line) != 0:
            filename = string.replace(line, "\n", "");
            metadata[filename] = mod_time;
            line = f.readline();

        f.close();
    except:
        print "no metadata"#do nothing.
        for i in sys.exc_info():
            print i;

    return metadata;

def sync_phase1(metadata, path1, dir1, path2, dir2):
    collisions = [];
    resolved = [];

    print "sync phase 1";


    for name in metadata:
        #if the file is in both dir1 and dir2
        if dir1.has_key(name) & dir2.has_key(name):
            #if file is not modified, no actions are required
            if ((metadata[name] >= dir1[name]) &
                (metadata[name] >= dir2[name])):
                resolved.append(name);

            #if the file has been modified only in one dir, propagate the change
            elif ((metadata[name] >= dir1[name]) &
                (metadata[name] < dir2[name])):
                base = os.path.dirname(os.path.join(path1, name));
                if (not os.path.isdir(base)):
                    os.makedirs(base);
                shutil.copyfile(os.path.join(path2, name),
                                os.path.join(path1, name));
                resolved.append(name);

            elif ((metadata[name] < dir1[name]) &
                  (metadata[name] >= dir2[name])):
                base = os.path.dirname(os.path.join(path2, name));
                if (not os.path.isdir(base)):
                    os.makedirs(base);
                shutil.copyfile(os.path.join(path1, name),
                                os.path.join(path2, name));
                resolved.append(name);

            #if file in both dirs are changed after previous sync, add collision
            elif ((metadata[name] < dir1[name]) &
                  (metadata[name] < dir2[name])):
                collisions.append(name);
            
            del dir1[name];
            del dir2[name];

        elif dir1.has_key(name) & (not dir2.has_key(name)):
            os.remove(os.path.join(path1, name));

        elif (not dir1.has_key(name)) & dir2.has_key(name):
            os.remove(os.path.join(path2, name));

    return (collisions, resolved);

def sync_phase2(path1, dir1, path2, dir2):
    collision = [];
    resolved = [];

    print "sync phase 2";

    for f in dir1:
        if dir2.has_key(f):
            collision.append(f);
            del dir2[f];
        else:
            resolved.append(f);
            base = os.path.dirname(os.path.join(path2, f));
            if (not os.path.isdir(base)):
                os.makedirs(base);
            shutil.copyfile(os.path.join(path1, f),
                            os.path.join(path2, f));

    for f in dir2:
        if dir1.has_key(f):
            collision.append(f);
            del dir1[f];
        else:
            resolved.append(f);
            base = os.path.dirname(os.path.join(path1, f));
            if (not os.path.isdir(base)):
                os.makedirs(base);
            shutil.copyfile(os.path.join(path2, f),
                            os.path.join(path1, f));

    return (collision, resolved);

def main():

    if (len(sys.argv) < 3):
        print "Usage: unison [dir1] [dir2]";
        return;

    dir1 = os.path.abspath(sys.argv[1]);
    dir2 = os.path.abspath(sys.argv[2]);

    #make sure the directories are two distinct directories
    if dir1 == dir2:
        print "The directories are the same directories!!!";
        return;

    print "** start synchronization of  %s and %s" % (dir1, dir2);

    #obtain the metadata (the last sync point)
    metadata = get_mdata(dir1, dir2);

    #create the hash tables for each directory contents
    dir1_ht = {};
    add_dir_contents_to_ht(dir1_ht, dir1, "");
    dir2_ht = {};
    add_dir_contents_to_ht(dir2_ht, dir2, "");

    #PHASE 1: go through files that was already in the last sync and sync them.
    (collisions, resolved) = sync_phase1(metadata, dir1, dir1_ht, dir2, dir2_ht);

    #PHASE 2: add the newly created files
    r = sync_phase2(dir1, dir1_ht, dir2, dir2_ht);

    collisions.extend(r[0]);
    resolved.extend(r[1]);

    create_new_metadata(dir1, dir2, resolved);
    create_new_metadata(dir2, dir1, resolved);


    #Print warnings for the collision!
    print "************ collisions ************";
    for f in collisions:
        print f;

    return;


if __name__ == '__main__':
    main();
