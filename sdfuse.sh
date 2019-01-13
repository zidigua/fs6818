if [ $# != 2 ]
then
	echo "Usage $0 <dev> <file>"
	exit
fi

echo "$2 fusing..."
dd iflag=dsync oflag=dsync if=$2 of=$1 seek=1 && \
	echo "$2 image has been fused successfully."
echo "Eject SD card"

