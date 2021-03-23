FROM node

RUN apt-get -qy update                              \
    && apt-get -y install --no-install-recommends   \
        make                         \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /home/node/app

COPY ./package*.json ./

# Install nodemon for hot reload
RUN npm install -g nodemon

RUN npm install

COPY . .

EXPOSE 5000

RUN cd tools/cFS-GroundSystem/Subsystems/cmdUtil && make && cd ../../../../

# run nodemon for hot reload
CMD nodemon server.js