import os
import sys

from pathlib import Path

from loguru import logger as log

from .app import App


if __name__ == "__main__":

    # get the path to the config file
    config_path_env = "CONFIG_PATH"
    if config_path_env in os.environ:
        config_path = Path(os.getenv(config_path_env))
    else:
        log.error(f"config path env variable '{config_path_env}' not set")
        sys.exit(1)

    # create and run the app
    App(config_path).run()
