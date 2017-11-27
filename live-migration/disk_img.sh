#!/bin/bash
#set -x

setup(){
	sudo losetup /dev/loop0  $1
	sudo kpartx -av /dev/loop0
	sudo pvscan
	sudo lvscan
	sudo lvchange -ay VolGroup00
	sudo lvscan
}

remove(){
	sudo lvchange -an VolGroup00
	sudo kpartx -d /dev/loop0
	sudo losetup -d /dev/loop0
}

usage(){
	echo -e "usage:"
	echo -e "$0 -i image_name: install image"
	echo -e "$0 -r :remove image"
	exit 0
}

main(){

	case $1 in
		-i)
			if [ $# -lt 2 ]
			then
				usage
			else
				setup $2	
			fi
			;;
		-r)
			if [ $# -gt 1 ]
			then
				usage
			else
				remove
			fi
			;;
		*)
			usage
	esac
}
main $@
