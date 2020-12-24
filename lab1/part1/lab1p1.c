/*
* File: lab1p1.c
* Programmer: Zach Nett
* Assignment: Lab 1 part 1, CS 360 Fall 2020
* Date: 8/29/20
*/
#include "lab1.h"

char* system_type_table[0xff];

int fd;
char buf[512];
PARTITION *p;

int main(int argc, char *argv[])
{
    char *disk;
    if (argc == 2) {
        disk = argv[1];
    } else {
        disk = "vdisk";
    }

    set_types();

    fd = open(disk, O_RDONLY);
    printf("fd = %d\n", fd);

    int bytes_read = 0;
    bytes_read = read_sector(fd, 0, buf);
    printf("%d bytes read successfully\n", bytes_read);

    // If read_sector had a read error, exit program w/ errors
    if (bytes_read == -1)
        return EXIT_FAILURE;

    // MBR at offset 0x1BE of sector 0
    p = (PARTITION *)(buf + 0x1BE);

    // Print header-- note escape characters are used for bold text (since assumed platform is linux)
    printf("\e[1mDevice\tBoot\tStart\tEnd\tSectors\tSize\tId\tType\e[0m\n");

    int device_number = 1;

    // Read 4 entries of MBR (or until sys_type is 5, or 0)
    while (p->sys_type != 5 && p->sys_type != 0) {
        printf("%s%d\t\t%d\t%d\t%d\t%dK\t%x\t%s\n", disk, device_number, p->start_sector, (p->start_sector + p->nr_sectors) - 1,
               p->nr_sectors, (p->nr_sectors * 512) / 1024, p->sys_type, system_type_table[p->sys_type]);
        device_number++;
        p++;
    }

    // if partition type is EXTENDED, it forms a linked list
    if (p->sys_type == 5)
    {
        // Print/process 4th partition (or nth with a sys_type of 5)
        printf("%s%d\t\t%d\t%d\t%d\t%dK\t%x\t%s\n", disk, device_number, p->start_sector, (p->start_sector + p->nr_sectors) - 1,
                p->nr_sectors, (p->nr_sectors * 512) / 1024, p->sys_type, system_type_table[p->sys_type]);
        device_number++;

        // all start sectors are relative to P4's start sector, so we need to keep track of it
        u32 head = p->start_sector;
        u32 current = head;
        while (p->sys_type != 0)
        {
            read_sector(fd, current, buf);
            p = (PARTITION *)(buf + 0x1BE);
            printf("%s%d\t\t%d\t%d\t%d\t%dK\t%x\t%s\n", disk, device_number, current + p->start_sector, (current + p->start_sector + p->nr_sectors) - 1,
                   p->nr_sectors, (p->nr_sectors * 512) / 1024, p->sys_type, system_type_table[p->sys_type]);
            device_number++;
            p++;
            current = head + p->start_sector;
            // current node is head plus offset
        }
    }

    close(fd);

    return EXIT_SUCCESS;
}

// read_sector() function taken from h3.c help code
int read_sector(int fd, int sector, char *buf)
{
    int n;
    lseek(fd, sector * 512, SEEK_SET);
    n = read(fd, buf, 512);
    if (n <= 0)
    {
        printf("read failed\n");
        return -1;
    }
    return n;
}

