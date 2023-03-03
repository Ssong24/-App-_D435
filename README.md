# rnd.tour.vps
This repo for Visual positioning system rnd project


## How to run Tests
Run python command for specific file
``` python tests/test_core.py```

How to disable noisy output from numba. ``` NUMBA_DISABLE_JIT=1 ``` Just set environment value to 1 before running testing command. 

``` os.environ['NUMBA_DISABLE_JIT'] = '1' ```

## How to build pyfeatures (Hahog)
1. Download [hahog](https://drive.google.com/drive/folders/1PFxCFIOBWzHFDKx-gQO0ipVasy4IgSEQ?usp=share_link) folder 

2. Build in hahog folder, then ```pyfeatures.so``` file will be created.

```
cd features/hahog
cmake . && make 
```

3. Move ```pyfeatures.so``` to ```rnd.visual.tour.vps/core/runners``` directory.
