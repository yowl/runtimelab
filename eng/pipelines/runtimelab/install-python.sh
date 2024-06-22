#!/usr/bin/env bash

# Install Python3 to $1 
python_config() {
    export PYTHON_VER="3.12.4" 
    export PYTHON_VER_SHORT="$(echo ${PYTHON_VER} | cut -d '.' -f1,2)" 
    cd $1
    pwd
    PATH=$1/python/bin:$PATH 
    wget --no-check-certificate "https://www.python.org/ftp/python/${PYTHON_VER}/Python-${PYTHON_VER}.tgz" 
    tar -zxvf $1/Python-${PYTHON_VER}.tgz 
    cd $1/Python-${PYTHON_VER}/ 
    ./configure --enable-optimizations --prefix=$1/python  
    make altinstall 
    ls -lR $1/python
    ln -s $1/python/bin/python${PYTHON_VER_SHORT} $1/python/bin/python3 
    ln -s $1/python/bin/pip${PYTHON_VER_SHORT} $1/python/bin/pip3 
    wget --quiet --no-check-certificate https://bootstrap.pypa.io/get-pip.py -O - | python3 - --prefix=$1/python 
    source ~/.bashrc 
    $1/python/bin/pip3 install --upgrade pip 
    $1/python/bin/pip3 install --upgrade pygithub 
    cd ~ && rm -rf ~/Python-${PYTHON_VER}* 
    $1/python/bin/python3 --version 
    $1/python/bin/pip3 --version 
    echo "Python ${PYTHON_VER} - Setup Completed!" 
}

python3 --version

apt install python3

##python_config $1

#echo Adding Python to path $1/python/bin
#echo '##vso[task.prependpath]'$1/python/bin