# Pull and build
git pull origin
make

# Setup
uvcdynctrl -s "Brightness" 118
uvcdynctrl -s "Contrast" 30
uvcdynctrl -s "Saturation" 73
uvcdynctrl -s "White Balance Temperature, Auto" 1
uvcdynctrl -s "Gain" 20
uvcdynctrl -s "White Balance Temperature" 4000
uvcdynctrl -s "Sharpness" 0
uvcdynctrl -s "Backlight Compensation" 1
uvcdynctrl -s "Exposure, Auto" 3
#uvcdynctrl -s "Exposure (Absolute)" 300
uvcdynctrl -s "Exposure, Auto Priority" 1

# Output settings
uvcdynctrl -g "Brightness"
uvcdynctrl -g "Contrast"
uvcdynctrl -g "Saturation"
uvcdynctrl -g "White Balance Temperature, Auto"
uvcdynctrl -g "Gain"
uvcdynctrl -g "White Balance Temperature"
uvcdynctrl -g "Sharpness"
uvcdynctrl -g "Backlight Compensation"
uvcdynctrl -g "Exposure, Auto"
uvcdynctrl -g "Exposure (Absolute)"
uvcdynctrl -g "Exposure, Auto Priority"

# Start the server
sudo ./AmbiLightServer