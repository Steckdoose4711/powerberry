from time import sleep

import numpy as np

from loguru import logger as log

from .services.cache import Cache
from .services.config import Config


class App:
    def __init__(self, config: Config):
        self.config = config
        self.cache = Cache(config.redis_host, config.redis_port)

    def run(self):
        log.info("powerberry app started")
        self.cache.init_synthetic()
        while True:
            self.cache.push_synthetic()
            sleep(1)

    def read_voltages(self) -> np.ndarray:
        # TODO
        pass
