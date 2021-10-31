import redis

from loguru import logger as log


class Cache:
    """Communicates with the in-memory persistence that is shared with the DSP"""

    def __init__(self, redis_host: str, redis_port: int):
        self._redis_host = redis_host
        self._redis_port = redis_port
        self._instance = None
        self._connect()

    def _connect(self):
        self._instance = redis.Redis(host=self._redis_host, port=self._redis_port, db=0)
        self._instance.ping()
        log.info(f"redis connected to {self._redis_host}:{self._redis_port}")
