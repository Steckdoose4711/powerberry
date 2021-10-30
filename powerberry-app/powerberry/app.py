import json

from time import sleep
from pathlib import Path

from loguru import logger as log


class App:
    def __init__(self, config_path: Path):
        self._config_path = config_path
        self.config = None
        self._read_config()

    def run(self):
        log.info("powerberry app started")
        while True:
            sleep(1)

    def _read_config(self) -> dict:
        with open(self._config_path, "r") as f:
            jzon = json.load(f)
            self.config = jzon
            log.info(f"config '{self._config_path}' loaded: {jzon}")
