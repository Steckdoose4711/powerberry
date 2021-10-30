import json

from time import sleep
from pathlib import Path

import redis
import numpy as np

from loguru import logger as log


class App:
    def __init__(self, config_path: Path, redis_host: str, redis_port: int):
        self._config_path = config_path
        self._redis_host = redis_host
        self._redis_port = redis_port

        self.config = None
        self.redis = None

        # read config and connect redis
        self._read_config()
        self._connect_redis()

    def run(self):
        log.info("powerberry app started")
        while True:
            sleep(1)

    def read_voltages(self) -> np.ndarray:
        # TODO
        pass

    def _read_config(self):
        with open(self._config_path, "r") as f:
            jzon = json.load(f)
            self.config = jzon
            log.info(f"config '{self._config_path}' loaded: {jzon}")

    def _connect_redis(self):
        self.redis = redis.Redis(host=self._redis_host, port=self._redis_port, db=0)
        self.redis.ping()
        log.info(f"redis connected to {self._redis_host}:{self._redis_port}")
