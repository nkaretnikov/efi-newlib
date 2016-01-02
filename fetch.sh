ARCHIVE="OVMF-X64-r15214.zip"
wget "http://downloads.sourceforge.net/project/edk2/OVMF/$ARCHIVE"

ACTUAL=`sha1sum $ARCHIVE | cut -f1 -d' '`
EXPECTED="81cc401fb2a2772b5b6f677dc39c7153877a763a"
if [[ $ACTUAL != $EXPECTED ]]
then
  echo "$ARCHIVE: invalid hash: expected $EXPECTED, but got $ACTUAL"
  exit 1
fi

unzip OVMF-X64-r15214.zip OVMF.fd
echo "fetch done"
