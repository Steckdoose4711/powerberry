import json
import os
import pathlib
import sys

from loguru import logger as log


class Config:
    """Retrieves configuration from environment variables or files or fails fast otherwise"""

    def __init__(self):
        self.keys = {}

        # read configuration file first
        self.from_env("CONFIG_PATH", cast=pathlib.Path)
        self._json_config = self._read_config(self.config_path)

        # query variables
        self.from_env("REDIS_HOST", default="localhost")
        self.from_env("REDIS_PORT", default=6379, cast=int)
        self.from_cfg("app_influx_host", default="localhost")
        self.from_cfg("app_influx_port", default=8086, cast=int)
        self.from_cfg("app_voltage_reference", default=228, cast=float)
        self.from_cfg("dsp_sampling_rate", cast=int)

    def from_env(self, key, default=None, cast=None):
        value = self._get_key(key, os.environ, "env variable", default, cast)
        self.keys[key.lower()] = value

    def from_cfg(self, key, default=None, cast=None):
        value = self._get_key(key, self._json_config, "config value", default, cast)
        self.keys[key.lower()] = value

    def _get_key(self, key, source, source_name, default=None, cast=None):
        value = None
        if key in source:
            value = source[key]
        elif default is not None:
            log.debug(f"{source_name} '{key}' not set, using default '{default}'")
            value = default
        else:
            log.error(f"required {source_name} '{key}' not set ")
            sys.exit(1)

        # potentially cast retrieved value
        assert value is not None
        if cast is not None:
            value = cast(value)

        return value

    def _read_config(self, config_path):
        with open(config_path, "r") as f:
            jzon = json.load(f)
            log.debug(f"config file '{config_path}' loaded: {jzon}")
            return jzon

    def __getattr__(self, name: str):
        """Enables the retrieval of configuration keys as instance fields"""
        if name in self.keys:
            return self.keys[name]
        else:
            raise AttributeError(f"no configuration key '{name}'")
