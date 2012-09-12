#!/bin/bash

redo-ifchange Config.build
source Config.build

redo-ifchange System.build
source System.build

source Vars.sh
source Arch.sh

function misuse {
	local comm=$1
	local var=$2

	echo "error: '$comm' used directly, should only be used through $var variable, please fix script" >&2
	exit 1
}

unalias -a
shopt -s expand_aliases
error=

alias gcc='misuse gcc CC'
alias cc='misuse cc CC'
if [[ -z $CC ]]; then
	echo $CC_ERROR >&2
	error=1
fi

alias ld='misuse ld LD'
if [[ -z $LD ]]; then
	echo $LD_ERROR >&2
	error=1
fi

alias as='misuse as AS'
if [[ -z $AS ]]; then
	echo $AS_ERROR >&2
	error=1
fi

alias ar='misuse ar AR'
if [[ -z $AR ]]; then
	echo $AR_ERROR >&2
	error=1
fi

alias objcopy='misuse objcopy OBJCOPY'
if [[ -z $OBJCOPY ]]; then
	echo $OBJCOPY_ERROR >&2
	error=1
fi

alias objdump='misuse objdump OBJDUMP'
if [[ -z $OBJDUMP ]]; then
	echo $OBJDUMP_ERROR >&2
	error=1
fi

alias strings='misuse strings STRINGS'
if [[ -z $STRINGS ]]; then
	echo $STRINGS_ERROR >&2
	error=1
fi

alias strip='misuse strip STRIP'
if [[ -z $STRIP ]]; then
	echo $STRIP_ERROR >&2
	error=1
fi

alias size='misuse size SIZE'
if [[ -z $SIZE ]]; then
	echo $SIZE_ERROR >&2
	error=1
fi

alias readelf='misuse readelf READELF'
if [[ -z $READELF ]]; then
	echo $READELF_ERROR >&2
	error=1
fi

alias addr2line='misuse addr2line ADDR2LINE'
if [[ -z $ADDR2LINE ]]; then
	echo $ADDR2LINE_ERROR >&2
	error=1
fi

alias nm='misuse nm NM'
if [[ -z $NM ]]; then
	echo $NM_ERROR >&2
	error=1
fi

alias python='misuse python PYTHON'
if [[ -z $PYTHON ]]; then
	echo $PYTHON_ERROR >&2
	error=1
fi

if [[ -n $error ]]; then
	exit 1
fi

alias cscope='misuse cscope CSCOPE'
alias qemu='misuse qemu QEMU'
