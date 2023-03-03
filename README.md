# rnd.tour.vps
This repo for Visual positioning system rnd project


## How to run Tests
Run python command for specific file
``` python tests/test_core.py```

How to disable noisy output from numba. ``` NUMBA_DISABLE_JIT=1 ``` Just set environment value to 1 before running testing command. 

``` os.environ['NUMBA_DISABLE_JIT'] = '1' ```

## How to build pyfeatures (Hahog)
1. Download [hahog](https://drive.google.com/drive/folders/1PFxCFIOBWzHFDKx-gQO0ipVasy4IgSEQ?usp=share_link) folder 

2. Create ```pyfeatures.so``` file 

  2-1. OpenCV library should be installed beforehand.

```
git clone https://github.com/opencv/opencv.git

git checkout 3.4.8

cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/opt/opencv/opencv.3.4.8 -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON

mkdir opencv/build
make -j2 (2: 프로세서 갯수-nproc)
sudo make install
sudo sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/opencv.conf’
sudo ldconfigmake
```

  2-2. Cmake Build in hahog directory
```
cd features/hahog
cmake . && make 
```a


3. Locate ```pyfeatures.so``` to ```rnd.visual.tour.vps/core/runners``` directory. 
