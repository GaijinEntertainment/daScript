import logging


class LoggingObject(object):

    def _log_info(self, msg):
        self.__log(level='info', msg=msg)

    def _log_debug(self, msg):
        self.__log(level='debug', msg=msg)

    @property
    def _log_namespaces(self):
        return []

    @property
    def __logger_id(self):
        return '.'.join(map(str,
            [self.__class__.__name__] + self._log_namespaces +
            [f'obj_{id(self)}']))

    def __log(self, level, msg):
        logger = logging.getLogger(self.__logger_id)
        log_fn = getattr(logger, level)
        log_fn(msg)
