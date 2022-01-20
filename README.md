# ArmTTS 

ML based Text-to-Speech(TTS) C++ library for the Armenian language (light version, only CPU and Tensorflow Lite).
The full version is available here: [Text-to-Speech (TTS) Engine for the Armenian Language](https://armtts.online).

Documentation page: https://80.241.216.240/armtts/index.html.

Requirements: ```cmake, build-essentials```

Models: [D0_20210703_512k_503k_cd8119f6_lite.zip](https://www.dropbox.com/s/ipzp9a6coafr3q9/D0_20210703_512k_503k_cd8119f6_lite.zip)


### Installation steps
```
1. clone the repository
2. download and update the models placed in 'models/'
3. mkdir build && cd build && cmake .. && make -j
```

### Third party dependencies used:
- Tensorflow Lite
- Boost
- FFTW
- SndFile
- GTest
- Log4cpp
- nlohmannjson
- Doxygen

### Full version features:
- Fast: both CPU and GPU hardware is supported (Tensorflow)
- Secure: REST API uses secure HTTPS connection, DDoS protected (NGINX).
- Portable: everything is containerised with Docker Compose.
- Fully Automated: deployment to any cloud takes less than an hour (Jenkins).
 
For more contact me at albert_grigoryan@yahoo.com
