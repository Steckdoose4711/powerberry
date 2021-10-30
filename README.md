# PowerBerry

Measures the current consumption of your house with a Raspberry Pi and visualizes it on a Grafana dashboard.

## Raspberry Pi Development

Simply spin-up all the containers with

    docker-compose up  -d

If you made changes and want to re-build the containers, run

    docker-compose up --build --force-recreate -d

And stop them again with

    docker-compose down

## Local Development

You can build and run the individual images manually

    cd ./powerberry-app
    docker build -t powerberry-app .

    cd ./powerberry-dsp
    docker build -t powerberry-dsp .

Run the individual images

    docker run -it -d --name powerberry-app powerberry-app
    docker run -it -d --name powerberry-dsp powerberry-dsp

### Attaching to the dev container

For the DSP you can attach to the dev container (replace `$APP_PATH` with the path to `powerberry-dsp`)

    cd ./powerberry-dsp
    docker build -t powerberry-dsp-dev -f Dockerfile.dev .
    docker run -it -d --name powerberry-dsp-dev -v $APP_PATH:/app powerberry-dsp-dev

On Windows, you can use the following command from the repository root

    docker run -it -d --name powerberry-dsp-dev -v %cd%\powerberry-dsp:/app powerberry-dsp-dev

Use "Attach to running container", install the C++ extension, and start coding.

You can remove the container again with

    docker rm -f powerberry-dsp-dev

### Running a local redis server

To quickly test a Redis server, you can run a local server

    docker run -d -p 6379:6379 --name redis redis:6-alpine
