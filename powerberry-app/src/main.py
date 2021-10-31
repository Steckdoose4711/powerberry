import os
import sys
import signal

from pathlib import Path

from loguru import logger as log

from .app import App


def get_env(key, default=None):
    """Get the value of the environment variable or exit otherwise."""
    if key in os.environ:
        return os.getenv(key)
    elif default is not None:
        log.info(f"env variable '{key}' not set, using default '{default}'")
        return default
    else:
        log.error(f"env variable '{key}' not set but required")
        sys.exit(1)


if __name__ == "__main__":
    # kill this app gracefully if we receive a SIGTERM from Docker
    signal.signal(signal.SIGTERM, lambda s, f: sys.exit(0))

    # get the path to the config file
    config_path = get_env("CONFIG_PATH", default="/config/config.json")
    redis_host = get_env("REDIS_HOST", default="localhost")
    redis_port = get_env("REDIS_PORT", default=6379)

    # create and run the app
    app = App(
        config_path=Path(config_path), redis_host=redis_host, redis_port=redis_port
    )
    app.run()
