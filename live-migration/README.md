#Copy disk

##Files
disk.c disk_final.c

##Discription
This program is supposed to copy the used part in a virtual machine disk.
By now it only supports ext2/3/4 and Linux LVM, no support for Windows disk 
format,such like fat16/32,ntfs.
disk_img.sh is a shell script helper which can automatically install and remove LVM.

##Useage
Compile it
        $ gcc -D_FILE_OFFSET_BITS=64 disk_final.c -o a.out
    
If you just have ext filesystem, then,
        $ a.out device_name disk_file 
If you have Linux LVM, then you should specific the volume group name,
        $ a.out device_name disk_file vg_name
Install image: 
        $ disk_img.sh -i image_name
Remove image:
        $ disk_img.sh -r 


#Run live migration

##Files
pcopy_client.c pcopy_server.c

##Discription
The two file complete the function of disk live migration with the method of pre-copy.
pcopy_client.c is the starting end-point, it implements the main function of disk pre-copy live migration.
Using following command to compile:
        $ gcc -D_FILE_OFFSET_BITS=64 pcopy_client.c -lpthread -o cli
pcopy_server.c is the server point. It mainly receive data from socket and write data to local. 
Using following command to compile:
        $ gcc -D_FILE_OFFSET_BITS=64 pcopy_server.c -o serv

##Useage
In the server side, run:
        $ serv ip
And in the client side, run:
        $ cli disk_name ip
