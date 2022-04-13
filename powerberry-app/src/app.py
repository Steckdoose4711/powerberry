from enum import Enum
from time import sleep

import numpy as np
import numpy.typing as npt
from loguru import logger as log
from redis import RedisError

from .services.cache import Cache
from .services.config import Config

CACHE_RETRY_SEC = 10
CACHE_CYCLE_SEC = 5


class AppState(Enum):
    Connecting = 1
    Processing = 2


class App:
    def __init__(self, config: Config):
        self.config = config
        self.cache = Cache(config.redis_host, config.redis_port)
        self.state = AppState.Connecting

    def run(self):
        while True:
            # state machine that re-connects
            # upon connection failures
            if self.state == AppState.Connecting:
                if self.connect():
                    self.state = AppState.Processing
            elif self.state == AppState.Processing:
                if not self.process():
                    self.state = AppState.Connecting

    def connect(self):
        connected = self.cache.connect()
        if not connected:
            log.warning(f"cache connection retry in {CACHE_RETRY_SEC} seconds ...")
            sleep(CACHE_RETRY_SEC)
        return connected

    def process(self):
        try:
            self._read_devices()
            return True
        except RedisError as e:
            log.warning(f"cache connection lost: {e}")
        return False

    def _read_devices(self):
        devices = self._query_devices()

        for dev, channels in devices.items():
            for ch in channels:
                self._read_channel(dev, ch)

        sleep(CACHE_CYCLE_SEC)

    def _query_devices(self):
        result = {}
        devices = self.cache.get_devices()
        for dev in devices:
            channels = self.cache.get_channels(dev)
            result[dev] = channels

        log.info(f"queried devices and channels: {result}")
        return result

    def _read_channel(self, dev, ch) -> npt.NDArray[np.float32]:
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
        ts, volt = self.cache.get_samples(dev, ch, num_blocks * sample_rate)
        ts, volt = ts.reshape(-1, sample_rate), volt.reshape(-1, sample_rate)

        log.info(
            f"read {num_blocks} blocks (at {sample_rate} Hz each) "
            f"from {dev}:{ch} "
            f"(mean values: {volt.mean(axis=1)} at times {ts.min()} .. {ts.max()})"
        )
