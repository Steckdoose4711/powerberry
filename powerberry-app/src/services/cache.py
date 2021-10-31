from typing import Set

import redis
import numpy as np
import numpy.typing as npt

from loguru import logger as log


class Cache:
    """Communicates with the in-memory persistence that is shared with the DSP"""

    def __init__(self, redis_host: str, redis_port: int):
        self.redis = None

        self._redis_host = redis_host
        self._redis_port = redis_port
        self._connect()

    def _connect(self):
        self.redis = redis.Redis(
            host=self._redis_host,
            port=self._redis_port,
            charset="ascii",
            decode_responses=True,
        )

        self.redis.ping()
        log.info(f"redis connected to {self._redis_host}:{self._redis_port}")

    def get_devices(self) -> Set[str]:
        return self.redis.smembers("devices")

    def get_channels(self, device: str) -> Set[str]:
        channels = self.redis.smembers(f"device:{device}:channels")
        return sorted(channels)

    def get_sample_rate(self, device: str, channel: str) -> int:
        sample_rate = self.redis.get(f"device:{device}:channel:{channel}:sample_rate")
        return int(sample_rate) if sample_rate else None

    def get_number_of_samples(self, device: str, channel: str) -> int:
        return self.redis.llen(f"device:{device}:channel:{channel}:current")

    def get_samples(
        self, device: str, channel: str, count: int
    ) -> npt.NDArray[np.float32]:
        samples = self.redis.rpop(f"device:{device}:channel:{channel}:current", count)
        return np.array(samples, dtype=np.float32)

    def push_synthetic_data(self):
        # TODO: remove once the dsp is ready ...
        self.redis.sadd("devices", 0)
        self.redis.sadd("device:0:channels", 0)
        self.redis.set("device:0:channel:0:sample_rate", 100)
        self.redis.lpush("device:0:channel:0:current", *range(500))
        self.redis.ltrim("device:0:channel:0:current", 0, 60_000)
        log.warning("filled redis with synthetic data for debugging purposes")
