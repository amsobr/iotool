#!/bin/bash

# Set a default password here to be used if not yet
# set in the environment
if [ -z "$SSHPASS" ]; then
    export SSHPASS=""
fi

usage() {
    cat <<EOF
build.sh build TREE         - build package (.tar.xz)
buid.sh deploy TREE HOST    - build and deploy $NWK_PREFIX.HOST

HOST format:
          D --> 192.168.1.D"
        C.D --> 192.168.C.D"
      B.C.D --> 192.B.C.D"
    A.B.C.D --> A.B.C.D"
EOF
}


get_ip_address() {
    LEN=$(echo $1 | awk -F "." '{print NF;}')
    case "$LEN" in
    "1")
        echo -n "192.168.1.$1"
        ;;
    "2")
        echo -n "192.168.$1"
        ;;
    "3")
        echo -n "192.$1"
        ;;
    "4")
        echo "$1"
        ;;
    *)
        echo "Invalid IP Address!"
        exit
    esac
}



CMD=$1
IP_ADDR=$(get_ip_address $2)



if [ $# -eq 2 ] && [ "$1" = "build" ]; then
    MODE="generate"
    BUILD_TREE=$2
    echo "Building update/patch (NO DEPLOY) from $BUILD_TREE"
elif [ $# -eq 3 ] && [ "$1" = "deploy" ]; then
    MODE="deploy"
    BUILD_TREE=$2
    IP_ADDR=$(get_ip_address $3)
    echo "Deploying from '$BUILD_TREE' into $IP_ADDR"
else
  echo "Invalid Parameters"
  usage
  exit 1
fi


TMP_DIR=$(mktemp -d)

if [ -z "$TMP_DIR" -o "$TMP_DIR" = "/" ]; then
  echo "TMP_DIR is is empty or '/'. Cannot continue!"
  exit 1
fi


echo -e "Creating filesystem structure on $TMP_DIR ..."
mkdir -p $TMP_DIR/usr/lib/
mkdir -p $TMP_DIR/usr/bin

# copy any application specific files here...
install -v -m 0755 -D $BUILD_TREE/iotool $TMP_DIR/usr/bin/iotool


if [ "$MODE" = "deploy" ]; then
  ARCHIVE=$(mktemp -u)
else
  ARCHIVE=iotool-bin.tar.bz2
fi

REMOTE_PATH="/$(basename $ARCHIVE)"

echo -e "\nCompressing package into $ARCHIVE"
tar vcjf $ARCHIVE -C $TMP_DIR .


if [ "$MODE" = "deploy" ]; then
  echo -e "\nUploading package $ARCHIVE into $IP_ADDR:/ ..."
  sshpass -e scp -o "StrictHostKeyChecking=no" -o "UserKnownHostsFile=/dev/null" $ARCHIVE root@$IP_ADDR:$REMOTE_PATH

  echo -e "Unpacking on target ..."
  sshpass -e ssh -o "StrictHostKeyChecking=no" -o "UserKnownHostsFile=/dev/null" root@$IP_ADDR tar vxf $REMOTE_PATH -C /

  echo -e "Cleaning up ..."
  sshpass -e ssh -o "StrictHostKeyChecking=no" -o "UserKnownHostsFile=/dev/null" root@$IP_ADDR rm $REMOTE_PATH
  rm $ARCHIVE
  rm -rf $TMP_DIR
else
  echo "Update packaged into $ARCHIVE"
fi

echo "FINISHED! KTHXBYE!"
