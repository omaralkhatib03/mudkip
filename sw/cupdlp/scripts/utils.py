import logging

class ColoredFormatter(logging.Formatter):
    COLORS = {
        'DEBUG': '\033[94m',   # Blue
        'INFO': '\033[96m',    # Green
        'WARNING': '\033[93m', # Yellow
        'ERROR': '\033[91m',   # Red
        'CRITICAL': '\033[95m' # Magenta
    }
    RESET = '\033[0m'

    def format(self, record):
        color = self.COLORS.get(record.levelname, self.RESET)
        levelname_colored = f"{color}{record.levelname}{self.RESET}"
        record.levelname = levelname_colored
        return super().format(record)

def setup_logger(logging_level):
    handler = logging.StreamHandler()
    formatter = ColoredFormatter(fmt="[%(levelname)s] %(asctime)s: %(message)s")
    handler.setFormatter(formatter)

    logger = logging.getLogger(__name__)
    logger.setLevel(logging_level)
    logger.addHandler(handler)
    return logger


