#!/usr/bin/env bash

# Install Python3 and Libraries to $1 
python_config() {
echo ifunc arg 1 is $1

    export PYTHON_VER="3.12.4" 
    export PYTHON_VER_SHORT="$(echo ${PYTHON_VER} | cut -d '.' -f1,2)" 
    cd $1
    pwd
    PATH=$1/python/bin:$PATH 
    wget --no-check-certificate "https://www.python.org/ftp/python/${PYTHON_VER}/Python-${PYTHON_VER}.tgz" 
    tar -zxvf $1/Python-${PYTHON_VER}.tgz 1>/dev/null 
    cd $1/Python-${PYTHON_VER}/ 
    ./configure --enable-optimizations --prefix=$1/python > /dev/null 2>&1; 
    make altinstall > /dev/null 2>&1; 
    ln -s $1/python/bin/python${PYTHON_VER_SHORT} $1/python/bin/python3 
    ln -s $1/python/bin/pip${PYTHON_VER_SHORT} $1/python/bin/pip3 
    wget --quiet --no-check-certificate https://bootstrap.pypa.io/get-pip.py -O - | python3 - --prefix=$1/python 
    source ~/.bashrc 
    $1/python/bin/pip3 install --upgrade pip 
    $1/python/bin/pip3 install --upgrade pygithub 
    $1/python/bin/pip3 install --upgrade --no-cache-dir -r /tmp/requirements.txt --use-pep517 
    cd ~ && rm -rf ~/Python-${PYTHON_VER}* 
    $1/python/bin/python3 --version 
    $1/python/bin/pip3 --version 
    echo "Python ${PYTHON_VER} - Setup Completed!" 
}

echo arg 1 is $1
python_config $1

echo Adding Python to path %1/python/bin
echo '##vso[task.prependpath]$1/python/bin'