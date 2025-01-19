![Darts!](samples/fractal-cl%284%29_g%281%29_c%28-0.835,-0.2321%29_dp%280.00625,0.00625%29_i%2832%29_s%284,2.25%29_o%280,0%29.png)

mwfractal
=========
### Dependencies ###
- automake
- imagemagick++

### build and run it in local docker
```sh
docker build -t mwfractal:asciiart --target asciiart . -f Dockerfile
docker run --rm mwfractal:asciiart --colourizer 3 --iter 48
# docker run --rm mwfractal:asciiart --help
```

### Installation Instructions (debian) ###
./bootstrap  
./configure  
make  

### Generate the Featured Image ###
On your command line: ```cjw@one$ ./mwfractal --width=640 --height=360 --colourizer=4```

### High Resolution Featured Image ###
[4K resolution featured image - 10 MB file](samples/fractal-cl%284%29_g%281%29_c%28-0.835,-0.2321%29_dp%280.00104167,0.00104167%29_i%2832%29_s%284,2.25%29_o%280,0%29.png)

### Made Using MWFractal ###
[Spiralling Towards the Julia Dragon](https://youtu.be/pL7BMrcrl_k)
1h12m of fractal morphing in 1080p60, made with mwfractal. 
