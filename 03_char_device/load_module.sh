#! /bin/bash
MAJOR=511

function load() {
    echo "Loading the module..."

    insmod chard.ko || exit 1

    rm -rf /dev/chard*

    # Re-make the special char files.
    mknod /dev/chard3 c ${MAJOR} 3
    mknod /dev/chard4 c ${MAJOR} 4

    # Script is run as superuser so the device files are owner by root.
    # Leave it as root for now.

    # Default permission bits will be that root has write access,
    # while everyone else has read access.
    chmod a+w /dev/chard*
}

function unload() {
    echo "Unloading the module..."

    rm -rf /dev/chard*
    rmmod chard.ko || exit 1
}



# https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameter-Expansion
# Sets default argument
argument=${1:-"load"}

case ${argument} in
    load)
        load
        ;;
    unload)
        unload
        ;;
    reload)
        # Continue on even if unload fails.
        ( unload )
        load
        ;;
    *)
        echo "Usage: $0 {load | unload | reload}"
        echo "Default is load."
        exit 1
        ;;
esac
