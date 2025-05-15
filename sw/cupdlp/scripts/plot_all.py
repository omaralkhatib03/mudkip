import logging
import utils

from summary_plot import plot_summary_csv
from user_events_plot import plot_user_events
from device_trace_plot import main as plot_trace

logger = utils.setup_logger(logging.DEBUG)

def main():

    # logger.info('Plotting Summary CSV')
    # plot_summary_csv('analysis/summary.csv')
    #
    logger.info('Plotting User Events')
    plot_user_events('analysis/user_events.csv')

    logger.info('Plotting Device Trace')
    plot_trace('analysis/device_trace_0.csv')

if __name__ == '__main__':
    main()
