# 🔌🍇 PowerBerry 

:construction_worker: This Project is under construction at the moment. Releases will be done soon

Measures the current consumption of your house with a Raspberry Pi and visualizes it on a Grafana dashboard.


## 🍓 Raspberry Pi Deployment

Get going with pre-built images from the container registry

    docker-compose -f docker-compose.ghcr.yml up -d --build

Build a production container from scratch

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

### 📃 Development Notes

Read [REDIS.md](REDIS.md) to learn more about the cache data structure.

### Documentation for configuration.json with default values

{
  "influx_host" : "powerberry-influx", /* name of the influxdb into which the processed data will be written */
  "influx_port" : "8086", /* port of the influx db */
  "dsp_number_adc_devices" : "1", number od adc chi/* anzahl der adc chips, die für den powerberry verwendet werden. Jeder Chip hat 8 Kanäle [Default = 1] */
  "dsp_number_channels_per_device" : "3", /* number of used channels per adc chip */
  "dsp_adc_reference_voltage_V" : "4.8", /* reference voltage that is used to convert adc samples into a voltage */
  "dsp_filter_type" : "median", /* type of the filter, which is used for adc sampling (median and mean are supported) */
  "dsp_measurement_rate" : "3", /* number of measuring points per adc sample. */
  "dsp_sampling_rate" : "100", /* number of ad samples per second. depending on this setting, more or less measurement points are written to redis */
  "app_voltage_reference" : "228.0" /* Assumed grid voltage */
}



