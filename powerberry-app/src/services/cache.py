from typing import Set

import numpy as np
import numpy.typing as npt
import redis
from loguru import logger as log


class Cache:
    """
    Communicates with the in-memory
    persistence that is shared with the DSP
    """

    def __init__(self, redis_host: str, redis_port: int):
        self._redis_host = redis_host
        self._redis_port = redis_port

        self.redis = redis.Redis(
            host=self._redis_host,
            port=self._redis_port,
            charset="ascii",
            decode_responses=True,
        )

    def connect(self) -> bool:
        try:
            self.redis.ping()
            log.info(f"redis connected to {self._redis_host}:{self._redis_port}")
            return True
        except redis.RedisError as e:
            log.warning(f"redis connection failed: {e}")
        return False

    def get_devices(self) -> Set[str]:
        return self.redis.smembers("devices")

    def get_channels(self, device: str) -> Set[str]:
        channels = self.redis.smembers(f"device:{device}:channels")
        return set(sorted(channels))

    def get_sample_rate(self, device: str, channel: str) -> int:
        sample_rate = self.redis.get(f"device:{device}:channel:{channel}:sample_rate")
        return int(sample_rate) if sample_rate else None

    def get_number_of_samples(self, device: str, channel: str) -> int:
        return self.redis.llen(f"device:{device}:channel:{channel}:voltage")

    def get_samples(self, device: str, channel: str, count: int) -> npt.NDArray[np.float32]:
        samples = self.redis.rpop(f"device:{device}:channel:{channel}:voltage", count)
        x, y = zip(*(e.split(";") for e in samples))
        timestamps = np.array(x, dtype=np.int64)
        voltages = np.array(y, dtype=np.float32)
        return timestamps, voltages
