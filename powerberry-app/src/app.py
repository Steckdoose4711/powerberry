from time import sleep

import numpy as np
import numpy.typing as npt

from loguru import logger as log

from .services.cache import Cache
from .services.config import Config


class App:
    def __init__(self, config: Config):
        self.config = config
        self.cache = Cache(config.redis_host, config.redis_port)

    def run(self):
        # self.cache.push_synthetic_data()
        devices = self._query_devices()

        while True:
            for dev, channels in devices.items():
                for ch in channels:
                    self.read_channel(dev, ch)
            sleep(5)

    def read_channel(self, dev, ch) -> npt.NDArray[np.float32]:
        sample_rate = self.cache.get_sample_rate(dev, ch)
        if not sample_rate:
            log.error(f"no sample rate set for {dev}:{ch}, skip")
            return

        # wait at least until we have as many samples as the sample rate suggests
        num_samples = self.cache.get_number_of_samples(dev, ch)
        if num_samples < sample_rate:
            log.debug(f"too few samples on {dev}:{ch} yet ({num_samples}), skip")
            return

        # retrieve full blocks only
        num_blocks = num_samples // sample_rate
        arr = self.cache.get_samples(dev, ch, num_blocks * sample_rate)
        arr = arr.reshape(-1, sample_rate)

        log.info(
            f"read {num_blocks} blocks (at {sample_rate} Hz each) "
            f"from {dev}:{ch} "
            f"(mean values: {arr.mean(axis=1)})"
        )

    def _query_devices(self):
        result = {}
        devices = self.cache.get_devices()
        for dev in devices:
            channels = self.cache.get_channels(dev)
            result[dev] = channels

        log.info(f"queried devices and channels: {result}")
        return result
