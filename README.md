# 🔌🍇 PowerBerry 

Measures the current consumption of your house with a Raspberry Pi and visualizes it on a Grafana dashboard.

## 🍓 Raspberry Pi Deployment

Let compose do the work

    docker-compose -f docker-compose.prod.yml up -d --build

Undeploy all containers with

    docker-compose down

### 💥 Troubleshooting

Check the logs with

    docker-compose logs -f

If you encounter errors like `PermissionError: [Errno 1] Operation not permitted` on Raspbian Buster, read [this comment](https://github.com/Steckdoose4711/powerberry/issues/13#issuecomment-955240891).

## 👩‍💻 Local Development

Likewise, let compose bootstrap a development environment

    docker-compose up -d --build

This will [bind-mount](https://docs.docker.com/storage/bind-mounts/) `./powerberry-app` and `./powerberry-dsp` into the respective containers.  
Moreover, a DSP container with a C++ toolchain is used without the app being compiled and run yet.

- Use the [Remote Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension to attach to one of the running containers
- Install the [Python](https://marketplace.visualstudio.com/items?itemName=ms-python.python) or [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) extension once attached to the container
- Use the existing `launch.json` configurations to run and debug the apps
