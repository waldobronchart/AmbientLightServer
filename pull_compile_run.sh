# Pull and build
git pull origin
make

# Setup
#uvcdynctrl -s "Brightness" 118
#uvcdynctrl -s "Contrast" 30
#uvcdynctrl -s "Saturation" 73
#uvcdynctrl -s "White Balance Temperature, Auto" 0
#uvcdynctrl -s "Gain" 20
#uvcdynctrl -s "White Balance Temperature" 8360
#uvcdynctrl -s "Sharpness" 0
#uvcdynctrl -s "Backlight Compensation" 1
#uvcdynctrl -s "Exposure, Auto" 3
#uvcdynctrl -s "Exposure (Absolute)" 166
#uvcdynctrl -s "Exposure, Auto Priority" 0

# Output settings
echo Brightness
uvcdynctrl -g "Brightness"

echo Contrast
uvcdynctrl -g "Contrast"

echo Saturation
uvcdynctrl -g "Saturation"

echo White Balance Temperature, Auto
uvcdynctrl -g "White Balance Temperature, Auto"

echo Gain
uvcdynctrl -g "Gain"

echo White Balance Temperature
uvcdynctrl -g "White Balance Temperature"

echo Sharpness
uvcdynctrl -g "Sharpness"

echo Backlight Compensation
uvcdynctrl -g "Backlight Compensation"

echo Exposure, Auto
uvcdynctrl -g "Exposure, Auto"

echo Exposure Absolute
uvcdynctrl -g "Exposure (Absolute)"

echo Exposure, Auto Priority
uvcdynctrl -g "Exposure, Auto Priority"

# Start the server
sudo ./build/AmbiLightServer