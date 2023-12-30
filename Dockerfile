FROM alpine:3.19.0
RUN apk update && \
    apk add --no-cache \
    libstdc++ \
    build-base \
    cmake \
    freetype-dev \
    libx11-dev \
    libxcursor-dev \
    libxrandr-dev \
    mesa-dev \
    libudev-zero-dev

WORKDIR /zombie-rampage

COPY src ./src
COPY include ./include
COPY extlibs ./extlibs
COPY CMakeLists.txt ./

WORKDIR /zombie-rampage/extlibs/SFML

RUN cmake -DSFML_BUILD_NETWORK=TRUE -DSFML_BUILD_WINDOW=TRUE \ 
          -DSFML_BUILD_GRAPHICS=TRUE -DSFML_BUILD_AUDIO=FALSE \
          -DCMAKE_CXX_FLAGS="-Wno-error=sign-conversion -Wno-error=overflow" 

RUN make && make install

WORKDIR /zombie-rampage/extlibs/entityx

RUN cmake -DENTITYX_BUILD_TESTING=FALSE .
RUN make && make install

WORKDIR /zombie-rampage

RUN cmake .

RUN make

CMD ./zombie-rampage --server
