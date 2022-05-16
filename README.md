# 🔌🍇 PowerBerry

:construction_worker: This Project is under construction at the moment. Releases will be done soon

Measures the current consumption of your house with a Raspberry Pi and visualizes it on a Grafana dashboard.


## 🍓 Raspberry Pi Deployment

Get going with pre-built images from the container registry

    docker-compose -f docker-compose.ghcr.yml up -d

Or build a production container from scratch

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

## ✏️ Configuration

Configuration is stored in `config.json`.
Mount the folder that holds this file at the path specified by the `CONFIG_PATH` environment variable.

```jsonc
{
  "influx_host" : "powerberry-influx",      /* hostname of influxdb */
  "influx_port" : "8086",                   /* port of influxdb */
  "dsp_number_adc_devices" : "1",           /* number of used ADC chips [default: 1] */
  "dsp_number_channels_per_device" : "3",   /* number of used channels per ADC chip */
  "dsp_adc_reference_voltage_V" : "4.8",    /* reference voltage to convert ADC values to a voltage */
  "dsp_filter_type" : "median",             /* ADC sampling filter type ['median' or 'mean'] */
  "dsp_measurement_rate" : "3",             /* number of measurement points per ADC sample */
  "dsp_sampling_rate" : "100",              /* number of ADC samples per second, dictates data throughput to Redis */
  "app_voltage_reference" : "228.0"         /* assumed grid voltage */
}
```
