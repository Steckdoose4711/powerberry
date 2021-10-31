import redis

from loguru import logger as log


class Cache:
    """Communicates with the in-memory persistence that is shared with the DSP"""

    def __init__(self, redis_host: str, redis_port: int):
        self.redis = None

        self._redis_host = redis_host
        self._redis_port = redis_port
        self._connect()

    def _connect(self):
        self.redis = redis.Redis(host=self._redis_host, port=self._redis_port, db=0)
        self.redis.ping()
        log.info(f"redis connected to {self._redis_host}:{self._redis_port}")

    def init_synthetic(self):
        # TODO: just for debugging purposes, push some data to redis
        self.redis.sadd("devices", 0)
        self.redis.sadd("device:0:channels", 0)
        self.redis.set("device:0:channel:0:sample_rate", 100)

    def push_synthetic(self):
        # TODO: just for debugging purposes, push some data to redis
        key = "device:0:channel:0:current"

        self.redis.lpush(key, *range(100))
        # self.redis.ltrim(key, 0, 60_000)

        count = self.redis.llen(key)
        log.info(f"number of measurements in {key}: {count:,}")
