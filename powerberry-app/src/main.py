import sys
import signal

from .app import App
from .services.config import Config


if __name__ == "__main__":
    # kill this app gracefully if we receive a SIGTERM from Docker
    signal.signal(signal.SIGTERM, lambda s, f: sys.exit(0))

    config = Config()
    app = App(config)
    app.run()