// Definitions taken from https://www.win.tue.nl/~aeb/partitions/partition_types-1.html (partition identifiers)
void set_types(void) {
    system_type_table[0x00]="Empty";
    system_type_table[0x01]="DOS 12-bit FAT";
    system_type_table[0x02]="XENIX root";
    system_type_table[0x03]="XENIX /usr";
    system_type_table[0x04]="DOS 3.0+ 16-bit FAT (up to 32M)";
    system_type_table[0x05]="Extended";
    system_type_table[0x06]="FAT16";
    system_type_table[0x07]="HPFS/NTFS/exFAT";
    system_type_table[0x08]="OS/2 (v1.0-1.3 only)";
    system_type_table[0x08]="AIX boot partition";
    system_type_table[0x08]="SplitDrive";
    system_type_table[0x08]="Commodore DOS";
    system_type_table[0x08]="DELL partition spanning multiple drives";
    system_type_table[0x08]="QNX 1.x and 2.x (\"qny\")";
    system_type_table[0x09]="AIX data partition";
    system_type_table[0x09]="Coherent filesystem";
    system_type_table[0x09]="QNX 1.x and 2.x (\"qnz\")";
    system_type_table[0x0a]="OS/2 Boot Manager";
    system_type_table[0x0a]="Coherent swap partition";
    system_type_table[0x0a]="OPUS";
    system_type_table[0x0b]="WIN95 OSR2 FAT32";
    system_type_table[0x0c]="W95 FAT32 (LBA)";
    system_type_table[0x0d]="SILICON SAFE";
    system_type_table[0x0e]="WIN95: DOS 16-bit FAT, LBA-mapped";
    system_type_table[0x0f]="WIN95: Extended partition, LBA-mapped";
    system_type_table[0x10]="OPUS (?)";
    system_type_table[0x11]="Hidden DOS 12-bit FAT";
    system_type_table[0x11]="Leading Edge DOS 3.x";
    system_type_table[0x12]="Configuration/diagnostics partition";
    system_type_table[0x14]="Hidden DOS 16-bit FAT &lt;32M";
    system_type_table[0x14]="AST DOS with ";
    system_type_table[0x16]="Hidden DOS 16-bit FAT &gt;=32M";
    system_type_table[0x17]="Hidden IFS (e.g., HPFS)";
    system_type_table[0x18]="AST SmartSleep Partition";
    system_type_table[0x19]="Unused";
    system_type_table[0x1b]="Hidden WIN95 OSR2 FAT32";
    system_type_table[0x1c]="Hidden WIN95 OSR2 FAT32, LBA-mapped";
    system_type_table[0x1e]="Hidden WIN95 16-bit FAT, LBA-mapped";
    system_type_table[0x20]="Unused";
    system_type_table[0x21]="Reserved";
    system_type_table[0x21]="Unused";
    system_type_table[0x22]="Unused";
    system_type_table[0x23]="Reserved";
    system_type_table[0x24]="NEC DOS 3.x";
    system_type_table[0x26]="Reserved";
    system_type_table[0x27]="PQservice";
    system_type_table[0x27]="Windows RE hidden partition";
    system_type_table[0x27]="MirOS partition";
    system_type_table[0x27]="RouterBOOT kernel partition";
    system_type_table[0x2a]="AtheOS File System (AFS)";
    system_type_table[0x2b]="SyllableSecure (SylStor)";
    system_type_table[0x31]="Reserved";
    system_type_table[0x32]="NOS";
    system_type_table[0x33]="Reserved";
    system_type_table[0x34]="Reserved";
    system_type_table[0x35]="JFS on OS/2 or eCS ";
    system_type_table[0x36]="Reserved";
    system_type_table[0x38]="THEOS ver 3.2 2gb partition";
    system_type_table[0x39]="Plan 9 partition";
    system_type_table[0x39]="THEOS ver 4 spanned partition";
    system_type_table[0x3a]="THEOS ver 4 4gb partition";
    system_type_table[0x3b]="THEOS ver 4 extended partition";
    system_type_table[0x3c]="PartitionMagic recovery partition";
    system_type_table[0x3d]="Hidden NetWare";
    system_type_table[0x40]="Venix 80286";
    system_type_table[0x40]="PICK";
    system_type_table[0x41]="Linux/MINIX (sharing disk with DRDOS)";
    system_type_table[0x41]="Personal RISC Boot";
    system_type_table[0x41]="PPC PReP (Power PC Reference Platform) Boot";
    system_type_table[0x42]="Linux swap (sharing disk with DRDOS)";
    system_type_table[0x42]="SFS (Secure Filesystem)";
    system_type_table[0x42]="Windows 2000 dynamic extended partition marker";
    system_type_table[0x43]="Linux native (sharing disk with DRDOS)";
    system_type_table[0x44]="GoBack partition";
    system_type_table[0x45]="Boot-US boot manager";
    system_type_table[0x45]="Priam";
    system_type_table[0x45]="EUMEL/Elan ";
    system_type_table[0x46]="EUMEL/Elan ";
    system_type_table[0x47]="EUMEL/Elan ";
    system_type_table[0x48]="EUMEL/Elan ";
    system_type_table[0x4a]="Mark Aitchison's ALFS/THIN lightweight filesystem for DOS";
    system_type_table[0x4a]="AdaOS Aquila (Withdrawn)";
    system_type_table[0x4c]="Oberon partition";
    system_type_table[0x4d]="QNX4.x";
    system_type_table[0x4e]="QNX4.x 2nd part";
    system_type_table[0x4f]="QNX4.x 3rd part";
    system_type_table[0x4f]="Oberon partition";
    system_type_table[0x50]="OnTrack Disk Manager (older versions) RO";
    system_type_table[0x50]="Lynx RTOS";
    system_type_table[0x50]="Native Oberon (alt)";
    system_type_table[0x51]="OnTrack Disk Manager RW (DM6 Aux1)";
    system_type_table[0x51]="Novell";
    system_type_table[0x52]="CP/M";
    system_type_table[0x52]="Microport SysV/AT";
    system_type_table[0x53]="Disk Manager 6.0 Aux3";
    system_type_table[0x54]="Disk Manager 6.0 Dynamic Drive Overlay (DDO)";
    system_type_table[0x55]="EZ-Drive";
    system_type_table[0x56]="Golden Bow VFeature Partitioned Volume.";
    system_type_table[0x56]="DM converted to EZ-BIOS";
    system_type_table[0x56]="AT&amp;T MS-DOS 3.x";
    system_type_table[0x57]="DrivePro";
    system_type_table[0x57]="VNDI Partition";
    system_type_table[0x5c]="Priam EDisk";
    system_type_table[0x61]="SpeedStor";
    system_type_table[0x63]="Unix System V (SCO, ISC Unix, UnixWare, ...), Mach, GNU Hurd";
    system_type_table[0x64]="PC-ARMOUR protected partition";
    system_type_table[0x64]="Novell Netware 286, 2.xx";
    system_type_table[0x65]="Novell Netware 386, 3.xx or 4.xx";
    system_type_table[0x66]="Novell Netware SMS Partition";
    system_type_table[0x67]="Novell";
    system_type_table[0x68]="Novell";
    system_type_table[0x69]="Novell Netware 5+, Novell Netware NSS Partition";
    system_type_table[0x6e]="??";
    system_type_table[0x70]="DiskSecure Multi-Boot";
    system_type_table[0x71]="Reserved";
    system_type_table[0x72]="V7/x86";
    system_type_table[0x73]="Reserved";
    system_type_table[0x74]="Reserved";
    system_type_table[0x74]="Scramdisk partition";
    system_type_table[0x75]="IBM PC/IX";
    system_type_table[0x76]="Reserved";
    system_type_table[0x77]="M2FS/M2CS partition";
    system_type_table[0x77]="VNDI Partition";
    system_type_table[0x78]="XOSL FS";
    system_type_table[0x7e]="Unused";
    system_type_table[0x7f]="Unused";
    system_type_table[0x80]="MINIX until 1.4a";
    system_type_table[0x81]="MINIX since 1.4b, early Linux";
    system_type_table[0x81]="Mitac disk manager";
    system_type_table[0x82]="Linux swap / Solaris";
    system_type_table[0x83]="Linux";
    system_type_table[0x84]="OS/2 hidden C: drive";
    system_type_table[0x84]="Hibernation partition";
    system_type_table[0x85]="Linux extended partition";
    system_type_table[0x86]="Old Linux RAID partition superblock";
    system_type_table[0x86]="FAT16 volume set";
    system_type_table[0x87]="NTFS volume set";
    system_type_table[0x88]="Linux plaintext partition table";
    system_type_table[0x8a]="Linux Kernel Partition (used by AiR-BOOT)";
    system_type_table[0x8b]="Legacy Fault Tolerant FAT32 volume";
    system_type_table[0x8c]="Legacy Fault Tolerant FAT32 volume using BIOS extd INT 13h";
    system_type_table[0x8d]="Free FDISK 0.96+ hidden Primary DOS FAT12 partitition";
    system_type_table[0x8e]="Linux Logical Volume Manager partition";
    system_type_table[0x90]="Free FDISK 0.96+ hidden Primary DOS FAT16 partitition";
    system_type_table[0x91]="Free FDISK 0.96+ hidden DOS extended partitition";
    system_type_table[0x92]="Free FDISK 0.96+ hidden Primary DOS large FAT16 partitition";
    system_type_table[0x93]="Hidden Linux native partition";
    system_type_table[0x93]="Amoeba";
    system_type_table[0x94]="Amoeba bad block table";
    system_type_table[0x95]="MIT EXOPC native partitions";
    system_type_table[0x96]="CHRP ISO-9660 filesystem";
    system_type_table[0x97]="Free FDISK 0.96+ hidden Primary DOS FAT32 partitition";
    system_type_table[0x98]="Free FDISK 0.96+ hidden Primary DOS FAT32 partitition (LBA)";
    system_type_table[0x98]="Datalight ROM-DOS Super-Boot Partition";
    system_type_table[0x99]="DCE376 logical drive";
    system_type_table[0x9a]="Free FDISK 0.96+ hidden Primary DOS FAT16 partitition (LBA)";
    system_type_table[0x9b]="Free FDISK 0.96+ hidden DOS extended partitition (LBA)";
    system_type_table[0x9e]="ForthOS partition";
    system_type_table[0x9f]="BSD/OS";
    system_type_table[0xa0]="Laptop hibernation partition";
    system_type_table[0xa1]="Laptop hibernation partition";
    system_type_table[0xa1]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xa3]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xa4]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xa5]="BSD/386, 386BSD, NetBSD, FreeBSD";
    system_type_table[0xa6]="OpenBSD";
    system_type_table[0xa6]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xa7]="NeXTStep";
    system_type_table[0xa8]="Mac OS-X";
    system_type_table[0xa9]="NetBSD";
    system_type_table[0xaa]="Olivetti Fat 12 1.44MB Service Partition";
    system_type_table[0xab]="Mac OS-X Boot partition";
    system_type_table[0xab]="GO! partition";
    system_type_table[0xad]="RISC OS ADFS";
    system_type_table[0xae]="ShagOS filesystem";
    system_type_table[0xaf]="ShagOS swap partition";
    system_type_table[0xaf]="MacOS X HFS";
    system_type_table[0xb0]="BootStar Dummy";
    system_type_table[0xb1]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xb1]="QNX Neutrino Power-Safe filesystem";
    system_type_table[0xb2]="QNX Neutrino Power-Safe filesystem";
    system_type_table[0xb3]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xb3]="QNX Neutrino Power-Safe filesystem";
    system_type_table[0xb4]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xb6]="HP Volume Expansion (SpeedStor variant)";
    system_type_table[0xb6]="Corrupted Windows NT mirror set (master), FAT16 file system";
    system_type_table[0xb7]="Corrupted Windows NT mirror set (master), NTFS file system";
    system_type_table[0xb7]="BSDI BSD/386 filesystem";
    system_type_table[0xb8]="BSDI BSD/386 swap partition";
    system_type_table[0xbb]="Boot Wizard hidden";
    system_type_table[0xbc]="Acronis backup partition";
    system_type_table[0xbd]="BonnyDOS/286";
    system_type_table[0xbe]="Solaris 8 boot partition";
    system_type_table[0xbf]="New Solaris x86 partition";
    system_type_table[0xc0]="CTOS";
    system_type_table[0xc0]="REAL/32 secure small partition";
    system_type_table[0xc0]="NTFT Partition";
    system_type_table[0xc0]="DR-DOS/Novell DOS secured partition";
    system_type_table[0xc1]="DRDOS/secured (FAT-12)";
    system_type_table[0xc2]="Unused";
    system_type_table[0xc2]="Hidden Linux";
    system_type_table[0xc3]="Hidden Linux swap";
    system_type_table[0xc4]="DRDOS/secured (FAT-16, &lt; 32M)";
    system_type_table[0xc5]="DRDOS/secured (extended)";
    system_type_table[0xc6]="DRDOS/secured (FAT-16, &gt;= 32M)";
    system_type_table[0xc6]="Windows NT corrupted FAT16 volume/stripe set";
    system_type_table[0xc7]="Windows NT corrupted NTFS volume/stripe set";
    system_type_table[0xc7]="Syrinx boot";
    system_type_table[0xc8]="Reserved for DR-DOS 8.0+";
    system_type_table[0xc9]="Reserved for DR-DOS 8.0+";
    system_type_table[0xca]="Reserved for DR-DOS 8.0+";
    system_type_table[0xcb]="DR-DOS 7.04+ secured FAT32 (CHS)/";
    system_type_table[0xcc]="DR-DOS 7.04+ secured FAT32 (LBA)/";
    system_type_table[0xcd]="CTOS Memdump? ";
    system_type_table[0xce]="DR-DOS 7.04+ FAT16X (LBA)/";
    system_type_table[0xcf]="DR-DOS 7.04+ secured EXT DOS (LBA)/";
    system_type_table[0xd0]="REAL/32 secure big partition";
    system_type_table[0xd0]="Multiuser DOS secured partition";
    system_type_table[0xd1]="Old Multiuser DOS secured FAT12";
    system_type_table[0xd4]="Old Multiuser DOS secured FAT16 &lt;32M";
    system_type_table[0xd5]="Old Multiuser DOS secured extended partition";
    system_type_table[0xd6]="Old Multiuser DOS secured FAT16 &gt;=32M";
    system_type_table[0xd8]="CP/M-86";
    system_type_table[0xda]="Non-FS Data";
    system_type_table[0xda]="Powercopy Backup";
    system_type_table[0xdb]="Digital Research CP/M, Concurrent CP/M, Concurrent DOS";
    system_type_table[0xdb]="CTOS (Convergent Technologies OS -Unisys)";
    system_type_table[0xdb]="KDG Telemetry SCPU boot";
    system_type_table[0xdd]="Hidden CTOS Memdump? ";
    system_type_table[0xde]="Dell PowerEdge Server utilities (FAT fs)";
    system_type_table[0xdf]="DG/UX virtual disk manager partition";
    system_type_table[0xdf]="BootIt EMBRM";
    system_type_table[0xe0]="Reserved by ";
    system_type_table[0xe1]="DOS access or SpeedStor 12-bit FAT extended partition";
    system_type_table[0xe3]="DOS R/O or SpeedStor";
    system_type_table[0xe4]="SpeedStor 16-bit FAT extended partition &lt; 1024 cyl.";
    system_type_table[0xe5]="Tandy MSDOS with";
    system_type_table[0xe6]="Storage Dimensions SpeedStor";
    system_type_table[0xe8]="LUKS";
    system_type_table[0xea]="Rufus extra partition";
    system_type_table[0xea]="Freedesktop boot";
    system_type_table[0xeb]="BeOS BFS";
    system_type_table[0xec]="SkyOS SkyFS";
    system_type_table[0xed]="Unused";
    system_type_table[0xee]="Indication that this legacy MBR is followed by an EFI header";
    system_type_table[0xef]="Partition that contains an EFI file system";
    system_type_table[0xf0]="Linux/PA-RISC boot loader";
    system_type_table[0xf1]="Storage Dimensions SpeedStor";
    system_type_table[0xf2]="DOS 3.3+ secondary partition";
    system_type_table[0xf3]="Reserved";
    system_type_table[0xf4]="SpeedStor large partition";
    system_type_table[0xf4]="Prologue single-volume partition";
    system_type_table[0xf5]="Prologue multi-volume partition";
    system_type_table[0xf6]="Storage Dimensions SpeedStor";
    system_type_table[0xf7]="DDRdrive Solid State File System";
    system_type_table[0xf9]="pCache";
    system_type_table[0xfa]="Bochs";
    system_type_table[0xfb]="VMware File System partition";
    system_type_table[0xfc]="VMware Swap partition";
    system_type_table[0xfd]="Linux raid partition with autodetect using persistent superblock";
    system_type_table[0xfe]="SpeedStor &gt; 1024 cyl.";
    system_type_table[0xfe]="LANstep";
    system_type_table[0xfe]="IBM PS/2 IML (Initial Microcode Load) partition,";
    system_type_table[0xfe]="Windows NT Disk Administrator hidden partition";
    system_type_table[0xfe]="Linux Logical Volume Manager partition (old)";
    system_type_table[0xff]="Xenix Bad Block Table";
}