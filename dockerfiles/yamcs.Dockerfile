FROM maven:3.6-openjdk-11

WORKDIR /yamcs

CMD [ "mvn", "yamcs:run" ]