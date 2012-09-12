#!/usr/bin/python

import os
import time

def add_dir_contents_to_ht(ht, dir1, base):
    l = os.listdir(dir1);
    for name in l:
        if os.path.isdir(os.path.join(dir1, name)):
            add_dir_contents_to_ht(ht, os.path.join(dir1, name),
                                   os.path.join(base, name));
        else:
            key = os.path.join(base, name);
            if ht.has_key(key):
                print "ERROR: the key for already exists in the ht.";
                exit();
            else:
                ht[key]= 0;

    return;
                                                    
def main():
    dir1 = "abcdef";
    dir2 = "abcdef";
    dir3 = "asdfad";

    if dir1 == dir2:
        print "test1 passed";
    if dir2 != dir3:
        print "test2 passed";


    l = os.listdir(".");

    for item in l:
        print "%s" % item;

    h = {};
    add_dir_contents_to_ht(h, ".", "");
    print h;

    k = time.time();
    t = "%.100f" % k;

    t = float(t);
    print t - k;
    print k;
    print type(k);

    if t - k == 0:
        print "yes";

    name = ".aweifjaowe";
    if name[0]  == '.':
        print "works";

    return;


if __name__ == '__main__':
    main();
