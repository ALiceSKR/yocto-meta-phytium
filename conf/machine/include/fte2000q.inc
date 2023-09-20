MACHINE_FEATURES += "efi pci"

DEFAULTTUNE = "aarch64"

# set IMAGETYPE
KERNEL_IMAGETYPE = "uImage Image.gz Image"

# serial port
SERIAL_CONSOLES = "115200;ttyAMA1"

IMAGE_INSTALL:append = " kernel-modules"

include conf/machine/include/arm/arch-armv8a.inc