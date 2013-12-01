# Set library path, why isn't this default somewhere?
export LD_LIBRARY_PATH=/usr/local/lib

# Turn off Auto Exposure
uvcdynctrl -s "Exposure, Auto Priority" 0

# Run the server
cd build
./AmbiLightServer
