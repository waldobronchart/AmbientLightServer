AmbientLightServer
==================

This is an application I wrote that drives the Ambient Light rig I built. It captures colors from the TV using a webcam, then samples colors at the edge of the screen and updates a bunch of LEDs at the back with those colors.

The sampling region and other camera properties can be controlled by a separate client that connects up to this server. The client is a python qt application that lives in another repo: [https://github.com/waldobronchart/AmbientLightPyClient](https://github.com/waldobronchart/AmbientLightPyClient) 

A video of the project and details on how I built it can be found on my blog: 

* [https://waldobronchart.com/note/ambient-light-tv-with-a-raspberry-pi-part1](https://waldobronchart.com/note/ambient-light-tv-with-a-raspberry-pi-part1)
* [https://waldobronchart.com/note/ambient-light-tv-with-a-raspberry-pi-and-camera-part2](https://waldobronchart.com/note/ambient-light-tv-with-a-raspberry-pi-and-camera-part2)

Building the application
----------------------------------------

If you're interested in getting this to work and changing it fit your own needs, here's a long list of things you need to do to get it to build.

### Preparing the SD card

You'll need at least 4GB space on the card.

Get Arch Linux from: [http://www.raspberrypi.org/downloads](http://www.raspberrypi.org/downloads) (I used archlinux-hf-2013-07-22.img) and burn the image:

* Windows: burn it with [http://sourceforge.net/projects/win32diskimager/](http://sourceforge.net/projects/win32diskimager/)
* OSX: using Terminal, find the target sd volume name with `diskutil list` and unmount it with `unmountDisk /dev/...`. Then write the image with `dd if=archlinux.... of=/dev/...` (replacing /dev/... appropriately)

### Resizing the partition and access setup

Boot the pi and either plug in a keyboard or connect to it via ssh.

SSH should already be enabled, so you should be able to connect up to it via the Terminal with `ssh root@192.168.x.x` (Default password should be root too). On windows, use Putty.

From here, just follow this tutorial to repartition the card:
[http://jan.alphadev.net/post/53594241659/growing-the-rpi-root-partition](http://jan.alphadev.net/post/53594241659/growing-the-rpi-root-partition)

If you're going to be ssh'ing, I'd recommend setting up a static IP too:
[https://wiki.archlinux.org/index.php/Network_Configuration#Manual_connection_at_boot_using_systemd](https://wiki.archlinux.org/index.php/Network_Configuration#Manual_connection_at_boot_using_systemd)

It's probably also best to create some swap space in case we run out of memory while compiling:

    cd ~
    fallocate -l 512M /swapfile
    mkswap /swapfile
    swapon /swapfile

### Get all updates for Arch linux and installing build tools

    pacman -Syu
    pacman -S gcc pkg-config git make cmake

The first line updates your package databases and installs updates to packages already installed.

The second like installs the needed build tools we need to compile AmbiLightServer.

### Building AmbiLightServer

Clone this git repo or your fork to your home folder (default /root/).

    cd ~
    git clone https://github.com/waldobronchart/AmbientLightServer (or your own fork)

#### Install Prerequisites (Arch Linux)

    # Jannson: JSON library used for serialisation
    pacman -S jansson

    # Log4Cplus: logging library
    #  have a coffee because this will take 25-30mins
    #  it might also install to /usr/local/lib instead of /usr/lib
    cd ~
    wget http://downloads.sourceforge.net/project/log4cplus/log4cplus-stable/1.1.2/log4cplus-1.1.2-rc2.tar.gz
    tar -zxvf log4cplus-1.1.2-rc2.tar.gz
    rm log4cplus-1.1.2-rc2.tar.gz
    cd log4cplus-1.1.2-rc2
    ./configure
    make
    make install

    # Boost: used for network communication and update loop timers
    pacman -S boost

    # WiringPi: library to interface with the output pins
    pacman -S wiringpi

    # OpenCV: used for capturing frames from the webcam
    cd ~
    wget http://downloads.sourceforge.net/project/opencvlibrary/opencv-unix/2.4.7/opencv-2.4.7.tar.gz
    tar -zxvf opencv-2.4.7.tar.gz
    rm opencv-2.4.7.tar.gz
    cd opencv-2.4.7
    ./configure
    make
    make install

OpenCV doesn't currently allow turning off Auto Exposure on the camera, so I currently hackily do it with `uvcdynctrl`. It doesn't seem to be present on arch linux by default, so you'll need to install that too (included in libwebcam).

    wget http://downloads.sourceforge.net/project/libwebcam/source/libwebcam-src-0.2.4.tar.gz  
    tar -zxvf libwebcam-src-0.2.4.tar.gz 
    cd libwebcam-src-0.2.4
    cd build
    cmake ..
    make
    make install

#### Install Prerequisites (Raspbian)

```
sudo apt install git libjansson-dev liblog4cplus-dev libboost-dev wiringpi libopencv-dev libopencv-dev libboost-thread-dev libboost-date-time-dev libboost-system-dev libboost-iostreams-dev libboost-timer-dev
```

### Compiling and running

This takes about 10mins:

    cd ~/AmbiLightServer/
    mkdir build
    make

Once built, use `sh run.sh` to start the server.

At this point you may want to create a service that automatically runs it when the Pi boots.
