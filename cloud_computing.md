# Create R docker image
Use docker file


    FROM ubuntu:bionic
    RUN set -x \
        && apt-get update \
        && { \
            which gpg \
            || apt-get install -y --no-install-recommends gnupg \
        ; } \
        && { \
            gpg --version | grep -q '^gpg (GnuPG) 1\.' \
            || apt-get install -y --no-install-recommends dirmngr \
        ; } \
        && rm -rf /var/lib/apt/lists/*
    RUN set -x \
            && apt-get update \
            && apt-get install -y --no-install-recommends r-base r-cran-glmnet \
            && rm -rf /var/lib/apt/lists/*
    CMD ["R"]


Build the image using

    sudo docker build -t test:latest .

Check if images created

    sudo docker images

# Test run the image

Run R within docker

    sudo docker run -ti --rm test 